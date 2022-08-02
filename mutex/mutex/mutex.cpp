#include <cassert>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class X {
    std::mutex mtx_;
    std::vector<int> data_;
public:
    // vectorオブジェクトへのアクセスを排他的にする
    void add_value(int value)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        data_.push_back(value);
    }

    void print()
    {
        for (int x : data_) {
            std::cout << x << std::endl;
        }
    }
};

class counter {
    int count_ = 0;
    std::recursive_mutex mtx_;
public:
    int add(int value)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        count_ += value;
        return count_;
    }

    int increment()
    {
        std::lock_guard<std::recursive_mutex> lock(mtx_); // ロックを取得する
        return add(1); // add()関数内でも同じミューテックスからロックを取得する
    }
};

std::mutex print_mtx_;
void print_value(int value)
{
    std::lock_guard<std::mutex> lock(print_mtx_);
    std::cout << "count == " << value << std::endl;
}

counter c;
void change_count()
{
    int value = c.increment();
    print_value(value);
}
class counter2 {
    int count_ = 0;
    std::timed_mutex mtx_;
public:
    int add(int value)
    {
        // ロックを取得する(3秒でタイムアウト)
        if (!mtx_.try_lock_for(std::chrono::seconds(3))) {
            // ロック取得がタイムアウト
            std::error_code ec(static_cast<int>(std::errc::device_or_resource_busy), std::generic_category());
            throw std::system_error(ec);
        }

        int result = count_ += value;

        mtx_.unlock();

        return result;
    }
};

void f(counter2& c)
{
    try {
        std::cout << c.add(3) << std::endl;
    }
    catch (std::system_error& e) {
        std::cout << e.what() << std::endl;
    }
}

// std::coutへのアクセスを排他的にする
void safe_print(int x)
{
    std::lock_guard<std::mutex> lock(print_mtx_);
    std::cout << x << std::endl;
}

class Y {
    std::mutex mtx_;
    std::vector<int> data_;
public:
    // vectorオブジェクトへのアクセスを排他的にする
    void add_value(int value)
    {
        std::lock_guard<std::mutex> lock(mtx_); // ロックを取得する(lock_guardのコンストラクタ)
        data_.push_back(value);
    } // ロックを手放す(lock_guardのデストラクタ)

    void print()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (int x : data_) {
            safe_print(x);
        }
    }
};


class Z {
    std::mutex mtx_;
    std::vector<int> data_;
public:
    std::unique_lock<std::mutex> get_lock()
    {
        return std::unique_lock<std::mutex>(mtx_); // ロックを取得する
    }

    // vectorオブジェクトへのアクセスを排他的にする
    void add_value(int value)
    {
        std::unique_lock<std::mutex> lk = get_lock(); // ロックされたunique_lockを受け取る

        data_.push_back(value);
    } // ロックを手放す(unique_lockのデストラクタ)

    void print()
    {
        std::unique_lock<std::mutex> lk = get_lock();

        for (int x : data_) {
            safe_print(x);
        }
    }
};

int main()
{
    //mutexは、スレッド間で使用する共有リソースを排他制御するためのクラスである
    {
        std::cout << "mutex" << std::endl;
        X x;

        std::thread t1([&x] { x.add_value(1); });
        std::thread t2([&x] { x.add_value(2); });

        t1.join();
        t2.join();

        x.print();
    }
    //recursive_mutexはmutexクラスとは異なり、同一スレッドからの再帰的なロック取得を許可する
    {
        std::cout << "recursive_mutex" << std::endl;
        std::thread t1(change_count);
        std::thread t2(change_count);

        t1.join();
        t2.join();
    }
    //timed_mutexは、スレッド間で使用する共有リソースを排他制御するためのクラスであり、ロック取得のタイムアウト機能をサポートする
    //recursive_timed_mutexは、スレッド間で使用する共有リソースを排他制御するためのクラスであり、再帰的なロックと、ロック取得のタイムアウト機能をサポートする。lock()メンバ関数によってリソースのロックを取得し、unlock()メンバ関数でリソースのロックを手放す
    {
        std::cout << "timed_mutex" << std::endl;
        counter2 c;

        std::thread t1([&] { f(c); });
        std::thread t2([&] { f(c); });

        t1.join();
        t2.join();
    }
    //defer_lock_t型とその値defer_lockは、遅延ロックのためのタグである
    {
        std::cout << "defer_lock" << std::endl;
        std::mutex mtx;
        {
            std::unique_lock<std::mutex> lk(mtx, std::defer_lock); // ここではlock()されない
            lk.lock(); // あとからロックする

            // ...共有リソースにアクセスする...

        } // unique_lockの破棄時にunlock()される
    }
    //try_to_lock_t型とその値try_to_lockは、遅延ロックのためのタグである
    {
        std::cout << "try_to_lock_t" << std::endl;
        std::mutex mtx;
        {
            std::unique_lock<std::mutex> lk(mtx, std::try_to_lock); // lock()の代わりにtry_lock()を呼び出す
            if (!lk) {
                // ロックの取得に失敗
                std::error_code ec(static_cast<int>(std::errc::device_or_resource_busy), std::generic_category());
                throw std::system_error(ec);
            }

            // ...共有リソースにアクセスする...

        } // unique_lockの破棄時にunlock()される
    }
    //adopt_lock:ロック済みミューテックスを受け取るためのタグである
    {
        std::cout << "adopt_lock" << std::endl;
        std::mutex mtx;
        {
            mtx.lock(); // ロックを取得する
            std::unique_lock<std::mutex> lk(mtx, std::adopt_lock); // ロック済みミューテックスの管理を委譲する

            // ...共有リソースにアクセスする...

        } // unique_lockの破棄時にunlock()される
    }
    //lock_guardは、ミューテックスのlock()/unlock()処理をコンストラクタとデストラクタで確実に実行するためのクラスである
    {
        std::cout << "lock_guard" << std::endl;
        Y x;

        std::thread t1([&x] { x.add_value(1); });
        std::thread t2([&x] { x.add_value(2); });

        t1.join();
        t2.join();

        x.print();
    }
    //scoped_lock:複数のミューテックスに対するロック取得と解放を、コンストラクタとデストラクタで確実に実行するためのクラス
    {
        std::cout << "scoped_lock" << std::endl;
        std::mutex m1;
        std::timed_mutex m2;

        {
            // m1とm2のロックを取得
            std::scoped_lock lk{ m1, m2 };

            // m1のミューテックスで保護されたデータと、
            // m2のミューテックスで保護されたデータを操作・・・

        } // lkのデストラクタによって、m1とm2のロックを解放
    }
    //unique_lockは、ミューテックスのlock()／unlock()処理を、コンストラクタとデストラクタで確実に実行するためのクラスである
    {
        std::cout << "unique_lock" << std::endl;
        Z x;

        std::thread t1([&x] { x.add_value(1); });
        std::thread t2([&x] { x.add_value(2); });

        t1.join();
        t2.join();

        x.print();
    }
    //try_lock:複数のミューテックスオブジェクトに対してtry_lock操作を行う
    {
        std::cout << "try_lock" << std::endl;
        std::mutex mtx1;
        std::recursive_mutex mtx2;

        // 複数のミューテックスオブジェクトに対してtry_lock()を呼び出す
        {
            int result = std::try_lock(mtx1, mtx2);

            assert(result == -1); // 全てのtry_lock()呼び出しが成功

            mtx1.unlock();
            mtx2.unlock();
        }

        // unique_lockに対してtry_lock()を呼び出す
        {
            std::unique_lock<std::mutex> lk1(mtx1, std::defer_lock);
            std::unique_lock<std::recursive_mutex> lk2(mtx2, std::defer_lock);

            int result = std::try_lock(lk1, lk2);

            assert(result == -1); // 全てのtry_lock()呼び出しが成功
        }

        // 一部のtry_lock()が失敗する場合
        {
            // mtx2をロックしておく。
            std::lock_guard<std::recursive_mutex> lk2_main_thread(mtx2);

            std::thread th([&]
                {
                    std::unique_lock<std::mutex> lk1(mtx1, std::defer_lock);
                    std::unique_lock<std::recursive_mutex> lk2(mtx2, std::defer_lock);

                    // 他のスレッドでmtx2をロックしているため、lk2のロックに失敗する。
                    int result = std::try_lock(lk1, lk2);

                    // lk2が失敗したので第2引数を示す1が返る(0始まり)
                    assert(result == 1);

                    // lk2が失敗したので、std::try_lock()内でlk2より前にtry_lock()が
                    // 成功した全てのミューテックスオブジェクトがunlock()される
                    assert(!lk1.owns_lock());
                });
            th.join();
        }
    }
    //lock:複数のミューテックスオブジェクトに対してlock操作を行う
    {
        std::cout << "lock" << std::endl;
        std::mutex mtx1;
        std::recursive_mutex mtx2;

        // 複数のミューテックスオブジェクトのロック取得を行う
        {
            std::lock(mtx1, mtx2);

            mtx1.unlock();
            mtx2.unlock();
        }

        // unique_lockに対してロック取得を行う
        {
            std::unique_lock<std::mutex> lk1(mtx1, std::defer_lock);
            std::unique_lock<std::recursive_mutex> lk2(mtx2, std::defer_lock);

            std::lock(lk1, lk2);
        }

        // 一部のlock()が失敗する場合
        {
            std::unique_lock<std::mutex> lk1(mtx1, std::defer_lock);
            std::unique_lock<std::recursive_mutex> lk2(mtx2, std::defer_lock);

            lk2.lock(); // ロック取得済みにしてlock()に渡す

            try {
                std::lock(lk1, lk2);
            }
            catch (std::system_error& e) {
                std::cout << e.what() << std::endl;
            }

            // lk2が失敗したので、std::lock()内でlk2より前にロック取得が
            // 成功した全てのミューテックスオブジェクトがunlock()される
            assert(!lk1.owns_lock());

            // lk2はロック取得済みで渡したので、ロック取得済み状態のまま
            assert(lk2.owns_lock());
        }
    }
    std::cin.get();
}

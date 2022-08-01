#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
int data;
std::atomic<bool> ready(false);

void f()
{
    while (!ready.load(std::memory_order_acquire)) {
    }

    std::cout << data << std::endl;   // (2)
}

std::atomic<int> a(-1);
int table[10];

void g()
{
    table[3] = 5;
    table[5] = -1;
    a.store(3, std::memory_order_release);
}

class my_mutex {
    std::atomic<bool> state_{ false }; // false:unlock, true:lock
public:
    void lock() noexcept {
        while (std::atomic_exchange(&state_, true) == true) {
            std::atomic_wait(&state_, true);
        }
    }

    void unlock() noexcept {
        std::atomic_store(&state_, false);
        std::atomic_notify_one(&state_);
    }
};

my_mutex mut;
void print(int x) {
    mut.lock();
    std::cout << x << std::endl;
    mut.unlock();
}

class spinlock {
private:
    std::atomic_flag state_;

public:
    spinlock() : state_(ATOMIC_FLAG_INIT) {}

    void lock()
    {
        // 現在の状態をロック状態にする
        while (state_.test_and_set(std::memory_order_acquire)) {
            // busy-wait...アンロックされるまで待機
        }
    }

    void unlock()
    {
        // 値をアンロック状態にする
        state_.clear(std::memory_order_release);
    }
};


namespace {
    spinlock lock;
}

template <class T>
void print(const T& x)
{
    std::lock_guard<spinlock> lk(lock);
    std::cout << x << std::endl;
}

void a()
{
    print(1);
}

void b()
{
    print(2);
}

int main()
{
    //memory_order:	メモリオーダーの種類
    {
        std::cout << "memory_order" << std::endl;
        std::thread t(f);

        data = 3;   // (1)
        ready.store(true, std::memory_order_release);

        t.join();
    }
    //kill_dependency:データ依存性を切る
    {
        std::cout << "kill_dependency" << std::endl;
        std::thread t1(g);

        int r0;
        do {
            r0 = a.load(std::memory_order_consume);
        } while (r0 < 0);
        std::cout << r0 << std::endl; // 正しく同期化されており、必ず"3"が出力される。

        int r1 = table[r0];
        std::cout << r1 << std::endl; // 正しく同期化されており、必ず"5"が出力される。

        int r2 = table[r1];
        std::cout << r2 << std::endl; // 正しく同期化されており、必ず"-1"が出力される。

        int r3 = table[std::kill_dependency(r1)];  // data race. 未定義動作。
        std::cout << r3 << std::endl; // 最適化などの結果、"-1"以外が出力される可能性がある。


        t1.join();
    }
    //LOCK_FREE
    //0	その型はロックフリーに振る舞うことはできない
    //1	その型はロックフリーに振る舞うことがある
    //2	その型は常にロックフリーに振る舞う
    {
        std::cout << "LOCK_FREE" << std::endl;
        std::cout << "bool      : " << ATOMIC_BOOL_LOCK_FREE << std::endl;
        std::cout << "char      : " << ATOMIC_CHAR_LOCK_FREE << std::endl;
        std::cout << "char8_t   : " << ATOMIC_CHAR8_T_LOCK_FREE << std::endl;
        std::cout << "char16_t  : " << ATOMIC_CHAR16_T_LOCK_FREE << std::endl;
        std::cout << "char32_t  : " << ATOMIC_CHAR32_T_LOCK_FREE << std::endl;
        std::cout << "wchar_t   : " << ATOMIC_WCHAR_T_LOCK_FREE << std::endl;
        std::cout << "short     : " << ATOMIC_SHORT_LOCK_FREE << std::endl;
        std::cout << "int       : " << ATOMIC_INT_LOCK_FREE << std::endl;
        std::cout << "long      : " << ATOMIC_LONG_LOCK_FREE << std::endl;
        std::cout << "long long : " << ATOMIC_LLONG_LOCK_FREE << std::endl;
        std::cout << "T*        : " << ATOMIC_POINTER_LOCK_FREE << std::endl;
    }
    //atomic_is_lock_free:オブジェクトがロックフリーに振る舞えるかを判定する
    {
        std::cout << "atomic_is_lock_free" << std::endl;
        std::atomic<int> x(3);

        if (std::atomic_is_lock_free(&x)) {
            std::cout << "atomic<int> is lock-free" << std::endl;
        }
        else {
            std::cout << "atomic<int> isn't lock-free" << std::endl;
        }
    }
    //atomic_store:アトミックに値を書き込む
    {
        std::cout << "atomic_store" << std::endl;
        std::atomic<int> x(3);

        // 2を書き込む
        std::atomic_store(&x, 2);

        std::cout << x.load() << std::endl;
    }
    //atomic_store_explicit:指定されたメモリオーダーにしたがってアトミックに値を書き込む
    {
        std::cout << "atomic_store_explicit" << std::endl;
        std::atomic<int> x(3);

        // 2を書き込む
        std::atomic_store_explicit(&x, 2, std::memory_order_release);

        std::cout << x.load() << std::endl;
    }
    //atomic_load:メモリオーダーにしたがって、アトミックに値を読み込む
    {
        std::cout << "atomic_load" << std::endl;
        std::atomic<int> x(3);

        // 値を読み込む
        int result = std::atomic_load(&x);

        std::cout << result << std::endl;
    }
    //atomic_load_explicit:指定されたメモリオーダーにしたがって、アトミックに値を読み込む
    {
        std::cout << "atomic_load_explicit" << std::endl;
        std::atomic<int> x(3);

        // 値を読み込む
        int result = std::atomic_load_explicit(&x, std::memory_order_acquire);

        std::cout << result << std::endl;
    }
    //atomic_exchange:アトミックに値を入れ替える
    {
        std::cout << "atomic_exchange" << std::endl;
        std::atomic<int> x(1);

        if (std::atomic_exchange(&x, 2) == 1) {
            std::cout << "replaced 1 by 2" << std::endl;
        }
        else {
            std::cout << "replace failed" << std::endl;
        }
    }
    //atomic_exchange_explicit指定されたメモリオーダーにしたがって、アトミックに値を入れ替える
    {
        std::cout << "atomic_exchange_explicit" << std::endl;
        std::atomic<int> x(1);

        if (std::atomic_exchange_explicit(&x, 2, std::memory_order_acquire) == 1) {
            std::cout << "replaced 1 by 2" << std::endl;
        }
        else {
            std::cout << "replace failed" << std::endl;
        }
    }
    //atomic_compare_exchange_weak:弱い比較でアトミックに値を入れ替える
    {
        std::cout << "atomic_compare_exchange_weak" << std::endl;
        {
            std::atomic<int> x(3);

            // x == expectedなので、xは2に置き換えられる
            int expected = 3;
            bool result = std::atomic_compare_exchange_weak(&x, &expected, 2);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
        {
            std::atomic<int> x(3);

            // x != expectedなので、expectedがxの値で置き換えられる
            int expected = 1;
            bool result = std::atomic_compare_exchange_weak(&x, &expected, 2);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
    }
    //atomic_compare_exchange_strong:強い比較でアトミックに値を入れ替える
    {
        std::cout << "atomic_compare_exchange_strong" << std::endl;
        {
            std::atomic<int> x(3);

            // x == expectedなので、xは2に置き換えられる
            int expected = 3;
            bool result = std::atomic_compare_exchange_strong(&x, &expected, 2);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
        {
            std::atomic<int> x(3);

            // x != expectedなので、expectedがxの値で置き換えられる
            int expected = 1;
            bool result = std::atomic_compare_exchange_strong(&x, &expected, 2);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
    }
    //atomic_compare_exchange_weak_explicit:弱い比較でアトミックに値を入れ替える
    {
        std::cout << "atomic_compare_exchange_weak_explicit" << std::endl;
        {
            std::atomic<int> x(3);

            // x == expectedなので、xは2に置き換えられる
            int expected = 3;
            bool result = std::atomic_compare_exchange_weak_explicit(&x, &expected, 2,
                std::memory_order_acquire,
                std::memory_order_acquire);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
        {
            std::atomic<int> x(3);

            // x != expectedなので、expectedがxの値で置き換えられる
            int expected = 1;
            bool result = std::atomic_compare_exchange_weak_explicit(&x, &expected, 2,
                std::memory_order_acquire,
                std::memory_order_acquire);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
    }
    //atomic_compare_exchange_strong_explicit:強い比較でアトミックに値を入れ替える
    {
        std::cout << "atomic_compare_exchange_strong_explicit" << std::endl;
        {
            std::atomic<int> x(3);

            // x == expectedなので、xは2に置き換えられる
            int expected = 3;
            bool result = std::atomic_compare_exchange_strong_explicit(&x, &expected, 2,
                std::memory_order_acquire,
                std::memory_order_acquire);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
        {
            std::atomic<int> x(3);

            // x != expectedなので、expectedがxの値で置き換えられる
            int expected = 1;
            bool result = std::atomic_compare_exchange_strong_explicit(&x, &expected, 2,
                std::memory_order_acquire,
                std::memory_order_acquire);

            std::cout << std::boolalpha << result << " " << x.load() << " " << expected << std::endl;
        }
    }
    //atomic_wait:起床されるまで待機する
    //atomic_notify_one:待機しているスレッドをひとつ起床させる
    {
        std::cout << "atomic_wait/atomic_notify_one" << std::endl;
        std::thread t1{ [] {
        for (int i = 0; i < 5; ++i) {
            print(i);
        }
        } };
        std::thread t2{ [] {
          for (int i = 5; i < 10; ++i) {
            print(i);
          }
        } };

        t1.join();
        t2.join();
    }
    //atomic_notify_all:待機している全てのスレッドを起床させる
    {
        std::cout << "atomic_notify_all" << std::endl;
        std::atomic<bool> x{ false };

        auto f = [&x] {
            while (true) {
                std::atomic_wait(&x, false);
                if (std::atomic_load(&x) == true) {
                    break;
                }
            }
        };

        std::thread t1{ f };
        std::thread t2{ f };

        std::atomic_store(&x, true);
        std::atomic_notify_all(&x);

        t1.join();
        t2.join();
    }
    //atomic_flag:フラグを表現するためのアトミッククラス
    {
        std::cout << "atomic_flg" << std::endl;
        std::thread t1(f);
        std::thread t2(g);

        t1.join();
        t2.join();
    }

    std::cin.get();
}


#include <iostream>
#include <future>
#include <string>
#include <thread>
#include <utility>

void foo(std::promise<int> p)
{
    p.set_value(3);

    try {
        p.set_value(1); // promiseに2回以上書き込むとエラー
    }
    catch (std::future_error& e) {
        std::cout << "value:" << e.code().value() << std::endl;
        std::cout << "what:" << e.what() << std::endl;
    }
}

void calc(std::promise<int> p)
{
    int sum = 0;
    for (int i = 0; i < 10; ++i) {
        sum += i + 1;
    }

    p.set_value(sum); // 結果値を書き込む
}
std::mutex print_mtx_;
template <class T>
void print(const T& x)
{
    std::lock_guard<std::mutex> lk(print_mtx_);
    std::cout << x << std::endl;
}

void process(std::shared_future<int> f)
{
    // 各shared_futureオブジェクトから結果値を取り出す
    int result = f.get();

    print(result);
}

int foo2() { std::cout << "executes foo()\n"; return 3; }

int main()
{
    //is_error_code_enum:future_errcに対するis_error_code_enumの特殊化
    {
        std::cout << "is_error_code_enum" << std::endl;
        std::error_code ec(std::future_errc::broken_promise);
        // 内部でstd::make_error_code(std::future_errc::broken_promise)が呼ばれる

        std::cout << ec.message() << std::endl;
    }
    //make_error_code:future_errcからerror_codeを生成する
    {
        std::cout << "make_error_code" << std::endl;
        std::error_code ec = std::make_error_code(std::future_errc::broken_promise);

        std::cout << "category : " << ec.category().name() << std::endl;
        std::cout << "value : " << ec.value() << std::endl;
        std::cout << "message : " << ec.message() << std::endl;
    }
    //make_error_condition:future_errcからerror_conditionを生成する
    {
        std::cout << "make_error_condition" << std::endl;
        std::error_condition ec = std::make_error_condition(std::future_errc::broken_promise);

        std::cout << "category : " << ec.category().name() << std::endl;
        std::cout << "value : " << ec.value() << std::endl;
        std::cout << "message : " << ec.message() << std::endl;
    }
    //future_category:future_errcのためのエラーカテゴリを取得する
    {
        std::cout << "future_category" << std::endl;
        const std::error_category& cat = std::future_category();

        std::cout << cat.name() << std::endl;
        std::cout << cat.message(static_cast<int>(std::future_errc::broken_promise)) << std::endl;
    }
    //future_errorは、future/promise操作でのエラーを扱うための例外クラスである
    {
        std::cout << "future_error" << std::endl;
        std::promise<int> p;
        std::future<int> f = p.get_future();

        std::thread t(foo, std::move(p));

        std::cout << f.get() << std::endl;

        t.join();
    }
    //promiseは、「別スレッドでの処理完了を待ち、その処理結果を取得する」といった非同期処理を実現するためのクラス
    {
        std::cout << "promise" << std::endl;
        std::promise<int> p;
        std::future<int> f = p.get_future();

        // 別スレッドで計算を行う
        std::thread t(calc, std::move(p));

        // calc()によって書き込まれた結果を取得
        std::cout << f.get() << std::endl;

        t.join();
    }
    //futureは、「別スレッドでの処理完了を待ち、その処理結果を取得する」といった非同期処理を実現するためのクラスであり、promiseクラスと組み合わせて使用する
    {
        std::cout << "future" << std::endl;
        std::promise<int> p;
        std::future<int> f = p.get_future();

        // 別スレッドで計算を行う
        std::thread t(calc, std::move(p));

        // calc()によって書き込まれた結果を取得
        std::cout << f.get() << std::endl;

        t.join();
    }
    //shared_futureは、futureクラスオブジェクトから変換によって生成されるクラスである
    {
        std::cout << "shared_future" << std::endl;
        std::promise<int> p;
        std::shared_future<int> f = p.get_future().share();

        std::thread t1(process, f);
        std::thread t2(process, f);

        int value = 3; // 何らかの計算
        p.set_value(value);  // 計算結果を設定する

        t1.join();
        t2.join();
    }
    //packaged_taskは、「別スレッドでの処理完了を待ち、その処理結果を取得する」といった非同期処理を実現するためのクラス
    {
        std::cout << "packaged_task" << std::endl;
        //std::packaged_task<int()> task(calc); // 非同期実行する関数を登録する
        //std::future<int> f = task.get_future();

        //// 別スレッドで計算を行う
        //std::thread t(std::move(task));
        //t.detach();

        //try {
        //    // 非同期処理の結果値を取得する
        //    std::cout << f.get() << std::endl;
        //}
        //catch (...) {
        //    // 非同期実行している関数内で投げられた例外を捕捉
        //}
    }
    //async:関数を非同期実行する
    {
        // 新たなスレッドで関数foo()を非同期実行
        {
            std::cout << "invokes std::async(std::launch::async, foo2)" << std::endl;
            std::future<int> f = std::async(std::launch::async, foo2);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << "main thread: slept for 10 msec\n";
            // 非同期実行の結果を取得
            int result = f.get();
            std::cout << "foo() = " << result << std::endl;
        }

        std::cout << std::endl;

        // 関数fを遅延状態で非同期実行
        {
            // この段階では関数foo()を実行しない
            std::cout << "invokes std::async(std::launch::deferred, foo2)" << std::endl;
            std::future<int> f = std::async(std::launch::deferred, foo2);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << "main thread: slept for 10 msec\n";

            // 非同期実行の結果を取得
            // この段階で関数foo()を実行
            int result = f.get();
            std::cout << "foo() = " << result << std::endl;
        }
    }
    std::cin.get();
}


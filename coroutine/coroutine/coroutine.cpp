#include <coroutine>
#include <iostream>
#include <utility>

struct task;

struct task_promise {
    task get_return_object();
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    auto yield_value(int) { return std::suspend_always{}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
};

struct task {
    using coro_handle = std::coroutine_handle<task_promise>;
    explicit task(task_promise& p)
        : coro_(coro_handle::from_promise(p)) {}
    task(task const&) = delete;
    task(task&& rhs)
        : coro_(std::exchange(rhs.coro_, nullptr)) {}
    ~task()
    {
        if (coro_)
            coro_.destroy();
    }

    void next()
    {
        if (!coro_.done())
            coro_.resume();
    }

private:
    coro_handle coro_;
};

task task_promise::get_return_object()
{
    return task{ *this };
}

// 戻り値taskの関数をコルーチンにアダプトする
template <typename... ArgTypes>
struct std::coroutine_traits<task, ArgTypes...> {
    using promise_type = task_promise;
};

task coro()
{
    std::cout << "coro#1" << std::endl;
    co_yield{};
    std::cout << "coro#2" << std::endl;
}


struct task2 {
    struct promise_type {
        int value_;
        auto get_return_object() { return task2{ *this }; }
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_value(int x) { value_ = x; }
        void unhandled_exception() { std::terminate(); }
    };

    ~task2()
    {
        if (coro_)
            coro_.destroy();
    }

    task2(task2 const&) = delete;
    task2(task2&& rhs)
        : coro_(std::exchange(rhs.coro_, nullptr)) {}

    int get()
    {
        if (!coro_.done()) {
            coro_.resume();
        }
        return coro_.promise().value_;
    }

private:
    explicit task2(promise_type& p)
        : coro_(std::coroutine_handle<promise_type>::from_promise(p)) {}

    std::coroutine_handle<promise_type> coro_;
};

task2 f()
{
    std::cout << "coroutine" << std::endl;
    co_return 42;
}

struct task3 {
    struct promise_type {
        int value_;
        auto get_return_object() { return task3{ *this }; }
        auto initial_suspend()
        {
            return std::suspend_never{};
        }
        auto final_suspend() noexcept
        {
            return std::suspend_always{};
        }
        void return_value(int x) { value_ = x; }
        void unhandled_exception() { std::terminate(); }
    };

    using coro_handle = std::coroutine_handle<promise_type>;

    ~task3()
    {
        if (coro_)
            coro_.destroy();
    }

    task3(task3 const&) = delete;
    task3(task3&& rhs)
        : coro_(std::exchange(rhs.coro_, nullptr)) {}

    int get()
    {
        if (!coro_.done()) {
            coro_.resume();
        }
        return coro_.promise().value_;
    }

private:
    explicit task3(promise_type& p)
        : coro_(coro_handle::from_promise(p)) {}

    coro_handle coro_;
};

task3 f2()
{
    std::cout << "coroutine" << std::endl;
    co_return 42;
}



struct task4 {
    struct promise_type {
        int value_;
        auto get_return_object() { return task4{ *this }; }
        auto initial_suspend()
        {
            return std::suspend_never{};
        }
        auto final_suspend() noexcept
        {
            return std::suspend_always{};
        }
        void return_value(int x) { value_ = x; }
        void unhandled_exception() { std::terminate(); }
    };

    using coro_handle = std::coroutine_handle<promise_type>;

    ~task4()
    {
        if (coro_)
            coro_.destroy();
    }

    task4(task4 const&) = delete;
    task4(task4&& rhs)
        : coro_(std::exchange(rhs.coro_, nullptr)) {}

    int get()
    {
        if (!coro_.done()) {
            coro_.resume();
        }
        return coro_.promise().value_;
    }

private:
    explicit task4(promise_type& p)
        : coro_(coro_handle::from_promise(p)) {}

    coro_handle coro_;
};

task4 f3()
{
    std::cout << "coroutine" << std::endl;
    co_return 42;
}
int main()
{
    //coroutine_traits:コルーチンのPromise型を指定するためのトレイト
    {
        std::cout << "coroutine_traits";
        auto t = coro();
        std::cout << "main#1" << std::endl;
        t.next();
        std::cout << "main#2" << std::endl;
    }
    //coroutine_handle:コルーチンに対応するコルーチンハンドル。 テンプレートパラメータPromiseには、コルーチンのPromise型を指定する
    {
        std::cout << "/coroutine_handle";
        auto c = f();
        std::cout << "main" << std::endl;
        int r = c.get();
        std::cout << "result=" << r << std::endl;
    }
    //suspend_never;コルーチンを中断させない、トリビアルAwaitable型
    {
        std::cout << "suspend_never";
        auto c = f2();
        std::cout << "main" << std::endl;
        int r = c.get();
        std::cout << "result=" << r << std::endl;
    }
    //suspend_never:コルーチンを中断させない、トリビアルAwaitable型
    {
        auto c = f();
        std::cout << "main" << std::endl;
        int r = c.get();
        std::cout << "result=" << r << std::endl;
    }
    std::cin.get();
}

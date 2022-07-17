#include <iostream>
#include <queue>

int main()
{
    {
        // intを要素に持つ優先順位付きキュー。
        // 降順に処理する
        std::priority_queue<int> que;
        // データを追加する
        que.push(3);
        que.push(1);
        que.push(4);

        // 処理順に出力する
        while (!que.empty()) {
            std::cout << "less : " << que.top() << std::endl;
            que.pop();
        }
    }

    std::cout << std::endl;

    {
        std::priority_queue<
            int,                // 要素の型はint
            std::vector<int>,   // 内部コンテナはstd::vector (デフォルトのまま)
            std::greater<int>   // 昇順 (デフォルトはstd::less<T>)
        > que;

        que.push(3);
        que.push(1);
        que.push(4);

        while (!que.empty()) {
            std::cout << "greater : " << que.top() << std::endl;
            que.pop();
        }
    }

    std::cout << std::endl;

    // 処理順を表す比較関数オブジェクトにラムダ式を使用する
    {
        auto compare = [](int a, int b) {
            return (a % 10 < b % 10); //1桁目の数字で優先度決定
        };

        std::priority_queue<
            int,
            std::vector<int>,
            decltype(compare) // 比較関数オブジェクトを指定
        > que{ compare };

        que.push(31);
        que.push(13);
        que.push(9);
        que.push(1002);

        while (!que.empty()) {
            std::cout << que.top() << std::endl;
            que.pop();
        }
    }

    std::cin.get();
}
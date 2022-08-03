
#include <iostream>
#include <semaphore>
#include <thread>
#include <queue>
int main()
{
	//counting_semaphoreクラスは、スレッド間で使用する共有リソースへの並行アクセスを制約する、軽量な同期プリミティブ
	{
        // アイテム在庫
        std::queue<int> stock;

        // 在庫管理カウンティングセマフォ（初期値=0／最大値=3）
        std::counting_semaphore<3> token{ 0 };
        // 在庫アクセス保護バイナリセマフォ（初期値=1／最大値=1）
        std::counting_semaphore<1> guard{ 1 };
        // 説明のためテンプレートパラメータleast_max_valueを明示指定しているが、
        // 両者ともstd::counting_semaphore<>と省略しても正しく動作する。

        // 生産者スレッド
        std::thread producer([&] {
            for (int i = 1; i <= 3; i++) {
                // 新しいアイテムを生産
                int item = i * 100;
                // アイテム在庫へ追加
                guard.acquire();
                stock.push(item);
                guard.release();
                // 在庫管理カウントを+1する
                token.release();
            }
            });

        // 消費者スレッド
        std::thread consumer([&] {
            for (int i = 1; i <= 3; i++) {
                // 在庫生産されるまで待機し、在庫管理カウントを-1する
                token.acquire();
                // アイテム在庫から取出
                guard.acquire();
                int item = stock.front();
                stock.pop();
                guard.release();
                // アイテムを消費
                std::cout << item << std::endl;
            }
            });

        producer.join();
        consumer.join();
	}
    //binary_semaphore	バイナリセマフォ counting_semaphore<1>	
    {
       
    }
	std::cin.get();
}


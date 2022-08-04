
#include <iostream>
#include <latch>
#include <mutex>

constexpr int NWORKER = 3;  // ワーカ数

std::mutex cout_mtx;  // 行単位cout出力用ミューテックス

int main()
{
    // 1回だけ初期化される共有データ
    int shared_data = 0;

    // 初期化済みを表すラッチ: 初期カウント値=1
    std::latch initialized{ 1 };
    // タスク完了を表すラッチ: 初期カウント値=ワーカ数+1(メインスレッド)
    std::latch completed{ NWORKER + 1 };

    // ワーカスレッド群をFire-and-Forget起動
    for (int id = 1; id <= NWORKER; id++) {
        std::thread([&, id] {
            // 共有データの初期化完了を待機
            initialized.wait();
            // メインスレッド上でのshared_data代入完了は保証されており、
            // 以降はshared_data読出のみであればデータ競合発生しない。
            int local_data = shared_data;

            { // ワーカスレッドのタスクを実行
                std::lock_guard lk{ cout_mtx };
                std::cout << "Worker#" << id << ":" << local_data << std::endl;
            }

            // タスク完了を通知
            completed.count_down();
            // ワーカスレッドはブロックされずそのまま終了する
            }).detach();
    }

    // 共有データを初期化
    shared_data = 42;

    // 共有データ初期化完了をワーカスレッド群へ通知
    initialized.count_down();
    // メインスレッドはブロックされず後続行を実行する

    { // メインスレッドのタスクを実行
        std::lock_guard lk{ cout_mtx };
        std::cout << "Main:" << shared_data << std::endl;
    }

    // メインスレッドのタスク完了通知 兼 全ワーカスレッドのタスク完了待機
    completed.arrive_and_wait();
}

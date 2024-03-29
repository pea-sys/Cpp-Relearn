﻿#include <barrier>
#include <iostream>
#include <thread>
#include <mutex>
constexpr int NWORKERS = 2;  // ワーカ数
constexpr int NPHASES = 3;  // フェーズ数

std::mutex cout_mtx;  // 行単位cout出力用ミューテックス

// このプログラムでは (NWORKERS+1)*NPHASES = 9個のタスクを実行する。
// 同じフェーズに属するタスクは複数のスレッド上で同時並行に実行される一方、
// バリアにより異なるフェーズに属するタスクが同時実行されないことを保証する。
// ここでは周期的なFork-Joinモデルのタスク並行実行が行われる。
int main()
{
    // バリア同期: 初期カウント値=ワーカ数+1(メインスレッド)
    std::barrier<> sync{ NWORKERS + 1 };

    // ワーカスレッド群をFire-and-Forget起動
    for (int id = 1; id <= NWORKERS; id++) {
        std::thread([&, id] {
            for (int phase = 1; phase <= NPHASES; phase++) {
                { // ワーカスレッドのフェーズタスクを実行
                    std::lock_guard lk{ cout_mtx };
                    std::cout << "Worker#" << id << " " << phase << std::endl;
                }

                // 合流ポイント: メインスレッド＋他ワーカスレッドと同期
                sync.arrive_and_wait();
            }
            }).detach();
    }

    // メインスレッド処理
    for (int phase = 1; phase <= NPHASES; phase++) {
        { // メインスレッドのフェーズタスクを実行
            std::lock_guard lk{ cout_mtx };
            std::cout << "Main     " << phase << std::endl;
        }

        // 合流ポイント: 全ワーカスレッドと同期
        sync.arrive_and_wait();
    }
}
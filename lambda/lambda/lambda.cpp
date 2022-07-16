/* ラムダ式(lambda expressions)」は、簡易的な関数オブジェクトをその場で定義するための機能
* 
ラムダ式によって自動的に定義される関数オブジェクトは、それぞれが一意な型名を持ち、その型名をユーザーが知る方法はない。
そのため、ラムダ式によって定義された関数オブジェクトを変数に持つためには、autoを使用して型推論するか
、std::functionクラスの変数に保持するかの、いずれかの方法をとることになる。

構文
[キャプチャリスト](パラメータリスト) mutable 例外仕様 属性 -> 戻り値の型 { 関数の本体 }
*/
#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
// コピーキャプチャの例
void copy_capture_example()
{
	int a = 0;
	auto f = [a] { return a; }; // 変数aをコピーキャプチャする
	auto g = [=] { return a; }; // デフォルトで環境にある変数をコピーして、ラムダ式のなかで使用する
	a = 1;

	// ラムダ式を定義した時点での変数aがコピーされ、使用されるので、
	// ラムダ式のなかでは、変数aの値は0となる
	int result = f(); // result == 0
	std::cout << result << std::endl;
	result = g();
	std::cout << result << std::endl;
}

// 参照キャプチャの例
void reference_capture_example()
{
	int a = 0;
	auto f = [&a] { return a; }; // 変数aを参照キャプチャする
	auto g = [&] { return a; }; //デフォルトで環境にある変数を参照して、ラムダ式のなかで使用する
	a = 1;

	// 参照キャプチャした変数は、ラムダ式を実行する時点での値となるので、
	// ラムダ式のなかでは、変数aの値は1となる
	int result = f(); // result == 1
	std::cout << result << std::endl;
	result = g();
	std::cout << result << std::endl;
}
std::function<int(int)> foo()
{
	int n = 3;
	return [&n](int i) -> int { return n + i; };
}

void proc(std::function<void()> on_complete)
{
	std::cout << std::endl;
	// …時間のかかる処理…
	for (int i = 0; i < 100; ++i) {
		std::cout << "." ;
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	std::cout << std::endl;

	// 完了ハンドラを呼び出す
	on_complete();
}
void search()
{
	std::vector<int> v = { 1, 2, 3, 4, 5 };

	// 条件一致する最初の要素を検索する
	decltype(v)::iterator it = std::find_if(
		v.begin(),
		v.end(),
		[](int x) { return x % 2 == 0; }
	);

	// 見つかった
	if (it != v.end()) {
		int found_value = *it;
		std::cout << found_value << std::endl;
	}
}
int main()
{
	copy_capture_example();
	reference_capture_example();
	std::cout << foo()(2); // キャプチャされたローカル変数nは寿命を迎えているため不定値を得る
	 // 処理が完了したときに呼ばれる関数を指定する(フックパターン)
	proc([] {
		std::cout << "complete" << std::endl;
		});
	search();
	std::cin.get();
}
#include <iostream>


template<typename T, typename U>
auto add(const T& lhs, const U& rhs)
-> decltype(lhs + rhs)                // add の戻り値型は lhs + rhs の式の型
{
	return lhs + rhs;
}
template<typename T>
auto d(T&& x)
{
	return x + x;
}
struct S {
	int i;
	S(int i) : i(i) {}
	operator decltype(i)(){ return i; }        // int 型への変換演算子　型変換演算子のオーバーロード
};

struct U {
	static constexpr int g = 20; //https://qiita.com/saltheads/items/dd65935878a0901fe9e7
};

struct P {
	P() { std::cout << "P()\n"; }
	~P() { std::cout << "~P()\n"; }
};

S f();

constexpr int U::g;

int main()
{
	{
		auto r = add(1, 2.0F);                // r の型は float 型
		std::cout << std::fixed << r << '\n';	//std::fixed:小数点表示指示
	}
	{
		int i = 10;
		decltype(i) j = i;                    // j は int 型
		decltype((i)) k = i;                  // k は int& 型（i は lvalue で丸括弧が付いているので）

		i = 42;
		std::cout << j << ", " << k << '\n';
	}
	{
		S s(42);
		std::cout << s << '\n';
	}
	{
		S s(42);
		auto mp = &decltype(s)::i;             // mp の型は int S::* 型（S の int 型のメンバへのポインタ）
		std::cout << s.*mp << '\n';
	}
	{
		auto l = [](auto&& x) {
			std::cout << "start\n";
			auto ret = d(std::forward<decltype(x)>(x));     // x の型のままで転送
			std::cout << "end\n";
			return ret;
		};
		std::cout << l(U::g) << '\n';
	}
	{
		P h();
		std::cout << "allocate\n";
		void* p = ::operator new(sizeof(h()));

		std::cout << "construct\n";
		::new(p) decltype(h());                       // 配置 new で p の指すメモリに S 型のオブジェクトを構築

		std::cout << "destruct\n";
		auto sp = static_cast<decltype(h())*>(p);     // デストラクタ呼び出しのため、S 型へのポインタにキャスト
		sp->~decltype(h())();                         // 明示的デストラクタ呼び出しで sp の指すメモリの S 型オブジェクトを破棄

		std::cout << "deallocate\n";
		::operator delete(p);
	}
	std::cin.get();
}
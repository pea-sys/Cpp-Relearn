// forward : 関数テンプレートの引数を転送する（実際はキャスト処理するだけ)

//　コピーを抑止する場合に使用されることが多い？

//この関数に渡した引数は、多くのケースにおいて右辺値参照にキャストされ、
//ムーブされることに注意すること。同じ引数に対して二回以上関数を呼び出しを行いたい場合は、
//std::forward()は使ってはならない：
#include <iostream>
#include <memory>
#include <utility>

/*
shared_ptrは、指定されたリソースへの所有権(ownership)を共有(share)するスマートポインタである。
複数のshared_ptrオブジェクトが同じリソースを共有し、所有者が0人、
つまりどのshared_ptrオブジェクトからもリソースが参照されなくなると、リソースが自動的に解放される。
*/
template <class T, class A1, class A2>
std::shared_ptr<T> factory(A1&& a1, A2&& a2) {
	return std::shared_ptr<T>(new T(std::forward<A1>(a1), std::forward<A2>(a2)));
}

struct A {
	A(int&, const double&) {}
};

int main()
{
	//std::shared_ptr<A> sp1 = factory<A>(2, 1.414);  // error: 2 will not bind to int&

	int i = 2;
	
	std::shared_ptr<A> sp2 = factory<A>(i, 1.414);  // OK
	std::cout << sp2 ;
	std::cin.get();
}

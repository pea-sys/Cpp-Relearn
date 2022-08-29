// コンパイル時の安全なキャストチェック
#include <iostream>
#include <string>

template <bool> struct CompileTimeChecker
{
	CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false> { };
#define STATIC_CHECK(expr, msg){\
	 class ERROR_msg {};\
	(void) sizeof((::CompileTimeChecker<(expr) != 0> (ERROR_msg())));\
}

struct A
{
	int a;
};
struct AB
{
	int a, b;
};
int main()
{
	std::cout << std::endl;
	STATIC_CHECK(sizeof(AB) >= sizeof(A), Destination_Too_Narrow);
	//コンパイルエラー
	//STATIC_CHECK(sizeof(A) >= sizeof(AB), Destination_Too_Narrow);
}
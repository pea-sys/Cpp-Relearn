// function template
#include <iostream>
using namespace std;
/*
テンプレートは、ジェネリック型で動作できる特別な関数です。これにより、タイプごとにコード全体を繰り返すことなく、
機能を複数のタイプまたはクラスに適合させることができる関数テンプレートを作成できます。
*/

//関数テンプレート：記法はどちらでも良い
//template <class identifier> function_declaration;
//template <typename identifier> function_declaration;

// コンセプトでテンプレートに型制約をつけられる(C#のwhere)
// 型が算術型であることを要求する
//https://cpprefjp.github.io/reference/type_traits/is_arithmetic.html
template<class NumericType>
concept Numeric = (is_arithmetic<NumericType>::value == true);

template <Numeric T>
requires equality_comparable<T> //標準的なコンセプトは標準ライブラリで提供されているので、まずはそこから探すのが良い
T GetMax(T a, T b) {
	T result;
	result = (a > b) ? a : b;
	return (result);
}

//クラステンプレート：
template <class T, int N>
class mysequence {
	T memblock[N];
public:
	void setmember(int x, T value);
	T getmember(int x);
};

template <class T, int N>
void mysequence<T, N>::setmember(int x, T value) {
	memblock[x] = value;
}

template <class T, int N>
T mysequence<T, N>::getmember(int x) {
	return memblock[x];
}


int main() {
	//テンプレート関数の呼び出し
	int i = 5, j = 6, k;
	long l = 10, m = 5, n;
	double p = 2.2, q = 0.8, r;
	char x = 'a', y = 'b', z;
	k = GetMax<int>(i, j);
	n = GetMax<long>(l, m);
	r = GetMax<double>(p, q);
	z = GetMax<char>(x, y);
	cout << k << endl;
	cout << n << endl;
	cout << r << endl;
	cout << z << endl;

	//テンプレートクラスの呼び出し
	mysequence <int, 5> myints;
	mysequence <double, 5> myfloats;
	myints.setmember(0, 100);
	myfloats.setmember(3, 3.1416);
	cout << myints.getmember(0) << '\n';
	cout << myfloats.getmember(3) << '\n';
	cin.get();
	return 0;
}
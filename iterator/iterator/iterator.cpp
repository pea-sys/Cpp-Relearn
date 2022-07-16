#include <iostream>
#include <concepts>
#include <iterator>
#include <memory>
#include <vector>
#include <forward_list>
#include <list>
#include <deque>

#pragma region input_iterator 

// 入力イテレーター：イテレーターの比較、イテレーターの参照、イテレーターのインクリメント
// イテレーターの中で最も制約が少ないので弱いイテレーターと表現されていることもある
template<std::input_iterator I>
void f1(const char* name) {
	std::cout << name << " is input_iterator" << std::endl;
}

template<typename I>
void f1(const char* name) {
	std::cout << name << " is not input_iterator" << std::endl;
}

//input_iteratorと判定できる最低限の機能を備えたイテレーター
struct sample_input_iterator {
	// *thisの参照を返さなければならない（戻り値型は自身の参照型）
	friend auto operator++(sample_input_iterator&)->sample_input_iterator&;
	// 戻り値型はvoidでも可
	friend auto operator++(sample_input_iterator&, int)->sample_input_iterator;

	// const修飾されていなければならない
	// indirectly_readableコンセプトにおいて、iter_reference_tの結果がconst有無両方で一致することが求められる
	friend auto operator*(const sample_input_iterator&) -> int&;

	// 少なくともこの3つは必須
	using difference_type = int;
	using value_type = int;
	using iterator_concept = std::input_iterator_tag;
};

#pragma endregion

#pragma region output_iterator

// output_iteratorとなるイテレータは、operator* による書き込みと前置 / 後置インクリメントによる
// 進行が可能であるが、等値比較は必ずしも可能ではない。
template<std::output_iterator<int> I>
void f2(const char* name) {
	std::cout << name << " is output_iterator<int>" << std::endl;
}

template<typename I>
void f2(const char* name) {
	std::cout << name << " is not output_iterator<int>" << std::endl;
}

//
struct sample_output_iterator {
	friend auto operator++(sample_output_iterator&)->sample_output_iterator&;
	friend auto operator++(sample_output_iterator&, int)->sample_output_iterator;

	friend auto operator*(const sample_output_iterator&) -> int&;

	using difference_type = int;
};
#pragma endregion

#pragma region forward_iterator
//forward_iteratorとなるイテレータは、入力イテレータであり、
//コピーと等値比較が可能になり、マルチパス保証があるイテレータである
template<std::forward_iterator I>
void f3(const char* name) {
	std::cout << name << " is forward_iterator" << std::endl;
}

template<typename I>
void f3(const char* name) {
	std::cout << name << " is not forward_iterator" << std::endl;
}


struct sample_forward_iterator {
	friend auto operator++(sample_forward_iterator&)->sample_forward_iterator&;
	friend auto operator++(sample_forward_iterator&, int)->sample_forward_iterator;

	friend auto operator*(const sample_forward_iterator&) -> int&;

	friend bool operator==(const sample_forward_iterator&, const sample_forward_iterator&);

	using difference_type = int;
	using value_type = int;
	using iterator_concept = std::forward_iterator_tag;
};

#pragma endregion

#pragma region bidirectional_iterator
//bidirectional_iteratorとなるイテレータは、前方向にも後方にも進むことのできる双方向イテレータである。
template<std::bidirectional_iterator I>
void f4(const char* name) {
	std::cout << name << " is bidirectional_iterator" << std::endl;
}

template<typename I>
void f4(const char* name) {
	std::cout << name << " is not bidirectional_iterator" << std::endl;
}

struct sample_bidirectional_iterator {
	friend auto operator++(sample_bidirectional_iterator&)->sample_bidirectional_iterator&;
	friend auto operator++(sample_bidirectional_iterator&, int)->sample_bidirectional_iterator;

	friend auto operator--(sample_bidirectional_iterator&)->sample_bidirectional_iterator&;
	friend auto operator--(sample_bidirectional_iterator&, int)->sample_bidirectional_iterator;

	friend auto operator*(const sample_bidirectional_iterator&) -> int&;

	friend bool operator==(const sample_bidirectional_iterator&, const sample_bidirectional_iterator&);

	using difference_type = int;
	using value_type = int;
	using iterator_concept = std::bidirectional_iterator_tag;
};

#pragma endregion

#pragma region random_access_iterator

//random_access_iteratorとなるイテレータは、双方向イテレータであり、+= + -= -による定数時間での進行と、
// -による定数時間での距離の計算が可能である。
template<std::random_access_iterator I>
void f5(const char* name) {
	std::cout << name << " is random_access_iterator" << std::endl;
}

template<typename I>
void f5(const char* name) {
	std::cout << name << " is not random_access_iterator" << std::endl;
}

struct sample_random_access_iterator {
	friend auto operator++(sample_random_access_iterator&)->sample_random_access_iterator&;
	friend auto operator++(sample_random_access_iterator&, int)->sample_random_access_iterator;

	friend auto operator--(sample_random_access_iterator&)->sample_random_access_iterator&;
	friend auto operator--(sample_random_access_iterator&, int)->sample_random_access_iterator;

	friend auto operator+(const sample_random_access_iterator&, int)->sample_random_access_iterator;
	friend auto operator+(int, const sample_random_access_iterator&)->sample_random_access_iterator;
	friend auto operator+=(sample_random_access_iterator&, int)->sample_random_access_iterator&;
	friend auto operator-(const sample_random_access_iterator&, int)->sample_random_access_iterator;
	friend auto operator-=(sample_random_access_iterator&, int)->sample_random_access_iterator&;

	friend auto operator-(const sample_random_access_iterator&, const sample_random_access_iterator&) -> int;

	friend auto operator*(const sample_random_access_iterator&) -> int&;
	auto operator[](int) const -> int&;

	friend std::strong_ordering operator<=>(const sample_random_access_iterator&, const sample_random_access_iterator&);
	friend bool operator==(const sample_random_access_iterator&, const sample_random_access_iterator&);

	using difference_type = int;
	using value_type = int;
	using iterator_category = std::random_access_iterator_tag;
};

#pragma endregion

#pragma region contiguous_iterator

//contiguous_iteratorとなるイテレータは、ランダムアクセスイテレータであり、
//参照する要素列がメモリ上で連続していることが保証される。
template<std::contiguous_iterator I>
void f6(const char* name) {
	std::cout << name << " is contiguous_iterator" << std::endl;
}

template<typename I>
void f6(const char* name) {
	std::cout << name << " is not contiguous_iterator" << std::endl;
}

#pragma endregion

/*std::iteratorは公式非推奨なので使用しないこと*/
int main() {
	f1<int*>("int*");
	f1<const int*>("const int*");
	f1<std::vector<int>::iterator>("std::vector<int>::iterator");
	f1<std::istream_iterator<double>>("std::istream_iterator<double>");
	f1<sample_input_iterator>("sample_input_iterator");

	std::cout << "\n";
	f1<int* const>("int* const");
	f1<std::ostream_iterator<double>>("std::ostream_iterator<double>");




	f2<int*>("int*");
	f2<std::vector<int>::iterator>("std::vector<int>::iterator");
	f2<std::ostream_iterator<int>>("std::ostream_iterator<int>");
	f2<sample_output_iterator>("sample_output_iterator");

	std::cout << "\n";
	f2<const int*>("const int*");
	f2<std::istream_iterator<int>>("std::istream_iterator<int>");
	f2<int* const>("int* const");



	f3<int*>("int*");
	f3<const int*>("const int*");
	f3<std::forward_list<int>::iterator>("std::forward_list<int>::iterator");
	f3<std::list<int>::iterator>("std::list<int>::iterator");
	f3<std::vector<int>::iterator>("std::vector<int>::iterator");
	f3<sample_forward_iterator>("sample_forward_iterator");

	std::cout << "\n";
	f3<int* const>("int* const");
	f3<std::istream_iterator<double>>("std::istream_iterator<double>");
	f3<std::ostream_iterator<double>>("std::ostream_iterator<double>");



	f4<int*>("int*");
	f4<const int*>("const int*");
	f4<std::list<int>::iterator>("std::list<int>::iterator");
	f4<std::vector<int>::iterator>("std::vector<int>::iterator");
	f4<sample_bidirectional_iterator>("sample_bidirectional_iterator");

	std::cout << "\n";
	f4<int* const>("int* const");
	f4<std::forward_list<int>::iterator>("std::forward_list<int>::iterator");
	f4<std::istream_iterator<double>>("std::istream_iterator<double>");
	f4<std::ostream_iterator<double>>("std::ostream_iterator<double>");
	

	f5<int*>("int*");
	f5<const int*>("const int*");
	f5<std::vector<int>::iterator>("std::vector<int>::iterator");
	f5<sample_random_access_iterator>("sample_random_access_iterator");

	std::cout << "\n";
	f5<int* const>("int* const");
	f5<std::forward_list<int>::iterator>("std::forward_list<int>::iterator");
	f5<std::list<int>::iterator>("std::list<int>::iterator");
	f5<std::istream_iterator<double>>("std::istream_iterator<double>");
	f5<std::ostream_iterator<double>>("std::ostream_iterator<double>");
	

	f6<int*>("int*");
	f6<const int*>("const int*");
	f6<std::vector<int>::iterator>("std::vector<int>::iterator");

	std::cout << "\n";
	f6<int* const>("int* const");
	f6<std::forward_list<int>::iterator>("std::forward_list<int>::iterator");
	f6<std::list<int>::iterator>("std::list<int>::iterator");
	f6<std::deque<int>::iterator>("std::deque<int>::iterator");
	f6<std::istream_iterator<double>>("std::istream_iterator<double>");
	f6<std::ostream_iterator<double>>("std::ostream_iterator<double>");
	std::cin.get();
}
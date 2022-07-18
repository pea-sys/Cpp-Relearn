#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
template<std::forward_iterator I>
void is_forward_iterator(const char* name) {
    std::cout << name << " is forward_iterator" << std::endl;
}

template<typename I>
void is_forward_iterator(const char* name) {
    std::cout << name << " is not forward_iterator" << std::endl;
}

//// mismatch の結果で得られた pair に対する情報を出力する
//template <class Range1, class Range2, class Pair>
//void print_mismatch_value(const Range1& r1, const Range2& r2, const Pair& p) {
//    std::cout << "mismatch index: " << std::distance(std::begin(r1), p.first) << std::endl;
//    std::cout << "mismatch value: (" << (std::end(r1) == p.first ? "end" : std::to_string(*p.first)) << ","
//        << (std::end(r2) == p.second ? "end" : std::to_string(*p.second)) << ")"
//        << std::endl;
//}

int main()
{
    bool result;
    std::vector<int> v = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1 };
    //アルゴリズ関数を呼ぶ場合、されぞれの関数に適合しているイテレータをパラメータとして呼ぶ必要がある
    is_forward_iterator<decltype(v)*>("vector<int>");

#pragma region シーケンス変更なし

    //範囲の全ての要素が条件を満たすかを判定する
    result = std::all_of(v.begin(), v.end(), [](int x) { return x < 5; });
    std::cout << "all_of : " << result << std::endl;
    result = std::all_of(v.begin(), v.end(), [](int x) { return x > -1; });
    std::cout << "all_of : " << result << std::endl;
    //範囲のいずれかの要素が条件を満たすかを判定する
    result = std::any_of(v.begin(), v.end(), [](int x) { return x < -1; });
    std::cout << "any_of : " << result << std::endl;
    result = std::any_of(v.begin(), v.end(), [](int x) { return x > -1; });
    std::cout << "any_of : " << result << std::endl;
    // 範囲の全ての要素が条件を満たさないかを判定する
    result = std::none_of(v.begin(), v.end(), [](int x) { return x > 9; });
    std::cout << "none_of : " << result << std::endl;
    result = std::none_of(v.begin(), v.end(), [](int x) { return x < 0; });
    std::cout << "none_of : " << result << std::endl;
    // 範囲の全ての要素に、指定された関数を適用する
    std::cout << "for_each : ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x + 1 << " "; });
    std::cout << std::endl;
    // 範囲の先頭N個の要素に、指定された関数を適用する
    std::cout << "for_each_n : ";
    std::for_each_n(v.begin(), 4, [](int x) { std::cout << x * 2 << " "; });
    std::cout << std::endl;
    // 指定された値を検索する
    auto find_result = std::find(v.begin(), v.end(), 1);
    if (find_result == v.end()) {
        std::cout << "find" << std::endl;
    }
    else {
        std::cout << "find: " << *find_result << std::endl;
    }
    // 範囲の中から、指定された条件を満たす最初の要素を検索する。
    find_result = std::find_if(v.begin(), v.end(), [](int x) { return x > 9; });
    if (find_result == v.end()) {
        std::cout << "find_if" << std::endl;
    }
    else {
        std::cout << "find_if: " << *find_result << std::endl;
    }
    // 範囲の中から、指定された条件を満たさない最初の要素を検索する。
    find_result = std::find_if_not(v.begin(), v.end(), [](int x) { return x > 9; });
    if (find_result == v.end()) {
        std::cout << "find_if_not" << std::endl;
    }
    else {
        std::cout << "find_if_not: " << *find_result << std::endl;
    }
    // 範囲の中から、特定のサブシーケンスを検索する。
    std::vector<int> w = { 6, 5, 4 };
    // 6, 5, 4に一致するインデックスを探す
    auto it = std::find_end(v.begin(), v.end(), w.begin(), w.end());
    if (it == v.end()) {
        std::cout << "find_end" << std::endl;
    }
    else {
        std::cout << "find_end: index =" << std::distance(v.begin(), it) << std::endl;
    }
    // 6, 5, 4 のどれかと一致する最初の要素を返す
    it = std::find_first_of(v.begin(), v.end(), w.begin(), w.end());
    if (it == v.end()) {
        std::cout << ":find_first_of" << std::endl;
    }
    else {
        std::cout << ":find_first_of: index =" << std::distance(v.begin(), it) << ", value =" << *it << std::endl;
    }
    // 隣接する要素で条件を満たしている最初の要素を検索する
    it = std::adjacent_find(v.begin(), v.end());
    if (it == v.end()) {
        std::cout << ":adjacent_find" << std::endl;
    }
    else {
        std::cout << ":adjacent_find: index =" << std::distance(v.begin(), it) << std::endl;
        std::cout << std::boolalpha << "*it = *(it+1): " << (*it == *(it + 1)) << std::endl;
    }
    // 値が 2 の要素がいくつあるかを数える
    std::cout << "count of 2: " << std::count(v.begin(), v.end(), 2) << std::endl;

    // 値が 1 または 3 の要素がいくつあるかを数える
    auto count = std::count_if(v.begin(), v.end(), [](int x) { return x == 1 || x == 3; });
    std::cout << "count of 1 or 3: " << count << std::endl;

    //auto pair = std::mismatch(v.begin(), v.end(), w.begin());
    //print_mismatch_value(v, w, pair);

    // 2つの範囲を等値比較する
    std::vector<int> v2 = { 9, 8, 9, 8, 5, 6, 5, 4, 3, 1, 0 };
    // x±1 の誤差を許すようにする
    result = std::equal(std::begin(v), std::end(v), std::begin(v2), std::end(v2),
        [](int x, int y) { return x - 1 <= y && y <= x + 1; });
    std::cout << "equal : " << std::boolalpha << result << std::endl;


    // あるシーケンスの中から、特定のサブシーケンスを探す
    // 連続している最初のシーケンスを探す
    // find_endと似ているけど、返すシーケンスが最初と最後で異なる
    it = std::search(v.begin(), v.end(), w.begin(), w.end());
    // v[0] の位置を指すイテレータが見つかる。
    if (it == v.end()) {
        std::cout << "search" << std::endl;
    }
    else {
        std::cout << "search: index =" << std::distance(v.begin(), it) << std::endl;
    }

    // 3 が 2 つ連続している最初のシーケンスを探す
    auto it1 = std::search_n(v.cbegin(), v.cend(), 2, 2);
    // v[5] の位置を指すイテレータが見つかる。
    if (it1 == v.cend()) {
        std::cout << "search_n" << std::endl;
    }
    else {
        std::cout << "search_n: index =" << std::distance(v.cbegin(), it1) << std::endl;
    }

#pragma endregion
    
    std::cin.get();
}

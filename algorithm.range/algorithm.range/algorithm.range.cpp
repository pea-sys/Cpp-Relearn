/*
C++20ではアルゴリズム関数の新しいバージョンがstd::ranges名前空間に追加された。従来の関数と比べて以下の点が異なる：

* テンプレート引数がコンセプトによって制約される
* イテレータの組に加えて、範囲(Range)も直接渡せる
* 射影(Projection)をサポートする
* ADLで発見されない
* 新しいアルゴリズム関数には範囲(Range)を直接渡すことができる
* 
* ※一部、C++23の関数も含む
*/
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <ranges>
#include <random>
#include <numeric>
#include <set>
#include <cassert>

void f(int& x)
{
    x *= 2;
}
struct P {
    int first; char second;
    friend std::ostream& operator<< (std::ostream& os, const P& p) {
        return os << '{' << p.first << ",'" << p.second << "'}";
    }
};
struct X {
    int id;
    std::string name;
};
auto print = [](std::string_view name, auto const& v) {
    std::cout << name << ": ";
    for (auto n = v.size(); const auto & e: v) {
        std::cout << e << (--n ? ", " : "\n");
    }
};
template <class Range1, class Range2, class I1, class I2>
void print_mismatch_value(const Range1& r1, const Range2& r2, const I1& i1, const I2& i2) {
    std::cout << "mismatch index: " << std::ranges::distance(std::begin(r1), i1) << std::endl;
    std::cout << "mismatch value: (" << (std::ranges::end(r1) == i1 ? "end" : std::to_string(*i1)) << ","
        << (std::ranges::end(r2) == i2 ? "end" : std::to_string(*i2)) << ")"
        << std::endl;
}
template <class X, class Y>
void compare_test(const X& x, const Y& y)
{
    if (std::ranges::lexicographical_compare(x, y)) {
        std::cout << "x less than y" << std::endl;
    }
    else {
        std::cout << "x not less than y" << std::endl;
    }

    // 比較演算のカスタマイズバージョン
    if (std::ranges::lexicographical_compare(x, y, std::ranges::greater())) {
        std::cout << "x less than y" << std::endl;
    }
    else {
        std::cout << "x not less than y" << std::endl;
    }
}
bool isOdd(int x) { return x % 2 != 0; }
void print_vector(const std::vector<int>& v)
{
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}
void print_unique_ptr(const std::unique_ptr<int>& v) {
    if (v) std::cout << *v << std::endl;
    else   std::cout << "(null)" << std::endl;
}
int main()
{
#pragma region シーケンスを変更しない操作


    //ranges::all_of:範囲の全ての要素が条件を満たすかを判定する。
    {
        std::cout << "range::all_of" << std::endl;
        constexpr std::array v = { 3, 1, 4 };

        std::cout << std::boolalpha;

        // 全ての要素が 5 より小さいか
        constexpr bool result1 = std::ranges::all_of(v, [](int x) { return x < 5; });
        std::cout << result1 << std::endl;

        // 全ての要素が 1 であるか
        constexpr bool result2 = std::ranges::all_of(v, [](int x) { return x == 1; });
        std::cout << result2 << std::endl;
    }
    //ranges::any_of:範囲のいずれかの要素が条件を満たすかを判定する
    {
        std::cout << "ranges::any_of" << std::endl;
        constexpr std::array v = { 3, 1, 4 };

        std::cout << std::boolalpha;

        // 5 以上の要素が存在するかどうか
        constexpr bool result1 = std::ranges::any_of(v, [](int x) { return x >= 5; });
        std::cout << result1 << std::endl;

        // 1 の要素が存在するかどうか
        constexpr bool result2 = std::ranges::any_of(v, [](int x) { return x == 1; });
        std::cout << result2 << std::endl;
    }
    //ranges::none_of:範囲の全ての要素が条件を満たさないかを判定する
    {
        std::cout << "ranges::none_of" << std::endl;
        constexpr std::array v = { 3, 1, 4 };

        std::cout << std::boolalpha;

        // 全ての要素が 3 以上であるか
        constexpr bool result1 = std::ranges::none_of(v, [](int x) { return x < 3; });
        std::cout << result1 << std::endl;

        // 全ての要素が 0 以外であるか
        constexpr bool result2 = std::ranges::none_of(v, [](int x) { return x == 0; });
        std::cout << result2 << std::endl;
    }
    //ranges::for_each:範囲の全ての要素に、指定された関数を適用する
    {
        std::cout << "ranges::for_each" << std::endl;
        constexpr std::array v = { 3, 1, 4 };

        // vの全ての要素にラムダ式を適用する
        std::ranges::for_each(v, [](int x) { std::cout << x << std::endl; });
    }
    //ranges;:for_each_n:範囲の先頭N個の要素に、指定された関数を適用する。
    {
        std::cout << "ranges::for_each_n" << std::endl;
        std::array a{ 1, 2, 3, 4, 5 };
        print("a", a);
        // Negate first three numbers:
        std::ranges::for_each_n(a.begin(), 3, [](auto& n) { n *= -1; });
        print("a", a);

        std::array s{ P{1,'a'}, P{2, 'b'}, P{3, 'c'}, P{4, 'd'} };
        print("s", s);
        // Negate data members 'pair::first' using projection:
        std::ranges::for_each_n(s.begin(), 2, [](auto& x) { x *= -1; }, &P::first);
        print("s", s);
        // Capitalize data members 'pair::second' using projection:
        std::ranges::for_each_n(s.begin(), 3, [](auto& c) { c -= 'a' - 'A'; }, &P::second);
        print("s", s);
    }
    //ranges::find:指定された値を検索する
    {
        std::cout << "ranges::find" << std::endl;
        constexpr std::array v = { 3, 1, 4 };
        constexpr auto result = std::ranges::find(v, 1);
        if (result == v.end()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: " << *result << std::endl;
        }
    }
    //ranges::find_if:範囲の中から、指定された条件を満たす最初の要素を検索する
    {
        std::cout << "ranges::find_if" << std::endl;
        std::vector<int> v = { 3, 1, 4 };
        // 3ではない最初の要素を検索する
        auto result = std::ranges::find_if(v, [](int x) { return x != 3; });
        if (result == v.end()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: " << *result << std::endl;
        }
    }
    //ranges::find_if_not:範囲の中から、指定された条件を満たさない最初の要素を検索する
    {
        std::cout << "ranges::find_if_not" << std::endl;
        std::vector<int> v = { 3, 1, 4 };
        // 3ではない最初の要素を検索する
        auto result = std::ranges::find_if_not(v, [](int x) { return x == 3; });
        if (result == v.end()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: " << *result << std::endl;
        }
    }
    //ranges::find_end;範囲の中から、特定のサブシーケンスを検索する
    {
        std::cout << "ranges::find_end" << std::endl;
        using namespace std::literals;
        constexpr auto secret{ "password password word..."sv };
        constexpr auto wanted{ "password"sv };

        constexpr auto found1 = std::ranges::find_end(
            secret.cbegin(), secret.cend(), wanted.cbegin(), wanted.cend());
        print(secret, found1);

        constexpr auto found2 = std::ranges::find_end(secret, "word"sv);
        print(secret, found2);

        const auto found3 = std::ranges::find_end(secret, "ORD"sv,
            [](const char x, const char y) { // uses a binary predicate
                return std::tolower(x) == std::tolower(y);
            });
        print(secret, found3);

        const auto found4 = std::ranges::find_end(secret, "SWORD"sv, {}, {},
            [](char c) { return std::tolower(c); }); // projects the 2nd range
        print(secret, found4);

        static_assert(std::ranges::find_end(secret, "PASS"sv).empty()); // => not found
    }
    //:ranges::find_first_of:ある集合の1つとマッチする最初の要素を検索する
    {
        //constexpr static auto haystack = { 1, 2, 3, 4 };
        //constexpr static auto needles = { 0, 3, 4, 3 };

        //constexpr auto found1 = std::ranges::find_first_of(haystack.begin(), haystack.end(),
        //    needles.begin(), needles.end());
        //static_assert(std::distance(haystack.begin(), found1) == 2);

    //    constexpr auto found2 = std::ranges::find_first_of(haystack, needles);
    //    static_assert(std::distance(haystack.begin(), found2) == 2);


    //    constexpr static auto negatives = { -6, -3, -4, -3 };
    //    constexpr auto not_found = std::ranges::find_first_of(haystack, negatives);
    //    static_assert(not_found == haystack.end());

    //    constexpr auto found3 = std::ranges::find_first_of(haystack, negatives,
    //        [](int x, int y) { return x == -y; }); // uses a binary comparator
    //    static_assert(std::distance(haystack.begin(), found3) == 2);


    //    struct P { int x, y; };
    //    constexpr static auto p1 = { P{1, -1}, P{2, -2}, P{3, -3}, P{4, -4} };
    //    constexpr static auto p2 = { P{5, -5}, P{6, -3}, P{7, -5}, P{8, -3} };

    //    // Compare only P::y data members by projecting them:
    //    const auto found4 = rng::find_first_of(p1, p2, {}, &P::y, &P::y);
    //    std::cout << "First equivalent element {" << found4->x << ", " << found4->y
    //        << "} was found at position " << std::distance(p1.begin(), found4)
    //        << ".\n";
    }
    //ranges::adjacent_find:隣接する要素で条件を満たしている最初の要素を検索する
    {
        std::cout << "ranges::adjacent_find" << std::endl;
        std::vector<int> v = { 1,4,3,3,1,2,2 };

        // 同じ値が連続している最初の要素を検索する
        auto it = std::ranges::adjacent_find(v);
        if (it == v.end()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: index==" << std::ranges::distance(v.begin(), it) << std::endl;
            std::cout << std::boolalpha << "*it == *(it+1): " << (*it == *(it + 1)) << std::endl;
        }
    }
    //ranges::count:指定された値と等値な要素の数を数える。
    {
        std::cout << "ranges::count" << std::endl;
        constexpr std::array v = { 1,4,3,3,1,2,2,1 };

        // 値が 1 の要素がいくつあるかを数える
        std::cout << "count of 1: " << std::ranges::count(v, 1) << std::endl;
    }
    //ranges::count_if:条件を満たしている要素の数を数える
    {
        std::cout << "ranges::count_if" << std::endl;
        constexpr std::array v = { 1,4,3,3,1,2,2,1 };

        // 値が 1 または 3 の要素がいくつあるかを数える
        auto count = std::ranges::count_if(v, [](int x) { return x == 1 || x == 3; });
        std::cout << "count of 1 or 3: " << count << std::endl;
    }
    //std::ranges::mismatch:2つのシーケンスが一致していない場所を検索する
    {
        std::cout << "ranges::mismatch" << std::endl;
        const std::vector<int>   v = { 1,2,3,4,3,2 };
        const std::array<int, 6> v2 = { 1,2,4,3,2,1 };
        const std::list<int>     v3 = { 1,2,3,4,3, };

        // v と v2 で異なる場所を探す
        {
            auto [i1, i2] = std::ranges::mismatch(v, v2);
            print_mismatch_value(v, v2, i1, i2);
        }

        std::cout << std::endl;

        // v と v3 で異なる場所を探す。
        {
            auto [i1, i2] = std::ranges::mismatch(v3, v);
            print_mismatch_value(v3, v, i1, i2);
        }
    }
    //ranges::equal:2つの範囲を等値比較する
    {
        std::cout << "ranges::equal" << std::endl;
        std::vector<int>   v = { 1,2,3,4,3,2 };
        std::array<int, 6> v2 = { 1,2,3,4,2,1 };

        // コンテナの中身が同じかどうか調べる
        bool result = std::ranges::equal(v, v2);
        std::cout << std::boolalpha << result << std::endl;

        // x±1 の誤差を許すようにする
        bool result2 = std::ranges::equal(v, v2, [](int x, int y) { return x - 1 <= y && y <= x + 1; });
        std::cout << std::boolalpha << result2 << std::endl;
    }
    //ranges::search:あるシーケンスの中から、特定のサブシーケンスを探す
    {
        std::cout << "ranges::search" << std::endl;
        std::vector<int> v = { 1,2,1,2,3 };
        std::list<int> ls = { 1,2 };

        // 1,2 と連続している最初のシーケンスを探す
        std::ranges::subrange sr = std::ranges::search(v, ls);
        // v[0] の位置を指すイテレータが見つかる。
        if (sr.empty()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: index==" << std::distance(v.begin(), sr.begin()) << std::endl;
        }
    }
    //ranges::search_n:あるシーケンスの中から、指定の要素が連続するサブシーケンスを探す
    {
        std::cout << "ranges::search_n" << std::endl;
        std::vector<int> v = { 1,2,3,2,1,3,3,2,3,3,1 };

        // 3 が 2 つ連続している最初のシーケンスを探す
        std::ranges::subrange sr1 = std::ranges::search_n(v, 2, 3);
        // v[5] の位置を指すイテレータが見つかる。
        if (sr1.empty()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: index==" << std::distance(v.begin(), sr1.begin()) << std::endl;
        }

        // 3 未満が 2 つ連続している最初のシーケンスを探す
        auto sr2 = std::ranges::search_n(v, 2, 3, [](int x, int y) { return x < y; });
        // v[0] の位置を指すイテレータが見つかる。
        if (sr2.empty()) {
            std::cout << "not found" << std::endl;
        }
        else {
            std::cout << "found: index==" << std::distance(v.begin(), sr2.begin()) << std::endl;
        }
    }
    //ranges::starts_with:シーケンスの先頭が指定されたシーケンスと一致するかを調べる
    {
        std::cout << "ranges::starts_with" << std::endl;
        const std::vector v = { 1,2,3,4,5,6 };
        const std::vector v1 = { 1,2,3 };
        const std::vector v2 = { 2,3,4 };

        std::cout << std::ranges::starts_with(v, v1) << std::endl;
        std::cout << std::ranges::starts_with(v, v2) << std::endl;
    }
    //ranges::ends_with:シーケンスの末尾が指定されたシーケンスと一致するかを調べる
    {
        std::cout << "ranges::ends_with" << std::endl;
        const std::vector v = { 1,2,3,4,5,6 };
        const std::vector v1 = { 1,2,3 };
        const std::vector v2 = { 4,5,6 };

        std::cout << std::ranges::ends_with(v1, v) << std::endl;
        std::cout << std::ranges::ends_with(v, v1) << std::endl;
        std::cout << std::ranges::ends_with(v, v2) << std::endl;
    }

#pragma endregion


#pragma region シーケンスを変更する操作

    //std::ranges::copy:指定された範囲の要素をコピーする
    {
        std::cout << "std::ranges::copy" << std::endl;
        std::vector<int> v = { 3,1,2 };

        // v から v2 へ普通にコピーする
        std::vector<int> v2(v.size()); // ちゃんと確保しておくこと
        std::ranges::copy(v, v2.begin());

        // back_inserter を使って v3 へ設定。
        // back_inserter は要素をコピーするときに v3.push_back() するイテレータを作る関数。
        std::list<int> ls3;
        std::ranges::copy(v2, std::back_inserter(ls3));

        // ostream_iterator を使って出力。
        // ostream_iterator<int>(cout, ",") は要素をコピーするときに cout << x << "," としてくれるイテレータ。
        std::ranges::copy(ls3, std::ostream_iterator<int>(std::cout, ","));
    }
    //ranges::copy_n:指定された数の要素をコピーする
    {
        std::cout << "std::ranges::copy_n" << std::endl;
        std::vector<int> v = { 3, 1, 5, 2, 4 };
        std::ranges::copy_n(v.begin(), 5, std::ostream_iterator<int>(std::cout, ","));
    }
    //:ranges::copy_if:条件を満たす要素のみをコピーする
    {
        std::cout << "std::ranges::copy_if" << std::endl;
        std::vector<int> v1 = { 3, 1, 4 };
        std::vector<int> v2 = { 1, 5, 9 };
        std::vector<int> v3 = { 2, 6, 5 };
        std::vector<int> result(v1.size() + v2.size() + v3.size());

        // copy_if の戻り値を使って、複数のコンテナにある奇数を全て繋げる
        auto out = result.begin();
        out = std::ranges::copy_if(v1, out, isOdd).out;
        out = std::ranges::copy_if(v2, out, isOdd).out;
        out = std::ranges::copy_if(v3, out, isOdd).out;

        std::ranges::copy(result.begin(), out, std::ostream_iterator<int>(std::cout, ","));
    }
    //ranges::copy_backward:指定された範囲の要素を後ろからコピーする
    {
        std::cout << "ranges::copy_backward" << std::endl;
        std::list<int> ls = { 1,2,3,4,5 };
        // 1,2,3 の範囲を、3,4,5 の値のある範囲へコピーする
        std::ranges::copy_backward(std::ranges::begin(ls), std::next(std::ranges::begin(ls), 3), std::ranges::end(ls));

        // 以下のコードだと期待した結果にならないことを確認しよう
        // std::ranges::copy(std::ranges::begin(ls), std::ranges::next(std::ranges::begin(ls), 3), std::ranges::next(std::ranges::begin(ls), 2));

        std::ranges::copy(std::ranges::begin(ls), std::ranges::end(ls), std::ostream_iterator<int>(std::cout, ","));
    }
    //ranges::move:指定された範囲の要素をムーブする
    {
        std::cout << "ranges::move" << std::endl;
        std::vector<std::unique_ptr<int>> v;
        for (int i = 0; i < 5; i++) {
            v.emplace_back(new int(i));
        }

        std::vector<std::unique_ptr<int>> v2;
        // v のそれぞれの要素を v2 へムーブする
        std::ranges::move(v, std::back_inserter(v2));

        for (const auto& v : v2) {
            std::cout << *v << std::endl;
        }
    }
    //ranges::move_backward:指定された範囲の要素を後ろからムーブする
    {
        std::cout << "ranges::move_backward" << std::endl;
        std::vector<std::unique_ptr<int>> v;

        for (int i = 0; i < 5; i++) {
            v.emplace_back(new int(i));
        }

        // 0,1,2 の値がある範囲を、2,3,4 の値がある範囲へムーブする
        std::ranges::move_backward(v | std::views::take(3), v.end());

        // 以下のコードだと期待した結果にならないことを確認しよう。
        // 移動元の後方と移動先の前方で範囲が重なっている場合は、move_backwardを使わないといけない
        // std::ranges::move(v | std::views::take(3), v.begin() + 2);

        std::ranges::for_each(v, &print_unique_ptr);
    }
    //ranges::swap_ranges:指定された2つの範囲同士を swap する
    {
        std::cout << "ranges::swap_ranges" << std::endl;

        std::vector<int> v1 = { 3,1,2 };
        std::vector<int> v2 = { 5,2,4,1,3 };

        std::ranges::swap_ranges(v1, v2);

        std::cout << "v1: ";
        std::ranges::copy(v1, std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;

        std::cout << "v2: ";
        std::ranges::copy(v2, std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;
    }
    //ranges::transform:全ての要素に関数を適用する
    {
        std::cout << "ranges::transform" << std::endl;
        std::vector<int> v = { 3,1,4 };
        std::vector<std::string> result;

        // 2倍してから文字列に変換する
        std::ranges::transform(v, std::back_inserter(result), [](int x) { return std::to_string(x * 2); });

        for (const std::string& s : result) {
            std::cout << s << ",";
        }
        std::cout << std::endl;
    }
    //ranges::replace:指定された値と一致する要素を指定された値に置き換える
    {
        std::cout << "ranges::replace" << std::endl;
        std::vector<int> v = { 3,1,2,1,2 };

        // 1 の要素を全部 10 に置き換える
        std::ranges::replace(v, 1, 10);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::replace_if:条件を満たす要素を指定された値に置き換える
    {
        std::cout << "ranges::replace_if" << std::endl;
        std::vector<int> v = { 3,1,2,1,2 };

        // 奇数の要素を全部 10 に置き換える
        std::ranges::replace_if(v, [](int x) { return x % 2 != 0; }, 10);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::replace_copy:指定された値を一致する要素を指定された値に置き換え、その結果を出力の範囲へコピーする
    {
        std::cout << "ranges::replace_copy" << std::endl;
        std::vector<int> v = { 3,1,2,1,2 };

        // 1 の要素を全部 10 に置き換えたものを出力する
        std::ranges::replace_copy(v, std::ostream_iterator<int>(std::cout, ","), 1, 10);
        std::cout << std::endl;
    }
    //ranges::replace_copy_if:条件を満たす要素を指定された値に置き換え、その結果を出力の範囲へコピーする。
    {
        std::cout << "ranges::replace_copy_if" << std::endl;
        std::vector<int> v = { 3,1,2,1,2 };

        // 奇数の要素を全部 10 に置き換えたものを出力する
        std::ranges::replace_copy_if(v, std::ostream_iterator<int>(std::cout, ","), [](int x) { return x % 2 != 0; }, 10);
    }
    //ranges:: fill:指定された値で出力の範囲に書き込む
    {
        std::cout << "ranges::fill" << std::endl;
        std::vector<int> v(5);

        // v を 3 の値で埋める
        std::ranges::fill(v, 3);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::fill_n:指定された値で出力の範囲に n 個を書き込む
    {
        std::cout << "ranges::fill_n" << std::endl;
        // 3 を10回出力する
        std::ranges::fill_n(std::ostream_iterator<int>(std::cout, ","), 10, 3);
        std::cout << std::endl;
    }
    //ranges::generate:出力の範囲へ関数の結果を書き込む
    {
        std::cout << "ranges::generate" << std::endl;
        std::vector<int> v(10);

        // 2 の累乗の値を生成する
        int n = 1;
        std::ranges::generate(v, [&n]() { auto t = n; n *= 2; return t; });

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //generate_n:出力の範囲へ関数の結果を n 個書き込む
    {
        std::cout << "generate_n" << std::endl;
        // 2 の累乗の値を生成して出力する
        int n = 1;
        std::ranges::generate_n(std::ostream_iterator<int>(std::cout, ","), 10, [&n] { auto t = n; n *= 2; return t; });
        std::cout << std::endl;
    }
    //ranges::remove:指定された要素を取り除く
    {
        std::cout << "ranges::remove" << std::endl;
        std::vector<int> v = { 2,3,1,2,1 };

        auto result = std::ranges::remove(v, 1);

        // [v.begin(), result.begin()) の範囲に 1 を除去した結果が入っている
        for (int x : std::ranges::subrange{ v.begin(), result.begin() }) {
            std::cout << x << ",";
        }
        std::cout << std::endl;

        // remove を使ってもコンテナの要素数は変わらないことに注意しよう
        std::cout << "size before: " << v.size() << std::endl;

        // [v.begin(), result.begin()) の範囲に 1 を除去した結果が入っているので、
        // [result.begin(),v.end()) を erase することでサイズも変更することができる
        // （Erase-remove イディオム）
        v.erase(result.begin(), v.end());
        std::cout << "size after: " << v.size() << std::endl;
    }
    //ranges::remove_if:条件を満たす要素を除ける
    {
        std::cout << "ranges::remove_if" << std::endl;
        std::vector<int> v = { 2,3,1,2,1 };

        auto result = std::ranges::remove_if(v, [](int x) { return x % 2 != 0; });

        // [v.begin(), result.begin()) の範囲に奇数を除去した結果が入っている
        for (int x : std::ranges::subrange{ v.begin(), result.begin() }) {
            std::cout << x << ",";
        }
        std::cout << std::endl;

        // remove を使ってもコンテナの要素数は変わらないことに注意しよう
        std::cout << "size before: " << v.size() << std::endl;

        // [v.begin(), result.begin()) の範囲に奇数を除去した結果が入っているので、
        // [result.begin(),v.end()) を erase することでサイズも変更することができる
        // （Erase-remove イディオム）
        v.erase(result.begin(), v.end());
        std::cout << "size after: " << v.size() << std::endl;
    }
    //ranges::remove_copy:指定された要素を除け、その結果を出力の範囲へコピーする
    {
        std::cout << "ranges::remove_copy" << std::endl;
        std::vector<int> v = { 2,3,1,2,1 };

        // 1 を除去した結果を出力する
        std::ranges::remove_copy(v, std::ostream_iterator<int>(std::cout, ","), 1);
        std::cout << std::endl;
    }
    //remove_copy_if:条件を満たす要素を除け、その結果を出力の範囲へコピーする
    {
        std::cout << "remove_copy_if" << std::endl;
        std::vector<int> v = { 2,3,1,2,1 };

        // 奇数を除去した結果を出力する
        std::ranges::remove_copy_if(v, std::ostream_iterator<int>(std::cout, ","), [](int x) { return x % 2 != 0; });
        std::cout << std::endl;
    }
    //ranges::unique:重複した要素を除ける
    {
        std::cout << "ranges::unique" << std::endl;
        // 入力の配列がソート済みではない場合、
        // 隣り合った重複要素が取り除かれる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };

            auto result = std::ranges::unique(v);

            // [v.begin(), result.begin())の範囲に、重複を除いた結果が入っている。
            // 不要になった要素を削除
            v.erase(result.begin(), result.end());

            print("unsorted unique", v);
        }

        // 入力の配列がソート済みである場合、
        // 重複している全ての要素が取り除かれて一意になる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };

            std::sort(v.begin(), v.end());
            auto result = std::ranges::unique(v);

            // 不要になった要素を削除
            v.erase(result.begin(), result.end());

            print("sorted unique", v);
        }
    }
    //ranges::unique_copy:隣り合った重複要素を取り除き、その結果を出力の範囲へコピーする
    {
        std::cout << "ranges::unique_copy" << std::endl;
        // 入力の配列がソート済みではない場合、
        // 隣り合った重複要素が取り除かれる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };
            std::vector<int> uniqued;

            // 重複を除いた要素がuniquedに追加されていく
            std::ranges::unique_copy(v, std::back_inserter(uniqued));

            print("unsorted unique", uniqued);
        }

        // 入力の配列がソート済みである場合、
        // 重複している全ての要素が取り除かれて一意になる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };
            std::vector<int> uniqued;

            std::ranges::sort(v);
            std::ranges::unique_copy(v, std::back_inserter(uniqued));

            print("sorted unique", uniqued);
        }
    }
    //ranges::reverse:要素の並びを逆にする
    {
        std::cout << "ranges::reverse" << std::endl;
        std::string str = "reverse";

        std::ranges::reverse(str);
        std::cout << str << std::endl;
    }
    //ranges::reverse_copy:要素の並びを逆にし、その結果を出力の範囲へコピーする
    {
        std::cout << "ranges::reverse_copy" << std::endl;
        std::string str = "reverse";

        std::ranges::reverse_copy(str, std::ostream_iterator<char>(std::cout, ""));
        std::cout << std::endl;
    }
    //ranges::rotate:middleの要素が先頭、middle-1の要素が末尾となるように、[first,last)の要素の並びを回転させる
    {
        std::cout << "ranges::rotate" << std::endl;
        std::string str = "rotate";

        std::ranges::rotate(str, str.begin() + 2);
        std::cout << str << std::endl;
    }
    //ranges::rotate_copy:middleの要素が先頭、middle-1の要素が末尾となるように、[first,last)の要素の並びを回転させ、その結果を出力の範囲へコピーする
    {
        std::cout << "ranges::rotate_copy" << std::endl;
        std::string str = "rotate";
        std::string result;

        std::ranges::rotate_copy(str, str.begin() + 2, std::back_inserter(result));

        std::cout << result << std::endl;
    }
    //std::ranges::sample:範囲から指定された個数の要素をランダムに抽出する
    {
        std::cout << "ranges::sample" << std::endl;
        // 乱数生成器を用意する
        std::random_device seed_gen;
        std::mt19937 engine{ seed_gen() };

        // 文字列中から3文字をランダム抽出する
        {
            const std::string input = "abcdef";
            const int n = 3;

            std::string result;
            std::ranges::sample(input, std::back_inserter(result), n, engine);
            std::cout << result << std::endl;
        }

        // 配列から3要素をランダム抽出する
        {
            const std::vector<int> input = { 0, 1, 2, 3, 4, 5 };
            const int n = 3;

            std::vector<int> result;
            std::ranges::sample(input, std::back_inserter(result), n, engine);

            for (int x : result) {
                std::cout << x;
            }
            std::cout << std::endl;
        }
        //ranges::copy:[first,last) のそれぞれの要素を同じ確率で並び替える
        {
            std::cout << "ranges::copy" << std::endl;
            std::vector<int> v(10);
            std::iota(v.begin(), v.end(), 0); // 0～9 までの値を生成

            std::cout << "before: ";
            std::ranges::copy(v, std::ostream_iterator<int>(std::cout));
            std::cout << std::endl;

            // シャッフル
            std::random_device seed_gen;
            std::mt19937 engine(seed_gen());
            std::ranges::shuffle(v, engine);

            std::cout << " after: ";
            std::ranges::copy(v, std::ostream_iterator<int>(std::cout));
            std::cout << std::endl;
        }
        //ranges::is_partitioned::与えられた範囲が条件によって区分化されているか判定する:与えられた範囲が条件によって区分化されているか判定する
        //ranges::partition:与えられた範囲を条件によって区分化する
        {
            std::cout << "ranges::partitiony" << std::endl;
            std::vector<int> v = { 1, 2, 3, 4, 5 };

            auto pred = [](int x) { return x % 2 == 0; };

            // 偶数グループと奇数グループに分ける
            std::ranges::partition(v, pred);

            for (int x : v) {
                std::cout << x << std::endl;
            }

            // 偶数グループと奇数グループに分かれているか
            if (std::ranges::is_partitioned(v, pred)) {
                std::cout << "partitioned" << std::endl;
            }
            else {
                std::cout << "not partitioned" << std::endl;
            }
        }
    }
    //stable_partition:与えられた範囲を相対順序を保ちながら条件によって区分化する
    {
        std::cout << "stable_partition" << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        // 偶数グループと奇数グループに分ける
        std::ranges::stable_partition(v, [](int x) { return x % 2 == 0; });

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::partition_copy:与えられた範囲を条件によって 2 つの出力の範囲へ分けてコピーする
    {
        std::cout << "ranges::partition_copy" << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        // 偶数グループと奇数グループに分ける
        std::vector<int> evens;
        std::vector<int> odds;
        std::ranges::partition_copy(v, std::back_inserter(odds), std::back_inserter(evens), isOdd);

        print("v", v);
        print("evens", evens);
        print("odds", odds);
    }
    //ranges::partition_point:与えられた範囲から条件によって区分化されている位置を得る。
    {
        std::cout << "ranges::partition_point" << std::endl;
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        std::ranges::partition(v, isOdd);

        // 偶数グループと奇数グループに分かれた位置を得る
        auto it = std::ranges::partition_point(v, isOdd);

        print("v", v);
        std::cout << *it << std::endl;
    }
    //ranges::sort:範囲を並べ替える
    {
        std::cout << "ranges::sort" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        // 昇順に並べ替える
        std::ranges::sort(v);

        for (int i : v) {
            std::cout << i;
        }
        std::cout << std::endl;

        // 降順に並べ替える
        std::ranges::sort(v, std::ranges::greater());

        for (int i : v) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    //ranges::stable_sort:範囲を安定ソートで並べ替える
    {
        std::cout << "ranges::stable_sort" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        // 並べ替える
        std::ranges::stable_sort(v);

        for (int i : v) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    //ranges::partial_sort:範囲を部分的にソートし、先頭 N 個を順に並んだ状態にする。N は middle - first で決まる
    {
        std::cout << "ranges::partial_sort" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        // 先頭2要素を並んだ状態にする
        std::ranges::partial_sort(v, v.begin() + 2);

        for (int i : v) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    //ranges::partial_sort_copy:範囲を部分的にソートした結果を他の範囲にコピーする
    {
        std::cout << "ranges::partial_sort_copy" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        // vから小さい順に2要素取り出す
        std::vector<int> result(2);
        std::ranges::partial_sort_copy(v, result);

        for (int i : result) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    //ranges::is_sorted:与えられた範囲がソート済みか判定する
    {
        std::cout << "ranges::is_sorted" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        std::cout << std::boolalpha;
        std::cout << "before: is sorted? " << std::ranges::is_sorted(v) << std::endl;

        std::ranges::sort(v);

        std::cout << " after: is sorted? " << std::ranges::is_sorted(v) << std::endl;
    }
    //std::ranges::is_sorted_until:ソート済みか判定し、ソートされていない位置のイテレータを取得する
    {
        std::cout << "ranges::is_sorted_until" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5 };

        std::cout << std::boolalpha;
        std::cout << "before: is sorted? "
            << (std::ranges::is_sorted_until(v) == v.end()) << std::endl;

        std::sort(v.begin(), v.end());

        std::cout << " after: is sorted? "
            << (std::ranges::is_sorted_until(v) == v.end()) << std::endl;
    }
    //ranges::nth_element:基準となる要素よりも小さい要素が前に来るよう並べ替える
    {
        std::cout << "ranges::nth_element" << std::endl;
        std::vector<int> v = { 5, 10, 4, 7, 1, 9, 8, 6, 2 };

        // 4番目に小さい値より小さい値を前に集める
        std::ranges::nth_element(v, v.begin() + 3);

        for (int i : v) {
            std::cout << i << ",";
        }
    }
    //ranges::lower_bound:指定された要素以上の値が現れる最初の位置のイテレータを取得する
    {
        std::cout << "ranges::lower_bound" << std::endl;
        // この関数単体としての使い方
        {
            // lower_bound で 4 以上の要素の位置を検索する場合、
            // 4 より小さい物と 4 以上の物がその順に並んでいれば、
            // 必ずしもソートされている必要はない。
            std::vector<int> v = { 3, 1, 4, 6, 5 };

            // 4以上の要素を二分探索で検索
            auto it = std::ranges::lower_bound(v, 4);
            if (it != v.end()) {
                std::size_t pos = std::ranges::distance(v.begin(), it);
                std::cout << *it << " pos=" << pos << std::endl;
            }
        }

        // 基本的な用途
        // ソート済み範囲から、特定の値を二分探索で見つける
        {
            std::vector<int> v = { 3, 1, 4, 6, 5 };
            std::ranges::sort(v);

            // 二分探索で値4を検索
            auto it = std::ranges::lower_bound(v, 4);
            if (it != v.end() && *it == 4) { // lower_boundでは4"以上"の値が見つかるので、
                                             // 値4を見つけたいなら検索結果の値を比較する必要がある
                std::size_t pos = std::ranges::distance(v.begin(), it);
                std::cout << *it << " pos=" << pos << std::endl;
            }
        }

        // 要素の一部の値を比較して見つける
        {
            // 要素は複数のメンバ変数をもつ
            std::vector<X> v = {
              {1, "Carol"},
              {3, "Alice"},
              {4, "Bob"},
              {5, "Eve"},
              {6, "Dave"}
            };

            const std::string key = "Bob";

            // X::nameメンバ変数をキーにして、
            // X::name == "Bob"となる要素を二分探索で見つける
            auto it = std::ranges::lower_bound(v, key, {}, &X::name);

            if (it != v.end() && it->name == key) {
                std::size_t pos = std::ranges::distance(v.begin(), it);
                std::cout << "id=" << it->id
                    << " name=" << it->name
                    << " pos=" << pos
                    << std::endl;
            }
        }
    }
    //ranges::upper_bound:指定された要素より大きい値が現れる最初の位置のイテレータを取得する
    {
        std::cout << "ranges::upper_bound" << std::endl;
        // upper_bound で 3 より大きい要素の位置を検索する場合、
        // 3 以下の物と 3 より大きい物がその順に並んでいれば、
        // 必ずしもソートされている必要はない。
        std::vector<int> v = { 3, 1, 4, 6, 5 };
        // 3より大きい要素を二分探索で検索
        auto it = std::ranges::upper_bound(v, 3);
        std::cout << *it << std::endl;       
    }
    //ranges::equal_range:指定した値と等しい範囲を取得する
    {
        std::cout << "ranges::equal_range" << std::endl;
        std::vector<int> v = { 3, 1, 4, 2, 5, 3 };
        std::vector<int> v2 = { 1, 4, 2, 5 };

        std::ranges::sort(v);
        std::ranges::sort(v2);

        auto result = std::ranges::equal_range(v, 3);
        auto result2 = std::ranges::equal_range(v2, 3);

        std::cout << "size: " << result.size() << std::endl;
        for (int i : result) {
            std::cout << i << std::endl;
        }
        std::cout << std::endl;

        std::cout << "size: " << result2.size() << std::endl;
        for (int i : result2) {
            std::cout << i << std::endl;
        }
    }
    //ranges::binary_search:2つの連続したソート済み範囲をマージする
    {
        std::cout << "ranges::binary_search" << std::endl;
        // binary_search で 4 を検索する場合、
        // 4 より小さい物、4 と等しい物、4 より大きい物がその順に並んでいれば、
        // 必ずしもソートされている必要はない。
        std::vector<int> v = { 3, 1, 4, 6, 5 };

        if (std::ranges::binary_search(v, 4)) {
            std::cout << "found" << std::endl;
        }
        else {
            std::cout << "not found" << std::endl;
        }
    }
    //:ranges::merge::2つのソート済み範囲をマージして、出力イテレータへ出力する
    {
        std::cout << "ranges::sort" << std::endl;
        std::vector<int> a = { 3, 1, 4, 2 };
        std::vector<int> b = { 2, 5, 6, 4 };
        std::vector<int> result;

        std::ranges::sort(a);
        std::ranges::sort(b);

        // aとbをマージ
        std::ranges::merge(a, b, std::back_inserter(result));

        for (int x : result) {
            std::cout << x << ",";
        }
    }
    //ranges::inplace_merge:2つの連続したソート済み範囲をマージする
    {
        std::cout << "ranges::inplace_merge" << std::endl;
        std::vector<int> v = { 1,4,5,  2,3,6 };

        // ソートされた2つの範囲をマージ
        std::ranges::inplace_merge(v, v.begin() + 3);

        for (int x : v) {
            std::cout << x << std::endl;
        }
    }
    //ranges::set_union:2つのソート済み範囲の和集合を得る
    {
        std::cout << "ranges::set_union" << std::endl;
        std::list<int> a = { 1, 2, 3, 4 };
        std::multiset<int> b = { 4, 5, 6, 2 };
        std::vector<int> result;

        // aとbの和集合を作る
        std::ranges::set_union(a, b, std::inserter(result, std::ranges::end(result)));

        for (int x : result) {
            std::cout << x << std::endl;
        }
    }
    //ranges::set_intersection:2つのソート済み範囲の積集合を得る
    {
        std::cout << "ranges::set_intersection" << std::endl;
        std::multiset<int> a = { 1, 2, 3, 4 };
        std::multiset<int> b = { 4, 5, 6, 2 };
        std::multiset<int> result;

        // aとbの積集合を作る
        std::ranges::set_intersection(a, b, std::inserter(result, result.end()));

        for (int x : result) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //std::ranges::set_difference:2つのソート済み範囲の差集合を得る
    {
        std::cout << "std::ranges::set_difference" << std::endl;
        std::multiset<int> a = { 1, 2, 3, 4, 5, 6 };
        std::multiset<int> b = { 2, 3, 4 };
        std::multiset<int> result;

        // a - bの差集合を作る
        std::ranges::set_difference(a, b, std::inserter(result, result.end()));

        for (int x : result) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::set_symmetric_difference:2つのソート済み範囲の対称差集合、すなわち2つの範囲の共通しない要素からなる集合を得る
    {
        std::cout << "ranges::set_symmetric_difference" << std::endl;
        std::multiset<int> a = { 1, 2, 3, 4, 5, 6 };
        std::multiset<int> b = { 2, 3, 4, 7, 8 };
        std::multiset<int> result;

        // 重ならない要素からなる集合を作る
        std::ranges::set_symmetric_difference(a, b, std::inserter(result, result.end()));

        for (int x : result) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::includes:2つのソート済み範囲において、一方の範囲の要素がもう一方の範囲に全て含まれているかを判定する
    {
        std::cout << "ranges::includes" << std::endl;
        std::set<int> a = { 1, 2, 3, 4, 5, 6 };
        std::set<int> b = { 2, 4, 6 };
        std::set<int> c = { 2, 4, 7 };

        std::cout << std::boolalpha;

        std::cout << std::ranges::includes(a, b) << std::endl;
        std::cout << std::ranges::includes(a, c) << std::endl;
    }
    //ranges::make_heap:ヒープ化された範囲に要素を追加したヒープ範囲を得る
    {
        std::cout << "ranges::make_heap" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        std::ranges::make_heap(v);

        // 要素を追加してヒープ化
        v.push_back(2);
        std::ranges::push_heap(v);

        std::ranges::sort_heap(v);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::push_heap:ヒープ化された範囲に要素を追加します
    {
        std::cout << "ranges::push_heap" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        std::ranges::make_heap(v);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
        // 要素を追加してヒープ化
        v.push_back(2);
        std::ranges::push_heap(v);

        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges::pop_heap:ヒープ化された範囲の先頭と末尾を入れ替え、ヒープ範囲を作り直す
    {
        std::cout << "ranges::pop_heap" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        std::ranges::make_heap(v);
        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
        // 最後尾要素を削除してヒープ化
        std::ranges::pop_heap(v);
        for (int x : v) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
    }
    //ranges:: sort_heap:ヒープ化された範囲を並べ替える
    {
        std::cout << "ranges:: sort_heap" << std::endl;
        std::vector<int> v = { 3, 1, 4 };
        std::ranges::make_heap(v);

        for (int x : v) {
            std::cout << x << ",";
        }
        // ヒープ化された範囲をソート
        std::ranges::sort_heap(v);

        for (int x : v) {
            std::cout << x <<",";
        }
        std::cout << std::endl;
    }
    //std::ranges::is_heap_until:範囲がヒープ化されているか判定し、ヒープ化されていない最初の要素を指すイテレータを取得する
    {
        std::cout << "ranges::is_heap_until" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        std::cout << std::boolalpha;

        std::cout << "before: is heap? "
            << (std::ranges::is_heap_until(v) == v.end()) << std::endl;

        std::ranges::make_heap(v);
        std::cout << " after: is heap? "
            << (std::ranges::is_heap_until(v) == v.end()) << std::endl;
    }
    //ranges::is_heap:範囲がヒープ化されているか判定する
    {
        std::cout << "ranges::is_heap" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        std::cout << std::boolalpha;

        std::cout << "before: is heap? "
            << std::ranges::is_heap(v) << std::endl;

        std::ranges::make_heap(v);
        std::cout << " after: is heap? "
            << std::ranges::is_heap(v) << std::endl;
    }

    //std::ranges::min:同じ型の2つの値、もしくは範囲によるN個の値のうち、最小値を取得する。
    {
        std::cout << "ranges::min" << std::endl;
        constexpr int result1 = std::ranges::min(2, 3);
        static_assert(result1 == 2);

        constexpr int result2 = std::ranges::min(2, 3, std::ranges::greater());
        static_assert(result2 == 3);

        constexpr int result3 = std::ranges::min({ 1, 2, 3 });
        static_assert(result3 == 1);

        constexpr std::array<int, 3> a = { 1, 2, 3 };

        constexpr int result4 = std::ranges::min(a, std::ranges::greater());
        static_assert(result4 == 3);
        std::cout << std::endl;
    }
    //ranges::max:同じ型の2つの値、もしくは範囲によるN個の値のうち、最大値を取得する
    {
        std::cout << "ranges::max" << std::endl;
        constexpr int result1 = std::ranges::max(2, 3);
        static_assert(result1 == 3);

        constexpr int result2 = std::ranges::max(2, 3, std::ranges::greater());
        static_assert(result2 == 2);

        constexpr int result3 = std::ranges::max({ 1, 2, 3 });
        static_assert(result3 == 3);

        constexpr std::array<int, 3> a = { 1, 2, 3 };

        constexpr int result4 = std::ranges::max(a, std::ranges::greater());
        static_assert(result4 == 1);
        std::cout << std::endl;
    }
    //ranges::minmax:同じ型の2つの値、もしくは範囲によるN個の値のうち、最小値と最大値の組を取得する。
    {
        std::cout << "ranges::minmax" << std::endl;
        const auto result1 = std::ranges::minmax(2, 3);
        assert(result1.min == 2 && result1.max == 3);

        const auto result2 = std::ranges::minmax(2, 3, std::ranges::greater());
        assert(result2.min == 3 && result2.max == 2);

        constexpr auto result3 = std::ranges::minmax({ 1, 2, 3 });
        static_assert(result3.min == 1 && result3.max == 3);

        constexpr std::array<int, 3> a = { 1, 2, 3 };

        constexpr auto result4 = std::ranges::minmax(a, std::ranges::greater());
        static_assert(result4.min == 3 && result4.max == 1);
        std::cout << std::endl;
    }
    //ranges::min_element:[first, last)の範囲において、最小要素を指す最初のイテレータを取得する
    {
        std::cout << "ranges::min_element" << std::endl;
        std::vector<int> v1 = { 3, 1, 4 };

        auto v1_min_element = std::ranges::min_element(v1);
        assert(*v1_min_element == 1);


        std::vector<std::pair<int, int>> v2 = { {0, 3}, {1, 1}, {2, 4} };

        auto v2_min_element = std::ranges::min_element(v2, {}, &std::pair<int, int>::second);
        assert(v2_min_element->first == 1);
        assert(v2_min_element->second == 1);
        std::cout << std::endl;
    }
    //std::ranges::max_element:[first, last)の範囲において、最大要素を指す最初のイテレータを取得する
    {
        std::cout << "ranges::max_element" << std::endl;
        std::vector<int> v1 = { 3, 1, 4 };

        auto v1_max_element = std::ranges::max_element(v1);
        assert(*v1_max_element == 4);


        std::vector<std::pair<int, int>> v2 = { {0, 3}, {1, 1}, {2, 4} };

        auto v2_max_element = std::ranges::max_element(v2, {}, &std::pair<int, int>::second);
        assert(v2_max_element->first == 2);
        assert(v2_max_element->second == 4);
        std::cout << std::endl;
    }
    //ranges::minmax_element:[first, last) の範囲において、最小要素を指すイテレータと最大要素を指すイテレータの組を取得する
    {
        std::cout << "ranges::minmax_element" << std::endl;
        std::vector<int> v = { 3, 1, 4 };

        auto [min1, max1] = std::ranges::minmax_element(v);
        assert(*min1 == 1);
        assert(*max1 == 4);

        auto [min2, max2] = std::ranges::minmax_element(v, std::ranges::greater());
        assert(*min2 == 4);
        assert(*max2 == 1);
    }
    //ranges::clamp:値を範囲内に収める
    {
        std::cout << "ranges::clamp" << std::endl;
        for (int i = 0; i < 10; ++i) {
            // iの値を範囲[2, 7]に収める
            int result = std::ranges::clamp(i, 2, 7);
            std::cout << i << " : " << result << std::endl;
        }
    }
    //ranges::lexicographical_compare:[first1, last1)および[first2, last2)の2つの範囲を辞書式順序で比較する。
    {
        std::cout << "ranges::lexicographical_compare" << std::endl;
        // 同じ長さの文字列比較
        {
          std::string x = "heilo";
          std::string y = "hello";

          std::cout << "same length string compare:" << std::endl;
          compare_test(x, y);
        }
        std::cout << std::endl;

        // 異なる長さの文字列比較
        {
            std::string x = "hell";
            std::string y = "hello";

            std::cout << "not same length string compare:" << std::endl;
            compare_test(x, y);
        }
    }
    //ranges::next_permutation:与えられた時点の[first, last)の範囲を起点の順列として、辞書順によるその次の順列を生成する。
    {
        std::cout << "ranges::next_permutation" << std::endl;
        // 昇順にソート済みの入力
        std::vector<int> v = { 1, 2, 3 };

        do {
            print_vector(v);
        } while (std::ranges::next_permutation(v).found);
    }
    //ranges::prev_permutation:与えられた時点の[first, last)の範囲を起点の順列として、辞書順によるその前の順列を生成する
    {
        std::cout << "ranges::prev_permutation" << std::endl;
        // 降順にソート済みの入力
        std::vector<int> v = { 3, 2, 1 };

        do {
            print_vector(v);
        } while (std::ranges::prev_permutation(v).found);
        std::cout << std::endl;
    }
    //:ranges::is_permutation:範囲 [first2, last2) を並べ替えたものが、[first1, last1) の範囲と一致するか判定する
    {
        std::cout << ":ranges::is_permutation" << std::endl;
        std::vector<int> v = { 1, 2, 3 };

        std::vector<int> good = { 2, 3, 1 };
        std::vector<int> bad = { 2, 3, 4 };

        std::cout << std::boolalpha;
        std::cout << std::ranges::is_permutation(v, good) <<",";
        std::cout << std::ranges::is_permutation(v, bad) << ",";
        std::cout << std::endl;
    }

#pragma endregion

    std::cin.get();
}

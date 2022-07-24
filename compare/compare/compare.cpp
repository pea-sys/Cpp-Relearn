#include <iostream>
#include <compare>
#include <type_traits>

template<typename T, typename Cat>
using fallback_comp3way_t = std::conditional_t<std::three_way_comparable<T>, std::compare_three_way_result<T>, std::type_identity<Cat>>::type;

using category = std::weak_ordering;

template<typename T1, typename T2, typename T3>
struct triple {
    T1 t1;
    T2 t2;
    T3 t3;

    // <=>を使用可能ならそれを、そうでないなら< ==を使ってdefault実装
    auto operator<=>(const triple&) const
        ->std::common_comparison_category_t<fallback_comp3way_t<T1, category>, fallback_comp3way_t<T2, category>, fallback_comp3way_t<T3, category>>
        = default;
};

struct no_spaceship {
    int n;

    bool operator<(const no_spaceship& that) const noexcept {
        return n < that.n;
    }
    bool operator<=(const no_spaceship& that) const noexcept {
        return n <= that.n;
    }
    bool operator>(const no_spaceship& that) const noexcept {
        return n > that.n;
    }
    bool operator>=(const no_spaceship& that) const noexcept {
        return n >= that.n;
    }
    bool operator==(const no_spaceship& that) const noexcept {
        return n == that.n;
    }
};
struct vector2d {
    double vec[2];

    auto operator<=>(const vector2d& that) const noexcept -> std::partial_ordering {
        //2つの要素の順序関係が一致する時にのみ順序を定義
        //v1 = (a1, b1), v2 = (a2, b2) の2つのベクトルに対して、a1 < a2 かつ b1 < b2 の時に限り v1 < v2 と順序を定義する

        auto comp0 = vec[0] <=> that.vec[0];
        auto comp1 = vec[1] <=> that.vec[1];

        if (comp0 == comp1) return comp0;

        return std::partial_ordering::unordered;
    }

    //生配列との比較
    auto operator<=>(const double(&other)[2]) const noexcept -> std::partial_ordering {
        return *this <=> vector2d{ other[0], other[1] };
    }

};

struct insensitive_string {
    std::string str;

    auto operator<=>(const insensitive_string& that) const -> std::weak_ordering {
        //大文字小文字を同値として扱って比較

        std::size_t length = std::min(str.length(), that.str.length());

        for (std::size_t i = 0; i < length; ++i) {
            char c1, c2;
            if (std::isalpha(static_cast<unsigned char>(str[i])) && std::isalpha(static_cast<unsigned char>(that.str[i]))) {
                c1 = std::tolower(str[i]);
                c2 = std::tolower(that.str[i]);
            }
            else {
                c1 = str[i];
                c2 = that.str[i];
            }
            if (auto comp = c1 <=> c2; comp != 0) return comp;
        }

        return str.length() <=> that.str.length();
    }

    //素の文字列との比較
    auto operator<=>(const char* other) const -> std::weak_ordering {
        return *this <=> insensitive_string{ other };
    }

};
struct national {
    std::size_t my_number;
    std::string first_name;
    std::string last_name;

    auto operator<=>(const national& that) const -> std::strong_ordering {
        //宣言と逆順の比較

        if (auto comp = last_name <=> that.last_name; comp != 0) return comp;
        if (auto comp = first_name <=> that.first_name; comp != 0) return comp;
        return my_number <=> that.my_number;
    }
};
//<=>が使用可能ならそれを使用して比較結果を出力
template<std::three_way_comparable T>
void print_is_less(const T& t, const T& u) {
    std::cout << "<=> : " << ((t <=> u) < 0) << std::endl;
}

//<=>が使用可能でないなら<演算子を使用
template<typename T>
void print_is_less(const T& t, const T& u) {
    std::cout << "<   : " << (t < u) << std::endl;
}


//<演算子だけが使用可能
struct L {
    int n;
    friend bool operator<(const L& a, const L& b) { return a.n < b.n; }
};

//<=>演算子含め、全ての比較演算が可能
struct S {
    int n;

    friend auto operator<=>(const S& a, const S& b) = default;
    //friend bool operator== (const S& a, const S& b) = default;
};

int main()
{
    //partial_ordering:partial_orderingは<=>の戻り値型として利用される比較カテゴリ型の一つであり、その比較が半順序の要件を満たしている事を表明する
    {
        std::cout << "partial_ordering";
        vector2d v1 = { 2.0, 4.0 }, v2 = { 3.0, 1.0 };

        std::cout << std::boolalpha;
        std::cout << (v1 < v2) << std::endl;
        std::cout << (v1 <= v2) << std::endl;
        std::cout << (v1 > v2) << std::endl;
        std::cout << (v1 >= v2) << std::endl;

        std::cout << "\n";

        double v3[] = { 3.0, 5.0 };

        std::cout << (v1 < v3) << std::endl;
        std::cout << (v1 <= v3) << std::endl;
        std::cout << (v1 > v3) << std::endl;
        std::cout << (v1 >= v3) << std::endl;

        std::cout << "\n";

        //異種型間比較時の逆順の演算子の導出
        std::cout << (v3 < v1) << std::endl;
        std::cout << (v3 <= v1) << std::endl;
        std::cout << (v3 > v1) << std::endl;
        std::cout << (v3 >= v1) << std::endl;
    }
    //weak_orderingは<=>の戻り値型として利用される比較カテゴリ型の一つであり、その比較が弱順序の要件を満たしている事を表明する
    {
        std::cout << "weak_ordering";
        insensitive_string str1 = { "spaceship" }, str2 = { "SPACESHIP" };

        std::cout << std::boolalpha;
        std::cout << (str1 < str2) << std::endl;
        std::cout << (str1 <= str2) << std::endl;
        std::cout << (str1 > str2) << std::endl;
        std::cout << (str1 >= str2) << std::endl;

        std::cout << "\n";

        const char* str3 = "Three-way";

        std::cout << (str1 < str3) << std::endl;
        std::cout << (str1 <= str3) << std::endl;
        std::cout << (str1 > str3) << std::endl;
        std::cout << (str1 >= str3) << std::endl;

        std::cout << "\n";

        //異種型間比較時の逆順の演算子の導出
        std::cout << (str3 < str1) << std::endl;
        std::cout << (str3 <= str1) << std::endl;
        std::cout << (str3 > str1) << std::endl;
        std::cout << (str3 >= str1) << std::endl;
    }
    //strong_ordering:strong_orderingは<=>の戻り値型として利用される比較カテゴリ型の一つであり、その比較が全順序の要件を満たしている事を表明する
    {
        std::cout << "strong_ordering";
        national p1 = { 1, "tarou", "yamada" }, p2 = { 2, "hanako", "suzuki" };

        std::cout << std::boolalpha;
        std::cout << (p1 < p2) << std::endl;
        std::cout << (p1 <= p2) << std::endl;
        std::cout << (p1 > p2) << std::endl;
        std::cout << (p1 >= p2) << std::endl;
    }
    //common_comparison_category:与えられた全ての型から変換可能な共通比較カテゴリ型（common comparison category type）を求める
    {
        std::cout << "common_comparison_category";
        triple<int, double, no_spaceship> t1 = { 10, 3.14, {20} }, t2 = { 10, 3.14, {30} };

        std::cout << std::boolalpha;
        std::cout << (t1 < t2) << std::endl;
        std::cout << (t1 <= t2) << std::endl;
        std::cout << (t1 > t2) << std::endl;
        std::cout << (t1 >= t2) << std::endl;
    }
    //compare_three_way_result:three_way_comparable及びthree_way_comparable_withは、指定された型TもしくはT, Uの間で<=>による三方比較を使用可能であり、その戻り値型が指定した比較カテゴリ型Catに変換可能であることを表すコンセプトである。
    {
        std::cout << "compare_three_way_result";
        std::cout << std::boolalpha;
        L l1{ 1 }, l2{ 2 };
        S s1{ 1 }, s2{ 2 };

        print_is_less(1, 2);
        print_is_less(-0.0, +0.0);
        print_is_less(l1, l2);
        print_is_less(s1, s2);
    }
    //is_eq, is_neq, is_lt, is_lteq, is_gt, is_gteq:任意の<=>による比較結果のカテゴリ型の値から、bool値としての結果を取り出す
    {
        std::cout << "is_eq, is_neq, is_lt, is_lteq, is_gt, is_gteq";
        std::strong_ordering comp1 = 1 <=> 2;
        std::strong_ordering comp2 = 3 <=> 3;

        std::cout << std::boolalpha;

        // (1)
        std::cout << std::is_eq(comp1) << std::endl;
        std::cout << std::is_eq(comp2) << std::endl;
        std::cout << "\n";

        // (2)
        std::cout << std::is_neq(comp1) << std::endl;
        std::cout << std::is_neq(comp2) << std::endl;
        std::cout << "\n";

        // (3)
        std::cout << std::is_lt(comp1) << std::endl;
        std::cout << std::is_lt(comp2) << std::endl;
        std::cout << "\n";

        // (4)
        std::cout << std::is_lteq(comp1) << std::endl;
        std::cout << std::is_lteq(comp2) << std::endl;
        std::cout << "\n";

        // (5)
        std::cout << std::is_gt(comp1) << std::endl;
        std::cout << std::is_gt(comp2) << std::endl;
        std::cout << "\n";

        // (6)
        std::cout << std::is_gteq(comp1) << std::endl;
        std::cout << std::is_gteq(comp2) << std::endl;
    }

    std::cin.get();
}

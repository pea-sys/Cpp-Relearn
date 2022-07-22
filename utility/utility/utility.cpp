#include <iostream>
#include <memory>
#include <utility>
#include <cassert>
#include <tuple>
#include <optional>
#include <any>
#include <string>
#include <variant>
template <class Array>
void print(const Array& ar)
{
    std::cout << '[';
    bool first = true;
    for (auto x : ar) {
        if (first) {
            first = false;
        }
        else {
            std::cout << ',';
        }
        std::cout << x;
    }
    std::cout << ']';
}
template <class T, class A1, class A2>
std::shared_ptr<T> factory(A1&& a1, A2&& a2) {
    return std::shared_ptr<T>(new T(std::forward<A1>(a1), std::forward<A2>(a2)));
}
struct A {
    A(int&, const double&) {}
};
struct B {
    B() {}

    // 左辺値からコピー
    B(const B&)
    {
        std::cout << "copy" << std::endl;
    }

    // 右辺値からムーブ
    B(B&&)
    {
        std::cout << "move" << std::endl;
    }
};
struct C {
    void f()
    {
        std::cout << "non-const" << std::endl;
    }

    void f() const //constメンバ関数内では、メンバ変数を変更するような他の関数の呼び出しも禁止されます
    {
        std::cout << "const" << std::endl;
    }
};
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};
std::ostream& operator<<(std::ostream& os, const Point& p)
{
    return os << '(' << p.x << ',' << p.y << ')';
}
// この関数は、型Fromから型Toへの明示的な型変換が定義されている場合のみ
// オーバーロード解決の候補となる
template <class To, class From>
decltype(static_cast<To>(std::declval<From>())) convert(From&& x)
{
    return static_cast<To>(std::forward<From>(x));
}


void g(int a, int b, int c)
{
    std::cout << a << ", " << b << ", " << c << std::endl;
}

template <class T, T... Seq>
void f(std::integer_sequence<T, Seq...>)
{
    // 定数のシーケンス{0, 1, 2}を取り出して、関数g()の引数として転送
    g(Seq...);
}
int main()
{
    //swap:2つの値を入れ替える
    {
        std::cout << "swap" << std::endl;
        {
            int a = 1;
            int b = 2;

            using std::swap;
            swap(a, b);

            std::cout << a << ", " << b << std::endl;
        }
        {
            int a[] = { 1, 2, 3 };
            int b[] = { 4, 5, 6 };

            using std::swap;
            swap(a, b);

            print(a);
            std::cout << ", ";
            print(b);
            std::cout << std::endl;
        }
    }
    //exchange:値を書き換え、書き換え前の値を返す
    {
        std::cout << "exchange" << std::endl;
        int state = 1;
        int before = std::exchange(state, 2);

        std::cout << "state : " << state << std::endl;
        std::cout << "before : " << before << std::endl;
    }
    //forward:関数テンプレートの引数を転送する
    {
        std::cout << "forward" << std::endl;
        int i = 2;
        std::shared_ptr<A> sp2 = factory<A>(i, 1.414);  // OK
    }
    //std::move:左辺値を右辺値にキャストする
    {
        std::cout << "move" << std::endl;
        B a;
        B a1 = a;             // A(const A&)が呼ばれる
        B a2 = std::move(a);  // A(A&&)が呼ばれる
    }
    //as_const:左辺値参照をconst左辺値参照にする
    {
        std::cout << "as_const" << std::endl;
        C a;
        a.f(); // f()が呼ばれる
        std::as_const(a).f(); // f() constが呼ばれる
    }
    //declval:指定された型の値を得る
    {
        std::cout << "as_const" << std::endl;
        int x = 3;
        double d = convert<double>(x);

        std::cout << d << std::endl;
    }
    //cmp_equal:整数を安全に等値比較する
    {
        std::cout << "cmp_equal" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_equal(1, 1) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_equal(1u, 1u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_equal(1, 1u) << std::endl;
        std::cout << std::cmp_equal(1u, 1) << std::endl;
    }
    //cmp_not_equal:整数を安全に非等値比較する
    {
        std::cout << "cmp_not_equal" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_not_equal(1, 2) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_not_equal(1u, 2u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_not_equal(1, 2u) << std::endl;
        std::cout << std::cmp_not_equal(1u, 2) << std::endl;
    }
    //:cmp_less:整数に対して安全に、左辺が右辺より小さいかを比較する
    {
        std::cout << "cmp_less" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_less(1, 2) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_less(1u, 2u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_less(1, 2u) << std::endl;
        std::cout << std::cmp_less(1u, 2) << std::endl;
    }
    //cmp_less_equal:整数に対して安全に、左辺が右辺以下かを比較する
    {
        std::cout << "cmp_less_equal" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_less_equal(2, 2) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_less_equal(2u, 2u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_less_equal(2, 2u) << std::endl;
        std::cout << std::cmp_less_equal(2u, 2) << std::endl;
    }
    //cmp_greater:整数に対して安全に、左辺が右辺より大きいかを比較する
    {
        std::cout << "cmp_greater" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_greater(2, 2) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_greater(2u, 1u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_greater(2, 1u) << std::endl;
        std::cout << std::cmp_greater(2u, 1) << std::endl;
    }
    //cmp_greater_equal:整数に対して安全に、左辺が右辺以上かを比較する
    {
        std::cout << "cmp_greater_equal" << std::endl;
        std::cout << std::boolalpha;

        // 符号付き整数型同士の比較
        std::cout << std::cmp_greater_equal(1, 1) << std::endl;

        // 符号なし整数型同士の比較
        std::cout << std::cmp_greater_equal(2u, 1u) << std::endl;

        // 符号付き整数型と符号なし整数型の比較
        std::cout << std::cmp_greater_equal(2, 1u) << std::endl;
        std::cout << std::cmp_greater_equal(2u, 1) << std::endl;
    }
    //in_range:値が型のとりうる範囲内か判定する
    {
        std::cout << "in_range" << std::endl;
        // -1は、符号付き整数型intのとりうる範囲内である
        assert(std::in_range<int>(-1));

        // -1は、符号なし整数型unsigned intのとりうる範囲内ではない
        assert(!std::in_range<unsigned int>(-1));

        // 255は、符号なし8ビット整数の範囲内である
        assert(std::in_range<std::uint8_t>(255));

        // 255は、符号付き8ビット整数の範囲内ではない
        assert(!std::in_range<std::int8_t>(255));
    }
    //pair:2つの異なる型の値を保持する「組」を表現するためのクラス
    {
        std::cout << "pair" << std::endl;
        // pairオブジェクトの構築
        std::pair<int, std::string> p = std::make_pair(1, "hello");

        // 要素の参照
        std::cout << p.first << std::endl;
        std::cout << p.second << std::endl;
    }
    //make_pair:pairクラスのオブジェクトを構築する
    {
        std::cout << "make_pair" << std::endl;
        std::pair<int, char> p1 = std::make_pair(1, 'a');

        int ar[3] = { 1, 2, 3 };
        char c = 'b';

        // 配列はT*となり、reference_wrapper<T>はT&となる。
        std::pair<int*, char&> p2 = std::make_pair(ar, std::ref(c));
    }
    //piecewise_construct_tクラスは、オーバーロードのための空クラスである
    {
        std::cout << "piecewise_construct_t" << std::endl;
        // pairの要素型のコンストラクタ引数をtupleで渡して構築
        std::pair<Point, Point> p(std::piecewise_construct,
            std::make_tuple(1, 2),
            std::make_tuple(3, 4));

        std::cout << p.first << ", " << p.second << std::endl;
    }
    //in_placeクラスは、オーバーロードのための空クラスである
    {
        std::cout << "in_place" << std::endl;
        // stringクラスのコンストラクタ引数3と'A'をとり、
        // optionalクラス内でstring型のオブジェクトを生成する。
        std::optional<std::string> p{ std::in_place, 3, 'A' };

        std::cout << p.value() << std::endl;
    }
    //in_place_type_t:in_place_type_tクラスは、オーバーロードのための空クラスである
    {
        std::cout << "in_place_type" << std::endl;
        std::any x{  std::in_place_type<std::string>, 3, 'z'  };

        std::cout << std::any_cast<std::string>(x) << std::endl;
    }
    //in_place_index:標準ライブラリの特定機能のコンストラクタに渡すことができる、曖昧性の解消タグである
    {
        std::cout << "in_place_index" << std::endl;
        // std::variant<int, int> x { 1 }; // どちらの int か曖昧

        std::variant<int, int> x{
          std::in_place_index<0>,
          1
        }; // テンプレート引数の最初の int に 1 を格納して構築

        std::visit([](auto&& arg) {std::cout << arg; }, x);
    }
    //integer_sequence:任意の整数型のシーケンスをコンパイル時に表現するクラスである
    {
        std::cout << "integer_sequence" << std::endl;
        f(std::integer_sequence<int, 0, 1, 2>());
    }
    //make_index_sequence:要素数を指定して、0から始まるsize_t型整数シーケンスを生成する、make_integer_sequenceの別名である
    {
        std::cout << "make_index_sequence" << std::endl;
        f(std::make_index_sequence<3>());
    }
    std::cin.get();
}
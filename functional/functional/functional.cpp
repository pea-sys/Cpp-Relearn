#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>


int add(int x) { return x + 1; }

struct functor {
    auto operator()() -> int {
        return 10;
    }

    auto operator()(int n) -> int {
        return n;
    }
};

struct has_member {
    auto member_function(int n) -> int {
        return n;
    }

    int member_object;
};

auto f(int) -> double {
    return 3.14159265359;
}

auto g(int) -> double {
    return 2.71828182846;
}

auto g(int, int) -> double {
    return 3.14159265359;
}


struct X {
    void f(int a, int b, int c) {
        std::cout << a << ',' << b << ',' << c << std::endl;
    }
};

void h(int a, int b, int c) {
    std::cout << a << ',' << b << ',' << c << std::endl;
}

int add3(int a, int b, int c)
{
    return a + b + c;
}

void square(double x)
{
    std::cout << x << "^2 == " << x * x << std::endl;
}

template<class Expr>
void test_for_bind(const Expr&)
{
    std::cout << std::is_bind_expression<Expr>::value << std::endl;
}
template<class Expr>
void test_for_placeholder(const Expr&)
{
    std::cout << std::is_placeholder<Expr>::value << std::endl;
}



void a(int& x)
{
    ++x;
}

template <class T>
void b(T x)
{
    a(x);
}


bool pred_func(int, char, double)
{
    return true;
}

struct pred_functor {
    bool operator()(double, int)
    {
        return false;
    }
};

//intを受け取ってその値と射影した値を足して返す
//デフォルトはnを2倍にする
template<typename Projection = std::identity>
auto c(int n, Projection proj = {}) -> int {
    return n + proj(n);
}

int main()
{
    //function:functionクラステンプレートは、パラメータの型リストArgTypes...、戻り値の型Rに合致する、あらゆる関数ポインタ、関数オブジェクト、メンバ関数ポインタ、メンバ変数ポインタを保持できるクラスである
    {
        std::cout << "function" << std::endl;
        // 関数を代入
        std::function<int(int)> f = add;

        // 関数オブジェクトを代入
        f = [](int x) { return x + 1; };

        // 保持している関数を呼び出す
        int result = f(1);
        std::cout << result << std::endl;
    }
    //bad_function_call:std::bad_functionは、空のstd::functionオブジェクトに対してoperator()を呼び出した際に送出される例外クラスである
    {
        std::cout << "bad_function_call" << std::endl;
        std::function<void()> f;
        try {
            f();
        }
        catch (std::bad_function_call& e) {
            std::cout << "bad function call" << std::endl;
        }
    }
    //invoke:関数呼び出し可能なオブジェクトfとその引数args...の組み合わせでINVOKE要件に従った関数呼び出しを行う
    {
        std::cout << "invoke" << std::endl;
        functor fobj{};

        //関数オブジェクト呼び出し
        std::cout << std::invoke(fobj) << std::endl;
        std::cout << std::invoke(fobj, 100) << std::endl;

        has_member obj{ 25 };
        //メンバ関数呼び出し
        std::cout << std::invoke(&has_member::member_function, obj, 50) << std::endl;
        //メンバ変数呼び出し
        std::cout << std::invoke(&has_member::member_object, obj) << std::endl;

        auto ref = std::ref(obj);
        //reference_wrapperからのメンバ関数呼び出し
        std::cout << std::invoke(&has_member::member_function, ref, 50) << std::endl;
        //reference_wrapperからのメンバ変数呼び出し
        std::cout << std::invoke(&has_member::member_object, ref) << std::endl;

        //関数呼び出し
        std::cout << std::invoke(f, 10) << std::endl;

        //フリー関数は一度関数ポインタにしないとオーバーロード解決できない
        std::cout << std::invoke((double(*)(int))g, 10) << std::endl;
        std::cout << std::invoke((double(*)(int, int))g, 10, 10) << std::endl;

        //コンパイルエラー
        //std::cout << std::invoke(g, 10) << std::endl;
    }
    //bind_front:関数の引数を先頭から順に部分適用する
    {
        std::cout << "bind_front" << std::endl;
        {
            // メンバ関数ポインタと*thisを束縛する
            X x;
            auto f = std::bind_front(&X::f, x);

            // メンバ関数の引数を渡して呼び出す
            f(1, 2, 3);
        }

        // 「メンバ関数ポインタと*thisの束縛」という機能に限定されてはおらず、
        // 先頭から順に束縛できる (関数オブジェクトの指定だけは必須)
        {
            X x;
            std::bind_front(&X::f)(x, 1, 2, 3);
            std::bind_front(&X::f, x, 1)(2, 3);

            std::bind_front(h)(1, 2, 3);
            std::bind_front(h, 1)(2, 3);
        }
    }
    //bind:Callable オブジェクトに対し、引数を部分的に束縛(bind)する
    {
        std::cout << "bind" << std::endl;
        // 第1引数のみを先に渡す
        using namespace std::placeholders;
        std::function<int(int, int)> f = std::bind(add3, 2, _1, _2);

        // 残りの引数を渡して関数を呼び出す
        const int result = f(3, 4);

        std::cout << result << std::endl;
    }
    //is_bind_expression:型 T が bind() の呼出し結果かどうかを判別する
    {
        std::cout << "is_bind_expression" << std::endl;
        test_for_bind(3.0 * 3.0);
        test_for_bind(std::bind(square, 3));
    }
    //is_placeholder:型 T がプレースホルダかどうかを判別する
    {
        std::cout << "is_placeholder" << std::endl;
        test_for_placeholder(3.0);
        test_for_placeholder(std::placeholders::_3);
    }
    //mem_fn:与えられたメンバ関数を呼び出す Callable オブジェクトを生成して返す
    {
        std::cout << "mem_fn" << std::endl;
        auto l = std::make_shared<std::less<int>>();
        std::cout << std::boolalpha;
        std::cout << (*l)(3, 5) << std::endl;
        std::cout << std::mem_fn(&std::less<int>::operator ())(l, 3, 5) << std::endl;
        std::cout << std::bind(*l, std::placeholders::_1, 5)(3) << std::endl;

        // std::cout << std::bind(l, std::placeholders::_1, 5)(3) << std::endl;
        //   エラー！ std::shared_ptr< std::less<int> > は Callable ではない

        // mem_fn() で包むと Callable になる
        std::cout <<
            std::bind(std::mem_fn(&std::less<int>::operator ()), l, std::placeholders::_1, 5)(3)
            << std::endl;
    }
    //reference_wrapper:コピー・代入可能なオブジェクトとして持ちまわれる参照オブジェクトを提供する
    {
        std::cout << "reference_wrapper" << std::endl;
        int x = 3;

        // 関数テンプレートの型推論によって、xの型が非参照のintと見なされる
        //b(x);

        // 関数テンプレートに変数を参照として渡す
        b(std::ref(x));

        std::cout << x << std::endl;
    }
    //cref変数へのconst参照tを保持するreference_wrapperオブジェクトを生成する
    {
        std::cout << "cref" << std::endl;
        int x = 3;

        // 参照ラッパーrは、変数xへのconst参照を保持する
        std::reference_wrapper<const int> r = std::cref(x);

        ++x;

        const int& rx = r.get();
        std::cout << rx << std::endl;
    }
    //hash:キーとなる型のためのハッシュ値を計算する関数オブジェクトである。
    {
        std::cout << "hash" << std::endl;
        int x;

        // char型の値'C'に対するハッシュ値を求める
        std::cout << std::hash<char>()('C') << std::endl;

        // int型の値100に対するハッシュ値を求める
        std::cout << std::hash<int>()(100) << std::endl;

        // double型の値3.14に対するハッシュ値を求める
        std::cout << std::hash<double>()(3.14) << std::endl;

        // int*型のアドレス値(&x)に対するハッシュ値を求める
        std::cout << std::hash<int*>()(&x) << std::endl;
    }
    //plus:加算を行う関数オブジェクトである
    {
        std::cout << "plus" << std::endl;
        std::cout << std::plus<int>()(3, 5) << std::endl;
    }
    //minusクラスは、減算を行う関数オブジェクト
    {
        std::cout << "minus" << std::endl;
        std::cout << std::minus<int>()(3, 2) << std::endl;
    }
    //multipliesクラスは、乗算を行う関数オブジェクトである
    {
        std::cout << "multiplies" << std::endl;
        std::cout << std::multiplies<int>()(3, 2) << std::endl;
    }
    //dividesクラスは、除算を行う関数オブジェクトである
    {
        std::cout << "divides" << std::endl;
        std::cout << std::divides<int>()(6, 2) << std::endl;
    }
    //modulusクラスは、剰余算を行う関数オブジェクトである
    {
        std::cout << "modulus" << std::endl;
        std::cout << std::modulus<int>()(6, 4) << std::endl;
    }
    //negateクラスは、符号反転を行う関数オブジェクトである
    {
        std::cout << "negate" << std::endl;
        std::cout << std::negate<int>()(3) << std::endl;
        std::cout << std::negate<int>()(-3) << std::endl;
    }
    //equal_toクラスは、等値比較を行う関数オブジェクトである
    {
        std::cout << "equal_to" << std::endl;
        std::cout << std::boolalpha << std::equal_to<int>()(3, 3) << std::endl;
    }
    //not_equal_toクラスは、非等値比較を行う関数オブジェクトである
    {
        std::cout << "not_equal_to" << std::endl;
        std::cout << std::boolalpha << std::not_equal_to<int>()(3, 2) << std::endl;
    }
    //lessクラスは、左辺が右辺より小さいかの比較を行う関数オブジェクトである
    {
        std::cout << "less" << std::endl;
        std::cout << std::boolalpha << std::less<int>()(2, 3) << std::endl;
    }
    //less_equalクラスは、左辺が右辺以下かの比較を行う関数オブジェクトである
    {
        std::cout << "less_equal" << std::endl;
        std::cout << std::boolalpha << std::less_equal<int>()(3, 3) << std::endl;
    }
    //greaterクラスは、左辺が右辺より大きいかの比較を行う関数オブジェクトである
    {
        std::cout << "greater" << std::endl;
        std::cout << std::boolalpha << std::greater<int>()(3, 2) << std::endl;
    }
    //greater_equalクラスは、左辺が右辺以上かの比較を行う関数オブジェクトである
    {
        std::cout << "greater_equal" << std::endl;
        std::cout << std::boolalpha << std::greater_equal<int>()(3, 2) << std::endl;
    }
    //logical_andクラスは、論理積(AND)を計算する関数オブジェクトである
    {
        std::cout << "logical_and" << std::endl;
        std::cout << std::boolalpha << std::logical_and<bool>()(true, true) << std::endl;
    }
    //logical_orクラスは、論理和(OR)を計算する関数オブジェクトである
    {
        std::cout << "logical_or" << std::endl;
        std::cout << std::boolalpha << std::logical_or<bool>()(false, true) << std::endl;
    }
    //logical_notクラスは、論理否定(NOT)を計算する関数オブジェクトである
    {
        std::cout << "logical_not" << std::endl;
        std::cout << std::boolalpha << std::logical_not<bool>()(false) << std::endl;
    }
    //bit_andクラスは、ビットごとの論理積(AND)をとる関数オブジェクトである
    {
        std::cout << "bit_and" << std::endl;
        // テンプレート引数で operator() の引数の型を指定した書き方（C++11 でも使用可能）
        std::cout << "0x" << std::hex << std::bit_and<int>()(0xFA, 0x47) << std::endl;

        // テンプレート引数で operator() の引数の型を指定しない書き方（C++14 以降で使用可能）
        std::cout << "0x" << std::hex << std::bit_and<>()(0xFA, 0x47) << std::endl;
    }
    //bit_orクラスは、ビットごとの論理和(OR)をとる関数オブジェクトである
    {
        std::cout << "bit_or" << std::endl;
        // テンプレート引数で operator() の引数の型を指定した書き方（C++11 でも使用可能）
        std::cout << "0x" << std::hex << std::bit_or<int>()(0xFA, 0x47) << std::endl;

        // テンプレート引数で operator() の引数の型を指定しない書き方（C++14 以降で使用可能）
        std::cout << "0x" << std::hex << std::bit_or<>()(0xFA, 0x47) << std::endl;
    }
    //bit_xorクラスは、ビットごとの排他的論理和(XOR)をとる関数オブジェクトである。
    {
        std::cout << "bit_xor" << std::endl;
        // テンプレート引数で operator() の引数の型を指定した書き方（C++11 でも使用可能）
        std::cout << "0x" << std::hex << std::bit_xor<int>()(0xFA, 0x47) << std::endl;

        // テンプレート引数で operator() の引数の型を指定しない書き方（C++14 以降で使用可能）
        std::cout << "0x" << std::hex << std::bit_xor<>()(0xFA, 0x47) << std::endl;
    }
    //bit_notクラスは、ビットごとの論理否定(NOT)をとる関数オブジェクトである
    {
        std::cout << "bit_not" << std::endl;
        // テンプレート引数で operator() の引数の型を指定した書き方
        std::cout << "0x" << std::hex << std::bit_not<int>()(0xFA) << std::endl;

        // テンプレート引数で operator() の引数の型を指定しない書き方
        std::cout << "0x" << std::hex << std::bit_not<>()(0xFA) << std::endl;
    }
    //任意個数の引数をとってbool型を返す関数オブジェクトを受け取り、戻り値を論理反転する関数オブジェクトに変換する
    {
        std::cout << "not_fn" << std::endl;
        std::cout << std::boolalpha;

        auto not_func = std::not_fn(pred_func);
        std::cout << not_func(1, 'a', 3.14) << std::endl;

        auto not_functor = std::not_fn(pred_functor{});
        std::cout << not_functor(3.14, 1) << std::endl;
    }
    //identityクラスは、受け取った引数をそのまま返す（恒等変換を行う）関数オブジェクトである。この恒等変換はC++の意味論においてのものであり、値そのもの及びCV修飾や値カテゴリも含めて引数をそのまま返す
    {
        std::cout << "identity" << std::endl;
        //デフォルトの射影で呼び出し
        std::cout << f(10) << std::endl;
        //何も足さないようにカスタマイズ
        std::cout << c(10, [](int) {return 0; }) << std::endl;
        //3倍にする
        std::cout << c(10, [](int m) {return m + m; }) << std::endl;
    }
    //std::default_searcherは、std::search()のデフォルトの検索法で、シーケンス (text) からサブシーケンス (pattern) を検索する関数オブジェクトである
    {
        std::cout << "std::default_searcher" << std::endl;
        // text内のpatternを検索する
  //                      xxxx
        std::string text = "babcabaabaac";
        std::string pattern = "abaa";

        // patternを登録
        std::default_searcher searcher{
          pattern.cbegin(),
          pattern.cend()
        };

        // textを指定して検索を実行
        using iterator = std::string::const_iterator;
        iterator result = std::search(text.cbegin(), text.cend(), searcher);

        // 見つかった
        if (result != text.cend()) {
            // 見つかった位置を取得
            std::ptrdiff_t n = std::distance(text.cbegin(), result);

            // 見つかった文字列 (pattern) を取得
            std::string s{ result, result + pattern.size() };

            std::cout << n << std::endl;
            std::cout << s << std::endl;
        }
        // 見つからなかった
        else {
            std::cout << "not found" << std::endl;
        }
    }
    //std::boyer_moore_searcherは、ボイヤー・ムーア法によって、シーケンス (text) からサブシーケンス (pattern) を検索する関数オブジェクト
    {
        std::cout << "std::boyer_moore_searcher" << std::endl;
        // text内のpatternを検索する
  //                      xxxx
        std::string text = "babcabaabaac";
        std::string pattern = "abaa";

        // patternを登録
        std::boyer_moore_searcher searcher{
          pattern.cbegin(),
          pattern.cend()
        };

        // textを指定して検索を実行
        using iterator = std::string::const_iterator;
        std::pair<iterator, iterator> result = searcher(text.cbegin(), text.cend());

        // 見つかった
        if (result.first != result.second) {
            // 見つかった位置を取得
            std::ptrdiff_t n = std::distance(text.cbegin(), result.first);

            // 見つかった文字列 (pattern) を取得
            std::string s{ result.first, result.second };

            std::cout << n << std::endl;
            std::cout << s << std::endl;
        }
        // 見つからなかった
        else {
            std::cout << "not found" << std::endl;
        }
    }
    //std::boyer_moore_horspool_searcherは、ボイヤー・ムーア・ホースプール法によって、シーケンス (text) からサブシーケンス (pattern) を検索する関数オブジェクトである
    {
        // text内のpatternを検索する
  //                      xxxx
        std::string text = "babcabaabaac";
        std::string pattern = "abaa";

        // patternを登録
        std::boyer_moore_horspool_searcher searcher{
          pattern.cbegin(),
          pattern.cend()
        };

        // textを指定して検索を実行
        using iterator = std::string::const_iterator;
        std::pair<iterator, iterator> result = searcher(text.cbegin(), text.cend());

        // 見つかった
        if (result.first != result.second) {
            // 見つかった位置を取得
            std::ptrdiff_t n = std::distance(text.cbegin(), result.first);

            // 見つかった文字列 (pattern) を取得
            std::string s{ result.first, result.second };

            std::cout << n << std::endl;
            std::cout << s << std::endl;
        }
        // 見つからなかった
        else {
            std::cout << "not found" << std::endl;
        }
    }
    std::cin.get();
}

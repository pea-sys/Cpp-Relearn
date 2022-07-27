
#include <iostream>
#include <concepts>
#include <vector>
#include <type_traits>
#include <optional>
#include <memory>
#include <span>
#include <any>
#include <random>

template<typename T, typename U>
    requires std::derived_from<T, U>
void check_derived() {
    std::cout << "U is base class of T" << std::endl;
}

template<typename T, typename U>
void check_derived() {
    std::cout << "U and T are others" << std::endl;
}
struct Base {};

//public継承
struct Derived1 : Base {};
//private
struct Derived2 : private Base {};
//protected
struct Derived3 : protected Base {};
//曖昧な継承
struct Derived4 : Base, Derived1 {};
//仮想継承
struct Derived5 : virtual public Base {};
struct Derived6 : virtual public Base {};
struct Derived7 : Derived5, Derived6 {};
//継承の継承
struct Derived8 : Derived1 {};


struct convert_int {
    operator int() { return 0; }
    convert_int(int) {}
};

struct convert_double {
    explicit convert_double(double) {}
    explicit operator double() { return 0.0; }
};

// 明示的な変換と暗黙的な変換で結果が異なる例
struct vague_convert {
    operator int() { return -1; }
    explicit operator double() { return 1.0; }
};


template<typename T, typename U>
    requires std::common_reference_with<T, U>
void f() {
    std::cout << "T, U share a common reference type" << std::endl;
}

template<typename T, typename U>
void f() {
    std::cout << "T, U not share a common reference type" << std::endl;
}

template<typename T, typename U>
    requires std::common_with<T, U>
void g() {
    std::cout << "T, U share a common type" << std::endl;
}

template<typename T, typename U>
void g() {
    std::cout << "T, U not share a common type" << std::endl;
}

template<std::integral T>
void h(const char* name) {
    std::cout << name << " is integral" << std::endl;
}

template<typename T>
void h(const char* name) {
    std::cout << name << " is not integral" << std::endl;
}
template<std::signed_integral T>
void j(const char* name) {
    std::cout << name << " is signed_integral" << std::endl;
}

template<typename T>
void j(const char* name) {
    std::cout << name << " is not signed_integral" << std::endl;
}

template<std::unsigned_integral T>
void k(const char* name) {
    std::cout << name << " is unsigned_integral" << std::endl;
}

template<typename T>
void k(const char* name) {
    std::cout << name << " is not unsigned_integral" << std::endl;
}

template<std::floating_point T>
void k(const char* name) {
    std::cout << name << " is floating point" << std::endl;
}

template<typename T, typename... Args>
    requires std::constructible_from<T, Args...>
void h(const char* name, const char* args) {
    std::cout << name << " is constructible from " << args << std::endl;
}

template<typename T, typename... Args>
void h(const char* name, const char* args) {
    std::cout << name << " is not constructible from " << args << std::endl;
}

template<std::default_initializable T>
void m(const char* name) {
    std::cout << name << " is default initializable" << std::endl;
}

template<typename T>
void m(const char* name) {
    std::cout << name << " is not default initializable" << std::endl;
}

template<std::copy_constructible T>
void n(const char* name) {
    std::cout << name << " is copy constructible" << std::endl;
}

template<typename T>
void n(const char* name) {
    std::cout << name << " is not copy constructible" << std::endl;
}
struct S {
    S() = delete;

    S(int m) : n(m) {}

    int n = 0;
};

struct SM {
    SM(const SM&) = delete;

    SM(int m) : n(m) {}

    int n = 0;
};

struct M {
    M(M&&) = delete;
};
struct C {
    C(const C&) = default;
};

template<std::move_constructible T>
void p(const char* name) {
    std::cout << name << " is move constructible" << std::endl;
}

template<typename T>
void p(const char* name) {
    std::cout << name << " is not move constructible" << std::endl;
}

struct SN {
    SN(SN&&) = delete;

    SN(int m) : n(m) {}

    int n = 0;
};
struct N {
    N(N&&) = default;
};

template<std::destructible T>
void q(const char* name) {
    std::cout << name << " is destructible" << std::endl;
}

template<typename T>
void q(const char* name) {
    std::cout << name << " is not destructible" << std::endl;
}

struct S1 {
    ~S1() noexcept(false) {}
};

struct S2 {
    ~S2() = delete;
};

template<typename LHS, typename RHS>
    requires std::assignable_from<LHS, RHS>
void r(const char* name, const char* arg) {
    std::cout << name << " is assignable from " << arg << std::endl;
}

template<typename LHS, typename RHS>
void r(const char* name, const char* arg) {
    std::cout << name << " is not assignable from " << arg << std::endl;
}

struct S3 {
    S3& operator=(const S3&) = default;
    S3& operator=(S3&&) = delete;
};


template<typename T>
    requires std::swappable<T>
void s(const char* name) {
    std::cout << name << " is swappable" << std::endl;
}

template<typename T>
void s(const char* name) {
    std::cout << name << " is not swappable" << std::endl;
}
namespace NS {
    struct swappable1 {
        int n = 0;

        swappable1(int m) : n(m) {}

        swappable1(swappable1&&) = delete;
    };

    // 非メンバ関数として定義
    void swap(swappable1& lhs, swappable1& rhs) {
        std::swap(lhs.n, rhs.n);
    }


    struct swappable2 {
        double d = 0.0;

        swappable2(double v) : d(v) {}

        swappable2(swappable2&&) = delete;

        // Hidden friendsな関数として定義
        friend void swap(swappable2& lhs, swappable2& rhs) {
            std::swap(lhs.d, rhs.d);
        }
    };
}

struct not_swappable {
    not_swappable(not_swappable&&) = delete;
    not_swappable& operator==(not_swappable&&) = delete;
};

template<typename T>
    requires std::equality_comparable<T>
void u(const char* name) {
    std::cout << name << " is equality comparable" << std::endl;
}

template<typename T>
void u(const char* name) {
    std::cout << name << " is not equality comparable" << std::endl;
}


struct eq_comp {
    int n = 0;

    friend bool operator==(const eq_comp&, const eq_comp&) = default;
};

struct not_eq_comp {
    int n = 0;

    friend bool operator==(const not_eq_comp&, const not_eq_comp&) = delete;
};

void func(int);
auto lambda = [](auto a) { return a; };
auto mut_lambda = [n = 0](auto a) mutable { ++n; return n + a; };

struct invocable {
    template<typename T>
    void operator()(T&& t) const {
        return t;
    }
};

struct not_invocable {};
#include <iostream>
#include <concepts>

template<typename T>
    requires std::totally_ordered<T>
void v(const char* name) {
    std::cout << name << " is totally ordered" << std::endl;
}

template<typename T>
void v(const char* name) {
    std::cout << name << " is not totally ordered" << std::endl;
}


struct ordered {
    int n = 0;

    friend auto operator<=>(const ordered&, const ordered&) = default;
};


struct not_ordered {
    int n = 0;

    bool operator==(const not_ordered&) const = default;
};
template<std::movable T>
void w(const char* name) {
    std::cout << name << " is movable" << std::endl;
}

template<typename T>
void w(const char* name) {
    std::cout << name << " is not movable" << std::endl;
}


struct movable {
    movable(movable&&) = default;
    movable& operator=(movable&&) = default;
};

struct not_movable1 {
    not_movable1(not_movable1&&) = delete;
};

struct not_movable2 {
    not_movable2& operator=(not_movable2&&) = delete;
};


template<std::copyable T>
void x(const char* name) {
  std::cout << name << " is copyable" << std::endl;
}

template<typename T>
void x(const char* name) {
  std::cout << name << " is not copyable" << std::endl;
}


struct copyable {
  copyable(const copyable&) = default;
  copyable& operator=(const copyable&) = default;
};



struct not_copyable1 {
  not_copyable1(const not_copyable1&) = delete;
};

struct not_copyable2 {
  not_copyable2& operator=(const not_copyable2&) = delete;
};


template<std::semiregular T>
void y(const char* name) {
  std::cout << name << " is semiregular" << std::endl;
}

template<typename T>
void y(const char* name) {
  std::cout << name << " is not semiregular" << std::endl;
}



template<std::regular T>
void z(const char* name) {
    std::cout << name << " is regular" << std::endl;
}

template<typename T>
void z(const char* name) {
    std::cout << name << " is not regular" << std::endl;
}


struct regular {
    regular() = default;
    regular(const regular&) = default;
    regular& operator=(const regular&) = default;

    bool operator==(const regular&) const = default;
};

struct semiregular {
    semiregular() = default;
    semiregular(const semiregular&) = default;
    semiregular& operator=(const semiregular&) = default;
};



template<typename F, typename... Args>
    requires std::invocable<F, Args...>
void az(const char* name) {
    std::cout << name << " is invocable" << std::endl;
}

template<typename F, typename... Args>
void az(const char* name) {
    std::cout << name << " is not invocable" << std::endl;
}



template<typename F, typename... Args>
    requires std::predicate<F, Args...>
void ax(const char* name) {
    std::cout << name << " is predicate" << std::endl;
}

template<typename F, typename... Args>
void ax(const char* name) {
    std::cout << name << " is not predicate" << std::endl;
}
bool func1(int);
int  func2(int);
int* func3(int);


struct predicate {
    bool operator()(int n) const {
        return n < 10;
    }
};

struct not_predicate {
    void operator()(int) {
    }
};
int main()
{
    //same_as:型T, Uが同じ型であることを表すコンセプトである。これを満たす型のペアはCVや参照修飾も含めて完全に同じ型である必要がある
    {
        std::cout << "concepts";
        std::cout << std::boolalpha;

        std::cout << std::same_as<int, int> << std::endl;
        std::cout << std::same_as<int, short> << std::endl;
        std::cout << std::same_as<int, const int> << std::endl;
        std::cout << std::same_as<int, int&> << std::endl;
        std::cout << std::same_as<int, std::int32_t> << std::endl;
        std::cout << std::same_as<std::int32_t, int> << std::endl;
    }
    //derived_from:型DerivedがBaseから派生していることを表すコンセプトである
    {
        std::cout << "derived_from";
        check_derived<int, int>();
        check_derived<Base, const Base>();
        check_derived<Derived1, Base>();
        check_derived<Derived2, Base>();
        check_derived<Derived3, Base>();
        check_derived<Derived4, Base>();
        check_derived<Derived5, Base>();
        check_derived<Derived7, Base>();
        check_derived<Derived8, Base>();
    }
    //convertible_to:Fromに指定された型および値カテゴリから型Toへ変換可能であることを表すコンセプト
    {
        std::cout << "convertible_to";
        std::cout << std::boolalpha;

        std::cout << "--- fundamental type ---\n";
        std::cout << std::convertible_to<int, short> << std::endl;
        std::cout << std::convertible_to<short, int> << std::endl;
        std::cout << std::convertible_to<std::size_t, int> << std::endl;
        std::cout << std::convertible_to<int, std::size_t> << std::endl;
        std::cout << std::convertible_to<int, const int> << std::endl;
        std::cout << std::convertible_to<const int, int> << std::endl;
        std::cout << std::convertible_to<int, double> << std::endl;
        std::cout << std::convertible_to<double, int> << std::endl;
        std::cout << std::convertible_to<float, double> << std::endl;
        std::cout << std::convertible_to<double, float> << std::endl;
        std::cout << std::convertible_to<int*, const int*> << std::endl;
        std::cout << std::convertible_to<const int*, int*> << std::endl;

        std::cout << "\n--- program defined type ---\n";
        std::cout << std::convertible_to<convert_int, int> << std::endl;
        std::cout << std::convertible_to<int, convert_int> << std::endl;
        std::cout << std::convertible_to<convert_double, double> << std::endl;
        std::cout << std::convertible_to<double, convert_double> << std::endl;
        std::cout << std::convertible_to<vague_convert, int> << std::endl;
        std::cout << std::convertible_to<vague_convert, double> << std::endl;
    }
    //common_reference_with:T, Uの間で、どちらの型からも変換可能な共通の参照型が存在することを表すコンセプトである
    {
        std::cout << "common_reference_with";
        f<std::size_t&, int&>();
        f<std::string&, std::string_view&>();
        f<std::vector<int>, std::vector<int>&>();
        f<std::vector<int>, std::vector<double>>();
        f<std::pair<int&, double&>, std::pair<int, double>>();
    }
    //common_with:T, Uの間で、どちらの型からも変換可能な共通の型が存在することを表すコンセプトである
    {
        std::cout << "common_with";
        g<std::size_t&, int&>();
        g<std::string&, std::string_view&>();
        g<std::vector<int>, std::vector<int>&>();
        g<std::vector<int>, std::vector<double>>();
        g<std::pair<int&, double&>, std::pair<int, double>>();
    }
    //integral:任意の型Tが整数型であることを表すコンセプトである。
    {
        std::cout << "integral";
        h<bool>("bool");
        h<int>("int");
        h<std::size_t>("std::size_t");
        h<std::uint32_t>("std::uint32_t");
        h<char>("char");
        h<char8_t>("char8_t");

        std::cout << "\n";

        h<int*>("int*");
        h<int&>("int&");
        h<std::integral_constant<int, 1>>("std::integral_constant<int, 1>");
    }
    //signed_integral:任意の型Tが符号付き整数型であることを表すコンセプトである
    {
        std::cout << "signed_integral";
        j<int>("int");
        j<std::int64_t>("std::int64_t");
        j<int>("char");

        std::cout << "\n";

        j<bool>("bool");
        j<std::size_t>("std::size_t");
        j<std::uint32_t>("std::uint32_t");
        j<char8_t>("char8_t");
        j<int*>("int*");
        j<int&>("int&");
        j<std::integral_constant<int, 1>>("std::integral_constant<int, 1>");
    }
    //unsigned_integral:任意の型Tが符号なし整数型であることを表すコンセプトである
    {
        std::cout << "unsigned_integral";
        k<bool>("bool");
        k<std::size_t>("std::size_t");
        k<std::uint32_t>("std::uint32_t");
        k<char8_t>("char8_t");

        std::cout << "\n";

        k<int>("int");
        k<std::int64_t>("std::int64_t");
        k<int>("char");
        k<int*>("int*");
        k<int&>("int&");
        k<std::integral_constant<unsigned int, 1>>("std::integral_constant<unsigned int, 1>");
    }
    //floating_point:任意の型Tが浮動小数点数型であることを表すコンセプトである
    {
        std::cout << "floating_point";
        k<float>("float");
        k<double>("double");
        k<long double>("long double");
        k<const double>("const double");
        k<volatile double>("volatile double");

        std::cout << "\n";

        k<bool>("bool");
        k<int>("int");
        k<std::size_t>("std::size_t");
        k<double*>("double*");
        k<double&>("double&");
    }
    //constructible_from:任意の型Tが引数型Args...から構築可能であることを表すコンセプトである
    {
        std::cout << "constructible_fromt";
        h<std::vector<int>, std::initializer_list<int>>("std::vector<int>", "std::initializer_list<int>");
        h<std::optional<std::vector<int>>, std::in_place_t, std::initializer_list<int>, std::allocator<int>>("std::optional<std::vector<int>>", "std::in_place_t, std::initializer_list<int>, std::allocator<int>");
        h<std::unique_ptr<const int>, std::unique_ptr<int>>("std::unique_ptr<const int>", "std::unique_ptr<int>");
        h<std::span<int, 4>, std::array<int, 4>&>("std::span<int, 4>", "std::array<int, 4>&");

        std::cout << "\n";

        h<std::vector<int>, std::vector<double>>("std::vector<int>", "std::vector<double>");
        h<std::optional<std::vector<int>>, std::optional<std::vector<double>>>("std::optional<std::vector<int>>", "std::optional<std::vector<double>>");
        h<std::unique_ptr<int>, std::unique_ptr<const int>>("std::unique_ptr<int>", "std::unique_ptr<const int>");
        h<std::span<int, 4>, std::array<int, 4>>("std::span<int, 4>", "std::array<int, 4>");
    }
    //default_initializable:任意の型Tがデフォルト構築可能であること表すコンセプトである
    {
        std::cout << "default_initializable";
        m<int>("int");
        m<S>("S");
    }
    //copy_constructible:任意の型Tがコピー構築可能であること表すコンセプトである
    {
        std::cout << "copy_constructible";
        n<int>("int");
        n<SM>("SM");
        n<M>("M");
        n<C>("C");
    }
    //move_constructible:任意の型Tがムーブ構築可能であること表すコンセプト
    {
        std::cout << "move_constructible";
        p<int>("int");
        p<SN>("S");
        p<N>("N");
    }
    //destructible:任意の型Tが破棄可能であることを表すコンセプトである
    {

        std::cout << "destructible";
        q<int>("int");
        q<std::vector<int>>("std::vector<int>");
        q<S1>("S1");
        q<S2>("S2");
        q<void>("void");
    }
    //assignable_from:指定された型および値カテゴリRHSから左辺値LHSへ代入可能であることを表すコンセプトである
    {
        std::cout << "assignable_from";
        r<int&, short>("int&", "short");
        r<std::vector<int>&, std::vector<int>>("std::vector<int>&", "std::vector<short>");
        r<std::unique_ptr<int>&, std::unique_ptr<int>>("std::unique_ptr<int>&", "std::unique_ptr<int>");
        r<S3&, S3&>("S3&", "S3&");

        std::cout << "\n";

        r<std::unique_ptr<int>&, std::unique_ptr<int>&>("std::unique_ptr<int>&", "std::unique_ptr<short>&");
        r<S3&, S3>("S3&", "S3");
    }
    //swappable,swappable_with:指定された型TもしくはT, Uのオブジェクト間で、その値の交換操作（swap操作）が可能であることを表すコンセプトである
    {
        std::cout << "swappable";
        s<int>("int");
        s<int&&>("int&&");
        s<NS::swappable1>("NS::swappable1");
        s<NS::swappable2>("NS::swappable2");
        s<int[5]>("int[5]");

        std::cout << "\n";

        s<const int>("const int");
        s<not_swappable>("not_swappable");
    }
    //equality_comparable:指定された型TもしくはT, Uの間で==演算子による同値比較が可能である事を表すコンセプトである。
    {
        std::cout << "equality_comparable";
        u<int>("int");
        u<eq_comp>("eq_comp");

        std::cout << "\n";

        u<std::any>("std::any");
        u<not_eq_comp>("not_eq_comp");
    }
    //totally_ordered:指定された型TもしくはT, Uの間で< <= > >=の演算子による比較が可能であり、その順序付けが全順序の要件を満たしている事を表すコンセプトである
    {
        std::cout << "totally_ordered";
        v<int>("int");
        v<double>("double");
        v<ordered>("ordered");

        std::cout << "\n";

        v<not_ordered>("not_ordered");
    }
    //movable:任意の型Tがオブジェクト型かつムーブ構築・代入が可能であることを表すコンセプトである
    {
        std::cout << "movable";
        w<int>("int");
        w<double>("double");
        w<std::nullptr_t>("std::nullptr_t");
        w<std::size_t>("std::size_t");
        w<movable>("movable");

        std::cout << "\n";
        w<void>("void");
        w<not_movable1>("not_movable1");
        w<not_movable2>("not_movable2");
    }
    //copyable:任意の型Tがmovableコンセプトを満たし、それに加えてコピー構築・代入が可能であることを表すコンセプトである
    {
        std::cout << "copyable";
        x<int>("int");
        x<double>("double");
        x<std::nullptr_t>("std::nullptr_t");
        x<std::size_t>("std::size_t");
        x<copyable>("copyable");

        std::cout << "\n";
        x<void>("void");
        x<movable>("movable");
        x<not_copyable1>("not_copyable1");
        x<not_copyable2>("not_copyable2");
    }
    //semiregular:任意の型Tがcopyableコンセプトを満たし、それに加えてデフォルト構築可能であることを表すコンセプトである
    {
        std::cout << "semiregular";
        y<int>("int");
        y<double>("double");
        y<std::nullptr_t>("std::nullptr_t");
        y<std::size_t>("std::size_t");
        y<semiregular>("semiregular");

        std::cout << "\n";
        y<void>("void");
        y<copyable>("copyable");
    }
    //regular:任意の型Tがsemiregularコンセプトを満たし、それに加えて等値比較可能であることを表すコンセプトである
    {
        std::cout << "regular";
        z<int>("int");
        z<double>("double");
        z<std::nullptr_t>("std::nullptr_t");
        z<std::size_t>("std::size_t");
        z<regular>("regular");

        std::cout << "\n";
        z<void>("void");
        z<semiregular>("semiregular");
    }
    //invocable:任意の関数呼び出し可能な型Fが引数Args...によって関数呼び出し可能であることを表すコンセプトである
    {
        std::cout << "invocable";
        az<decltype(func), int>("func(int)");
        az<decltype(lambda), int>("lambda(int)");
        az<decltype(lambda), int*>("lambda(int*)");
        az<invocable, int>("invocable(int)");
        az<invocable, int***>("invocable(int***)");

        // 内部状態を保ち、等しさを保持しない呼び出し可能な型
        az<decltype(mut_lambda), int>("mut_lambda(int)");
        az<std::mt19937>("std::mt19937()");

        std::cout << "\n";

        az<decltype(func), int*>("func(int*)");
        az<not_invocable>("not_invocable()");
        az<not_invocable, int>("not_invocable(int)");
    }
    //predicate:任意の関数呼び出し可能な型Fが引数Args...によって関数呼び出し可能（regular_invocable）であり、その戻り値型がboolに変換可能であることを表すコンセプトである
    {
        std::cout << "predicate";
        ax<decltype(func1), int>("func1(int)");
        ax<decltype(func2), int>("func2(int)");
        ax<decltype(func3), int>("func3(int)");
        ax<decltype(lambda), int>("lambda(int)");
        ax<std::mt19937>("std::mt19937()");
        ax<predicate, int>("predicate(int)");

        std::cout << "\n";
        ax<not_predicate, int>("not_predicate(int)");
    }

    std::cin.get();
}


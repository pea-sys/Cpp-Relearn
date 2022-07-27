
#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>
#include <vector>
#include <cassert>
#include <map>

template <class T, class Alloc>
class MyVector {
    T* data_;
    std::size_t size_;
    Alloc alloc_;
public:
    MyVector(std::size_t size)
    {
        using traits = std::allocator_traits<Alloc>;

        size_ = size;

        // メモリを確保
        data_ = traits::allocate(alloc_, size);

        // 要素を構築
        for (std::size_t i = 0; i < size_; ++i) {
            traits::construct(alloc_, &data_[i]);
        }
    }

    ~MyVector()
    {
        using traits = std::allocator_traits<Alloc>;

        // 要素を破棄
        for (std::size_t i = 0; i < size_; ++i) {
            traits::destroy(alloc_, &data_[i]);
        }

        // メモリを解放
        traits::deallocate(alloc_, data_, size_);
    }
};

template <class T, class Allocator = std::allocator<T>>
struct X {
    T x_;
    Allocator alloc_;
public:
    using allocator_type = Allocator;

    X(std::allocator_arg_t, Allocator alloc, T x)
        : alloc_(alloc), x_(x) {}
};

// 偽アロケータ
struct MyAlloc {};

// アロケータを使用しない偽コンテナ
struct MyContainer0 {
    MyContainer0(int) noexcept {}
};

// 偽アロケータを使用する偽コンテナ（allocator_arg_t 使用）
struct MyContainer1 {
    using allocator_type = MyAlloc;
    MyContainer1(std::allocator_arg_t, const MyAlloc&, int) noexcept {}
};

// 偽アロケータを使用する偽コンテナ（最後の引数）
struct MyContainer2 {
    using allocator_type = MyAlloc;
    MyContainer2(int, const MyAlloc&) noexcept {}
};

// 偽アロケータ用挿入演算子
std::ostream& operator<<(std::ostream& os, const MyAlloc&)
{
    return os << "MyAlloc";
}

// allocator_arg 用挿入演算子
std::ostream& operator<<(std::ostream& os, const std::allocator_arg_t&)
{
    return os << "allocator_arg_t";
}

// piecewise_construct 用挿入演算子
std::ostream& operator<<(std::ostream& os, const std::piecewise_construct_t&)
{
    return os << "piecewise_construct_t";
}

// tuple 用挿入演算子
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t)
{
    os << "tuple(";
    std::apply([&os](const auto&... args) {
        ((os << args << ", "), ...);
        }, t);
    return os << ')';
}


// 状態付きアロケータ
template <typename T>
class MyAllocB {
public:
    using value_type = T;
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(sizeof(T) * n)); }
    void deallocate(T* p, std::size_t n) { ::operator delete(static_cast<void*>(p), sizeof(T) * n); }
    MyAllocB(int state) noexcept : state(state) {}
    template <typename U>
    MyAllocB(const MyAllocB<U>& o) noexcept : state(o.state) {}
    int state;
};

template <typename T>
bool operator==(const MyAllocB<T>& lhs, const MyAllocB<T>& rhs) noexcept
{
    return lhs.state == rhs.state;
}

template <typename T>
bool operator!=(const MyAllocB<T>& lhs, const MyAllocB<T>& rhs) noexcept
{
    return lhs.state != rhs.state;
}

// 型別名
using VEC = std::vector<int, MyAllocB<int>>;

using V = std::pair<std::pair<VEC, int>, int>;

struct Vector {
    int x, y;
};

struct Y : public std::enable_shared_from_this<Y> {
    std::shared_ptr<Y> d()
    {
        // thisを指すshared_ptrオブジェクトを作る
        return shared_from_this();
    }
};

template <class Key, class Value>
using shared_ptr_map = std::map<
    std::shared_ptr<Key>,
    Value,
    std::owner_less<std::shared_ptr<Key>>
>;
struct Z {
    int i;
    int j;
};

struct hoge {
    hoge() { std::cout << "hoge::hoge()" << std::endl; }
    ~hoge() { std::cout << "hoge::~hoge()" << std::endl; }
};

struct useless_type {};

// operator&がオーバーロードされたクラス
class nonaddressable {
public:
    useless_type operator&() const { return useless_type(); }
};

// 例えば翻訳単位が異なる、あるいは他人の作成した仕様はドキュメントでわかるが
// ソースコードは不明な実行時にリンクするどこかのライブラリがあるとしよう。
namespace external_library
{
    static char* memory_pool = nullptr;

    // そのライブラリの中で仕様上は領域全体としては 16-byte アライメントされた
    // 4 つ連続した float 型の領域を return してくれる関数があったとしよう。
    float* get_data()
    {
        if (memory_pool == nullptr)
            memory_pool = new char[32];

        static_assert(sizeof(decltype(memory_pool)) == 8);

        // 特に、このような実装でアライメントの仕様を満たしていた場合には
        // 翻訳単位が異なれば、呼び出し元を翻訳するコンパイラはそれを知るのは難しい。
        auto data = (float*)(((std::uint64_t)memory_pool + 0x0f) >> 4 << 4);

        for (auto n = 0; n < 4; ++n)
            data[n] = n;

        return data;
    }
}
int main()
{
	
	//allocator_traitsは、アロケータクラスの機能に間接的にアクセスするためのインタフェースを提供するクラスである
    {
        std::cout << "allocator_traits" << std::endl;
        MyVector<int, std::allocator<int>> v(3);
    }
    //uses_allocator:型 T が Alloc 型のアロケータオブジェクトを用いた uses-allocator 構築をする際に、実際にアロケータオブジェクトを使用するかを調べる
    {
        std::cout << "uses_allocator" << std::endl;
        const bool result = std::uses_allocator<X<int>, std::allocator<int>>::value;
        static_assert(result, "should be true");
    }
    //Alloc 型のアロケータオブジェクト alloc を使用した T 型オブジェクトの uses-allocator 構築のために必要なコンストラクタ引数を、tuple 型にして返す
    {
        std::cout << "uses_allocator_construction_args" << std::endl;
        auto a0 = std::uses_allocator_construction_args<MyContainer0>(MyAlloc{}, 0);
        std::cout << a0 << '\n';
        auto a1 = std::uses_allocator_construction_args<MyContainer1>(MyAlloc{}, 1);
        std::cout << a1 << '\n';
        auto a2 = std::uses_allocator_construction_args<MyContainer2>(MyAlloc{}, 2);
        std::cout << a2 << '\n';
        auto a3 = std::uses_allocator_construction_args<std::pair<MyContainer1, MyContainer2>>(MyAlloc{}, 3, 4);
        std::cout << a3 << '\n';
    }
    //make_obj_using_allocatorAlloc 型のアロケータオブジェクト alloc、および、コンストラクタ引数 args を用いて、T 型のオブジェクトを uses-allocator 構築する
    {
        std::cout << "make_obj_using_allocatorAlloc" << std::endl;
        auto v = std::make_obj_using_allocator<V>(
            MyAllocB<int>{42}, std::make_pair(VEC{ MyAllocB<int>(99) }, 1), 2
            );
        std::cout << v.first.first.get_allocator().state << '\n';
    }
    //uninitialized_construct_using_allocator:Alloc 型のアロケータオブジェクト alloc、および、コンストラクタ引数 args を用いて、p で指定された領域に T 型のオブジェクトを uses-allocator 構築する
    {
        std::cout << "uninitialized_construct_using_allocator" << std::endl;
        alignas(V) char s[sizeof(V)];
        auto p = std::uninitialized_construct_using_allocator(
            reinterpret_cast<V*>(s),
            MyAllocB<int>{42}, std::make_pair(VEC{ MyAllocB<int>(99) }, 1), 2
        );
        std::cout << p->first.first.get_allocator().state << '\n';
    }
    //uninitialized_default_construct:未初期化領域の範囲の各要素をデフォルト構築する
    {
        std::cout << "uninitialized_default_construct" << std::endl;
        std::allocator<Vector> alloc;

        // メモリ確保。
        // この段階では、[p, p + size)の領域は未初期化
        const std::size_t size = 3;
        Vector* p = alloc.allocate(size);

        // 未初期化領域[p, p + size)をデフォルト構築
        std::uninitialized_default_construct(p, p + size);

        // 各要素を出力
        // (値構築すると各値がゼロ初期化されるが、デフォルト構築ではゼロ初期化されない)
        for (std::size_t i = 0; i < size; ++i) {
            const Vector& v = *(p + i);
            std::cout << v.x << ',' << v.y << std::endl;
        }

        // オブジェクトを破棄
        std::destroy(p, p + size);

        // メモリ解放
        alloc.deallocate(p, size);
    }
    //uninitialized_default_construct_n:未初期化領域の範囲のうち、先頭N個の要素をデフォルト構築する
    {
        std::cout << "uninitialized_default_construct_n" << std::endl;
        std::allocator<Vector> alloc;

        // メモリ確保。
        // この段階では、[p, p + size)の領域は未初期化
        const std::size_t size = 3;
        Vector* p = alloc.allocate(size);

        // 未初期化領域[p, p + size)をデフォルト構築
        std::uninitialized_default_construct_n(p, size);

        // 各要素を出力
        // (値構築すると各値がゼロ初期化されるが、デフォルト構築ではゼロ初期化されない)
        for (std::size_t i = 0; i < size; ++i) {
            const Vector& v = *(p + i);
            std::cout << v.x << ',' << v.y << std::endl;
        }

        // オブジェクトを破棄
        std::destroy(p, p + size);

        // メモリ解放
        alloc.deallocate(p, size);
    }
    //uninitialized_value_construct:未初期化領域の範囲の各要素を値構築する
    {
        std::cout << "uninitialized_value_construct" << std::endl;
        std::allocator<Vector> alloc;

        // メモリ確保。
        // この段階では、[p, p + size)の領域は未初期化
        const std::size_t size = 3;
        Vector* p = alloc.allocate(size);

        // 未初期化領域[p, p + size)を値構築
        std::uninitialized_value_construct(p, p + size);

        // 各要素を出力
        for (std::size_t i = 0; i < size; ++i) {
            const Vector& v = *(p + i);
            std::cout << v.x << ',' << v.y << std::endl;
        }

        // オブジェクトを破棄
        std::destroy(p, p + size);

        // メモリ解放
        alloc.deallocate(p, size);
    }
    //uninitialized_value_construct_n:未初期化領域の範囲のうち、先頭N個の要素を値構築する
    {
        std::cout << "uninitialized_value_construct_n" << std::endl;
        std::allocator<Vector> alloc;

        // メモリ確保。
        // この段階では、[p, p + size)の領域は未初期化
        const std::size_t size = 3;
        Vector* p = alloc.allocate(size);

        // 未初期化領域[p, p + size)を値構築
        std::uninitialized_value_construct_n(p, size);

        // 各要素を出力
        for (std::size_t i = 0; i < size; ++i) {
            const Vector& v = *(p + i);
            std::cout << v.x << ',' << v.y << std::endl;
        }

        // オブジェクトを破棄
        std::destroy(p, p + size);

        // メモリ解放
        alloc.deallocate(p, size);
    }
    //shared_ptrは、指定されたリソースへの所有権(ownership)を共有(share)するスマートポインタであ
    {
        // newしたポインタをshared_ptrオブジェクトに管理させる
        // 所有者は1人。
        std::shared_ptr<int> p1(new int(3));

        {
            // shared_ptrオブジェクトをコピーすることで、
            // 複数のオブジェクトが一つのリソースを共有できる。
            // 所有者が2人になる。
            std::shared_ptr<int> p2 = p1;

            // 共有しているリソースにアクセスする
            std::cout << *p2 << std::endl;
        } // p2のデストラクタが実行される。
          // リソースの所有者が1人になる。
          // ここではまだ、リソースは解放されない。

        std::cout << *p1 << std::endl;
    } // p1のデストラクタが実行される。
      // リソースの所有者が0人になる。
      // 誰もリソースを参照しなくなったので、リソースが解放される。
    
    //make_shared:shared_ptr オブジェクトを構築する
    {
        std::cout << "make_shared:shared_ptr" << std::endl;
        std::shared_ptr<int> sp = std::make_shared<int>(42);
        if (sp) {
            std::cout << *sp << std::endl;
        }
    }
    //make_shared_for_overwrite:デフォルトの初期値を持つT型のオブジェクト、またはその配列へのshared_ptrを構築し、返却する
    {
        std::cout << "make_shared_for_overwrite" << std::endl;
        std::shared_ptr<int> sp = std::make_shared_for_overwrite<int>();
        if (sp) {
            std::cout << *sp << std::endl;
        }
    }
    //allocate_shared:class T に対する shared_ptr<T>オブジェクト を作成し返却する
    {
        std::cout << "allocate_shared" << std::endl;
        std::allocator<int> alloc;
        std::shared_ptr<int> sp = std::allocate_shared<int>(alloc, 42);
        if (sp) {
            std::cout << *sp << std::endl;
        }
    }
    //allocate_shared_for_overwrite:デフォルトの初期値を持つT型のオブジェクト、またはその配列へのshared_ptrを構築し、返却する。
    {
        std::cout << "allocate_shared_for_overwrite" << std::endl;
        std::allocator<int> alloc;
        std::shared_ptr<int> sp = std::allocate_shared_for_overwrite<int>(alloc);
        if (sp) {
            std::cout << *sp << std::endl;
        }
    }
    //enable_shared_from_thisは、shared_ptrで管理しているオブジェクトのthisポインタを、shared_ptrとして扱うことを可能にするためのクラスである
    {
        std::cout << "enable_shared_from_this" << std::endl;
        std::shared_ptr<Y> p(new Y());
        std::shared_ptr<Y> q = p->d();

        assert(p == q);
    }
    //weak_ptrは、shared_ptrオブジェクトが持つリソースへの弱参照を保持するクラスである
    {
        std::cout << "weak_ptr" << std::endl;
        // weak_ptrオブジェクトwpは、
        // shared_ptrオブジェクトspを監視する
        std::shared_ptr<int> sp(new int(42));
        std::weak_ptr<int> wp = sp;

        // wpの監視対象であるspが、有効なリソースを保持している状態なら処理する。
        if (std::shared_ptr<int> r = wp.lock()) {
            std::cout << "get weak_ptr value : " << *r << std::endl;
        }

        sp.reset();

        // shared_ptrオブジェクトが無効になったことを検知する
        if (wp.expired()) {
            std::cout << "shared_ptr managed object deleted." << std::endl;
        }
    }

    //bad_weak_ptrは、weak_ptrオブジェクトから破棄済みのshared_ptrオブジェクトを構築しようとした場合に発生する例外クラスである
    {
        std::cout << "bad_weak_ptr" << std::endl;
        auto sp = std::make_shared<int>(42);
        std::weak_ptr<int> wp(sp);

        sp.reset();
        try {
            std::shared_ptr<int> i(wp);
        }
        catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
    }
    //owner_lessは、スマートポインタを所有権ベースで小なり比較するための、2項関数オブジェクトである
    {
        std::cout << "owner_less" << std::endl;
        shared_ptr_map<int, std::string> m;

        std::shared_ptr<int> p1(new int(2));

        std::shared_ptr<Z> px(new Z());
        // 同じ所有権(px)を持つが、異なるポインタを指すp2とp3
        std::shared_ptr<int> p2(px, &(px->i));
        std::shared_ptr<int> p3(px, &(px->j));

        m[p1] = "Alice";
        m[p2] = "Bob";
        m[p3] = "Carol"; // owner_lessでは、p2とp3が同じリソースを
                         // 指していると見なされるので、
                         // p2の要素が上書きされる

        std::cout << m.at(p1) << std::endl;
        std::cout << m.at(p2) << std::endl;
        std::cout << m.at(p3) << std::endl;
    }
    //unique_ptrは指定されたリソースへのポインタの所有権(ownership)を唯一(unique)持っているように振舞うスマートポインタである
    {
        std::cout << "unique_ptr" << std::endl;
        std::unique_ptr<hoge> p0(new hoge());

        // hogeオブジェクトの所有権をp0からp1に移動
        // p0は何も所有していない状態になる
        std::unique_ptr<hoge> p1(std::move(p0));

        if (p0) {
            std::abort(); // p0は空
        }

        // p1が所有しているリソースが解放される
    }
    //make_unique:unique_ptrオブジェクトを構築する
    {
        std::cout << "make_unique" << std::endl;
        // (1)
        // 型Tのコンストラクタ引数を受け取ってunique_ptrオブジェクトを構築。
        //
        // ここでは、型std::pair<First, Second>のunique_ptrオブジェクトを構築するために、
        // First型とSecond型の引数を渡している。
        std::unique_ptr<std::pair<int, int>> p1 = std::make_unique<std::pair<int, int>>(3, 1);
        std::cout << p1->first << ':' << p1->second << std::endl;

        // (2)
        // 型T[]の要素数を受け取ってunique_ptr<T[]>オブジェクトを構築。
        //
        // ここでは、要素数3の、int型動的配列を構築している。
        std::unique_ptr<int[]> p2 = std::make_unique<int[]>(3);
        p2[0] = 1;
        p2[1] = 2;
        p2[3] = 3;
    }
    //make_unique_for_overwrite:unique_ptrオブジェクトを構築する。その際、型Tのオブジェクトはデフォルト構築される
    {
        std::cout << "make_unique_for_overwrite" << std::endl;
        std::unique_ptr<std::pair<int, int>> p1 = std::make_unique_for_overwrite<std::pair<int, int>>();
        std::cout << p1->first << ':' << p1->second << std::endl;
    }
    //default_deleteは、リソースを自動的に解放するスマートポインタクラスであるunique_ptrにおいて、デフォルトで使用されるデリータクラスである。
    {
        std::cout << "default_delete" << std::endl;
        {
            std::unique_ptr<int, std::default_delete<int>> p(new int());
            *p = 3;
        } // pが指しているintオブジェクトがdeleteされる

        {
            std::unique_ptr<int[], std::default_delete<int[]>> p(new int[3]);

            for (std::size_t i = 0; i < 3; ++i) {
                p[i] = i;
            }
        } // pが指しているint配列がdelete[]される
    }
    //pointer_traits
    {
        std::cout << "pointer_traits" << std::endl;
        // スマートポインタの要素型を取得する
        using smart_ptr_element = std::pointer_traits<std::shared_ptr<int>>::element_type;
        static_assert(std::is_same<smart_ptr_element, int>::value, "element type is int");

        // ポインタの要素型を取得する
        using ptr_element = std::pointer_traits<int*>::element_type;
        static_assert(std::is_same<ptr_element, int>::value, "element type is int");
    }
    //to_address:ポインタと見なせるオブジェクトからアドレスを取得する
    {
        std::cout << "to_address" << std::endl;
        int x = 3;
        int* p = &x;

        // ポインタからアドレスを取得する。
        // なにもせず、渡したものが返る
        int* result1 = std::to_address(p);
        std::cout << *result1 << std::endl;

        // スマートポインタからアドレスを取得する。
        // スマートポインタが所有権管理しているポインタが返る
        std::shared_ptr<int> sp{ new int(1) };
        int* result2 = std::to_address(sp);
        std::cout << *result2 << std::endl;
    }
    //addressof:変数のアドレスを必ず取得する
    {
        std::cout << "addressof" << std::endl;
        {
            int x = 3;
            int* p1 = std::addressof(x); // OK : アドレス取得できる
            int* p2 = &x; // OK : アドレス取得できる
        }
        {
            nonaddressable x;
            nonaddressable* p1 = std::addressof(x); // OK : アドレス取得できる
        //  nonaddressable* p2 = &x; // エラー！アドレス取得できない
        }
    }
    //align:アライメント調整された領域を得る
    {
        std::cout << "align" << std::endl;
        char buffer[256];
        void* ptr = buffer;
        std::size_t space = sizeof(buffer) - 1;

        // intアライメントで、intをN個確保。
        std::size_t N = 3;
        if (std::align(alignof(int), sizeof(int) * N, ptr, space)) {
            std::cout << ptr << " " << space << std::endl;
        }
    }
    //assume_aligned:特定のメモリ領域のアライメント情報をプログラマが明示的にコンパイラへ与えコンパイラが最適化のヒントとして活用できるようにする
    //難しすぎてスキップ

    std::cin.get();
}

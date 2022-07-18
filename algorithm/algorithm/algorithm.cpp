#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <random>

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
void print(const std::unique_ptr<int>& v) {
    if (v) std::cout << *v << " ";
    else   std::cout << "(null)" << " ";
}
void print_vector(const char* tag, const std::vector<int>& v) {
    std::cout << tag << " : ";
    bool first = true;
    for (int x : v) {
        if (first) {
            first = false;
        }
        else {
            std::cout << ',';
        }
        std::cout << x;
    }
    std::cout << std::endl;
}
bool is_even(int x) { return x % 2 == 0; }
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


#pragma region シーケンスを変更する操作

    // 指定された範囲の要素をコピーする
    std::vector<int> v3(v.size()); // ちゃんと確保しておくこと
    std::copy(v.begin(), v.end(), v3.begin());
    std::cout << "copy : ";
    std::copy(v3.begin(), v3.end(), std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    // 指定された数の要素をコピーする
    std::cout << "copy_n : ";
    std::copy_n(v.begin(), 5, std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    // 条件を満たす要素のみをコピーする
    std::cout << "copy_if : ";
    std::copy_if(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ","), [](int x) { return x < 3; });
    std::cout << std::endl;

    // 指定された範囲の要素を後ろからコピーする
    std::cout << "copy_backward : ";
    std::list<int> ls = { 1,2,3,4,5 }; // BidirectionalIterator
    std::copy_backward(ls.begin(), std::next(ls.begin(), 3), ls.end());
    std::copy(ls.begin(), ls.end(), std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    // 指定された範囲の要素をムーブする
    std::vector<std::unique_ptr<int>> v7;
    for (int i = 0; i < 5; i++)
        v7.emplace_back(new int(i));

    std::vector<std::unique_ptr<int>> v6;
    // v のそれぞれの要素を v2 へムーブする
    std::move(v7.begin(), v7.end(), std::back_inserter(v6));
    std::cout << "move : ";
    std::for_each(v6.begin(), v6.end(),
        [](const std::unique_ptr<int>& v) { std::cout << *v; });
    std::cout << std::endl;


    // 指定された範囲の要素を後ろからムーブする
    {
        std::cout << "move_backward : ";
        std::vector<std::unique_ptr<int>> v;

        for (int i = 0; i < 5; i++)
            v.emplace_back(new int(i));

        // 0,1,2 の値がある範囲を、2,3,4 の値がある範囲へムーブする
        std::move_backward(v.begin(), v.begin() + 3, v.end());

        std::for_each(v.begin(), v.end(), &print);
        std::cout << std::endl;
    }

    //指定された2つの範囲同士を swap する
    {
        std::cout << "swap_ranges : ";
        std::vector<int> v1 = { 3,1,2 };
        std::vector<int> v2 = { 5,2,4,1,3 };

        std::swap_ranges(v1.begin(), v1.end(), v2.begin());

        std::cout << "v1: ";
        std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;

        std::cout << "v2: ";
        std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;
    }

    //2つのイテレータの要素を入れ替える
    {
        std::cout << "iter_swap : ";
        std::vector<int> v1 = { 3,1,4 };
        std::vector<int> v2 = { 4,2,5 };

        std::iter_swap(v1.begin() + 1, v2.begin() + 2);

        std::cout << "v1: ";
        std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;

        std::cout << "v2: ";
        std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;
    }
    {
        std::cout << "transform : ";
        std::vector<int> v = { 3,1,4 };
        std::vector<std::string> result;

        // 2倍してから文字列に変換する
        std::transform(v.begin(), v.end(), std::back_inserter(result),
            [](int x) { return std::to_string(x * 2); });

        std::for_each(result.begin(), result.end(),
            [](const std::string& s) { std::cout << s << ","; });
        std::cout << std::endl;
    }
    //	指定された要素を除く
    {
        std::cout << "replace : ";
        std::vector<int> v = { 3,1,2,1,2 };

        // 1 の要素を全部 10 に置き換える
        std::replace(v.begin(), v.end(), 1, 10);

        std::for_each(v.begin(), v.end(),
            [](int x) { std::cout << x << ","; });
        std::cout << std::endl;
    }
    //	条件を満たす要素を除ける
    {
        std::cout << "replace_if : ";
        std::vector<int> v = { 3,1,2,1,2 };

        // 奇数の要素を全部 10 に置き換える
        std::replace_if(v.begin(), v.end(),
            [](int x) { return x % 2 != 0; }, 10);

        std::for_each(v.begin(), v.end(),
            [](int x) { std::cout << x << ","; });
        std::cout << std::endl;
    }
    //指定された値を一致する要素を指定された値に置き換え、その結果を出力の範囲へコピーする
    {
        std::cout << "replace_copy : ";
        std::vector<int> v = { 3,1,2,1,2 };

        // 1 の要素を全部 10 に置き換えたものを出力する
        std::replace_copy(v.begin(), v.end(),
            std::ostream_iterator<int>(std::cout, ","), 1, 10);
        std::cout << std::endl;
    }
    //条件を満たす要素を指定された値に置き換え、その結果を出力の範囲へコピーする
    {
        std::cout << "replace_copy_if : ";
        std::vector<int> v = { 3,1,2,1,2 };

        // 奇数の要素を全部 10 に置き換えたものを出力する
        std::replace_copy_if(v.begin(), v.end(),
            std::ostream_iterator<int>(std::cout, ","),
            [](int x) { return x % 2 != 0; }, 10);
        std::cout << std::endl;
    }
    //指定された値で出力の範囲に書き込む
    {
        std::cout << "fill : ";
        std::vector<int> v(5);

        // v を 3 の値で埋める
        std::fill(v.begin(), v.end(), 3);

        std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ","; });
        std::cout << std::endl;
    }
    // 指定された値で出力の範囲に n 個を書き込む
    {
        std::cout << "fill_n : ";
        // 3 を出力
        std::fill_n(std::ostream_iterator<int>(std::cout, ","), 10, 3);
        std::cout << std::endl;
    }
    //出力の範囲へ関数の結果を書き込む
    {
        std::cout << "generate : ";
        std::vector<int> v(10);

        // 2 の累乗の値を生成する
        int n = 1;
        std::generate(v.begin(), v.end(), [&n]() { auto t = n; n *= 2; return t; });

        std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << ","; });
        std::cout << std::endl;
    }
    //出力の範囲へ関数の結果を n 個書き込む
    {
        std::cout << "generate_n : ";
        // 2 の累乗の値を生成して出力する
        int n = 1;
        std::generate_n(std::ostream_iterator<int>(std::cout, ","),
            10, [&n] { auto t = n; n *= 2; return t; });
        std::cout << std::endl;
    }
    //指定された要素を取り除く
    {
        std::cout << "remove : ";
        std::vector<int> v = { 2,3,1,2,1 };

        auto result = std::remove(v.begin(), v.end(), 1);

        // [v.begin(),result) の範囲に 1 を除去した結果が入っている
        std::for_each(v.begin(), result, [](int x) { std::cout << x << ","; });
        std::cout << std::endl;

        // remove を使ってもコンテナの要素数は変わらないことに注意しよう
        std::cout << "size before: " << v.size() << std::endl;

        // [v.begin(),result) の範囲に 1 を除去した結果が入っているので、
        // [result,v.end()) を erase することでサイズも変更することができる
        // （Erase-remove イディオム）
        v.erase(result, v.end());
        std::cout << "size after: " << v.size() << std::endl;
        std::cout << std::endl;
    }
    //条件を満たす要素を除ける
    {
        std::cout << "remove_if : ";
        std::vector<int> v = { 2,3,1,2,1 };

        auto result = std::remove_if(v.begin(), v.end(),
            [](int x) { return x % 2 != 0; });

        // [v.begin(),result) の範囲に奇数を除去した結果が入っている
        std::for_each(v.begin(), result, [](int x) { std::cout << x << ","; });
        std::cout << std::endl;

        // remove を使ってもコンテナの要素数は変わらないことに注意しよう
        std::cout << "size before: " << v.size() << std::endl;

        // [v.begin(),result) の範囲に奇数を除去した結果が入っているので、
        // [result,v.end()) を erase することでサイズも変更することができる
        // （Erase-remove イディオム）
        v.erase(result, v.end());
        std::cout << "size after: " << v.size() << std::endl;
    }
    //指定された要素を除け、その結果を出力の範囲へコピーする
    {
        std::cout << "remove_copy : ";
        std::vector<int> v = { 2,3,1,2,1 };

        // 1 を除去した結果を出力する
        std::remove_copy(v.begin(), v.end(),
            std::ostream_iterator<int>(std::cout, ","), 1);
        std::cout << std::endl;
    }
    //条件を満たす要素を除け、その結果を出力の範囲へコピーする
    {
        std::cout << "remove_copy_if : ";
        std::vector<int> v = { 2,3,1,2,1 };

        // 奇数を除去した結果を出力する
        std::remove_copy_if(v.begin(), v.end(),
            std::ostream_iterator<int>(std::cout, ","),
            [](int x) { return x % 2 != 0; });
        std::cout << std::endl;
    }
    //重複した要素を除ける。
    {
        std::cout << "unique : ";
        // 入力の配列がソート済みではない場合、
        // 隣り合った重複要素が取り除かれる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };

            decltype(v)::iterator result = std::unique(v.begin(), v.end());

            // [v.begin(), result)の範囲に、重複を除いた結果が入っている。
            // 不要になった要素を削除
            v.erase(result, v.end());

            print_vector("unsorted unique", v);
        }

        // 入力の配列がソート済みである場合、
        // 重複している全ての要素が取り除かれて一意になる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };

            std::sort(v.begin(), v.end());
            decltype(v)::iterator result = std::unique(v.begin(), v.end());

            // 不要になった要素を削除
            v.erase(result, v.end());

            print_vector("sorted unique", v);
        }
    }
    //隣り合った重複要素を取り除き、その結果を出力の範囲へコピーする
    {
        std::cout << "unique_copy : ";
        // 入力の配列がソート済みではない場合、
        // 隣り合った重複要素が取り除かれる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };
            std::vector<int> uniqued;

            // 重複を除いた要素がuniquedに追加されていく
            std::unique_copy(v.begin(), v.end(), std::back_inserter(uniqued));

            print_vector("unsorted unique", uniqued);
        }

        // 入力の配列がソート済みである場合、
        // 重複している全ての要素が取り除かれて一意になる
        {
            std::vector<int> v = { 2,5,3,3,1,2,4,2,1,1,4,4,3,3,3 };
            std::vector<int> uniqued;

            std::sort(v.begin(), v.end());
            std::unique_copy(v.begin(), v.end(), std::back_inserter(uniqued));

            print_vector("sorted unique", uniqued);
        }
    }
    //要素の並びを逆にする
    {
        std::cout << "reverse : ";
        std::string str = "reverse";

        std::reverse(str.begin(), str.end());
        std::cout << str << std::endl;
    }
    //要素の並びを逆にし、その結果を出力の範囲へコピーする
    {
        std::cout << "reverse_copy : ";
        std::string str = "reverse";

        std::reverse_copy(str.begin(), str.end(),
            std::ostream_iterator<char>(std::cout, ""));
    }
    //middleの要素が先頭、middle-1の要素が末尾となるように、[first,last)の要素の並びを回転させる
    {
        std::cout << "rotate : ";
        std::string str = "rotate";

        std::rotate(str.begin(), str.begin() + 2, str.end());
        std::cout << str << std::endl;
    }
    //middleの要素が先頭、middle-1の要素が末尾となるように、[first,last)の要素の並びを回転させ、その結果を出力の範囲へコピーする
    {
        std::cout << "rotate_copy : ";
        std::string str = "rotate";
        std::string result;

        std::rotate_copy(str.begin(), str.begin() + 2, str.end(),
            std::back_inserter(result));

        std::cout << result << std::endl;
    }
    //範囲の要素をn個だけ左にシフトさせる
    {
        std::cout << "shift_left : ";
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        std::shift_left(v.begin(), v.end(), 2);

        for (int x : v) {
            std::cout << x << ',';
        }
        std::cout << std::endl;
    }
    //範囲の要素をn個だけ右にシフトさせる
    {
        std::cout << "shift_right : ";
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        std::shift_right(v.begin(), v.end(), 2);

        for (int x : v) {
            std::cout << x << ',';
        }
        std::cout << std::endl;
    }
    //範囲から指定された個数の要素をランダムに抽出する
    {
        std::cout << "sample : ";
        // 乱数生成器を用意する
        std::random_device seed_gen;
        std::mt19937 engine{ seed_gen() };

        // 文字列中から3文字をランダム抽出する
        {
            const std::string input = "abcdef";
            const int n = 3;

            std::string result;
            std::sample(input.begin(),
                input.end(),
                std::back_inserter(result),
                n,
                engine);
            std::cout << result << std::endl;
        }

        // 配列から3要素をランダム抽出する
        {
            const std::vector<int> input = { 0, 1, 2, 3, 4, 5 };
            const int n = 3;

            std::vector<int> result;
            std::sample(input.begin(),
                input.end(),
                std::back_inserter(result),
                n,
                engine);

            for (int x : result) {
                std::cout << x;
            }
            std::cout << std::endl;
        }
    }
    //[first,last) のそれぞれの要素を同じ確率で並び替える
    {
        std::cout << "shuffle : ";
        std::string input = "0123456789abcdef";

        std::cout << "before: " << input << std::endl;

        // シャッフル
        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::shuffle(input.begin(), input.end(), engine);

        std::cout << " after: " << input << std::endl;
    }
    //与えられた範囲が条件によって区分化されているか判定する
    {
        std::cout << "partition: ";
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        auto pred = [](int x) { return x % 2 == 0; };

        // 偶数グループと奇数グループに分ける
        std::partition(v.begin(), v.end(), pred);

        std::for_each(v.begin(), v.end(), [](int x) {
            std::cout << x ;
            });

        // 偶数グループと奇数グループに分かれているか
        if (std::is_partitioned(v.begin(), v.end(), pred)) {
            std::cout << "partitioned" ;
        }
        else {
            std::cout << "not partitioned" ;
        }
        std::cout << std::endl;
    }
    //与えられた範囲を相対順序を保ちながら条件によって区分化する
    {
        std::cout << "stable_partition: ";
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        // 偶数グループと奇数グループに分ける
        std::stable_partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

        std::for_each(v.begin(), v.end(), [](int x) {
            std::cout << x;
            });
        std::cout  << std::endl;
    }
    
    //与えられた範囲を条件によって 2 つの出力の範囲へ分けてコピーする
    {
        std::cout << "partition_copy: ";
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        // 偶数グループと奇数グループに分ける
        std::vector<int> evens;
        std::vector<int> odds;
        std::partition_copy(v.begin(), v.end(),
            std::back_inserter(evens),
            std::back_inserter(odds),
            is_even);

        print_vector("v", v);
        print_vector("evens", evens);
        print_vector("odds", odds);
    }
    //与えられた範囲から条件によって区分化されている位置を得る
    {
        std::cout << "partition_point: ";
       
        std::vector<int> v = { 1, 2, 3, 4, 5 };

        std::partition(v.begin(), v.end(), is_even);

        // 偶数グループと奇数グループに分かれた位置を得る
        decltype(v)::iterator it = std::partition_point(v.begin(), v.end(), is_even);

        print_vector("v", v);
        std::cout << *it << std::endl;
    }
#pragma endregion

    std::cin.get();
}

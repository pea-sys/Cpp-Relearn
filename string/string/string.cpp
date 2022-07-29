
#include <iostream>
#include <string>


int main()
{
    //char_traitsは、basic_string文字列クラスのおける、各要素の特性を使用するためのクラスである
    {
        std::cout << "char_traits" << std::endl;
        // std::stringと同じ
        std::basic_string<char, std::char_traits<char>> a = "aaa";
        std::basic_string<char, std::char_traits<char>> b = "bbb";

        // char_traits::compare()で比較が行われる
        if (a < b) {
            std::cout << "then" << std::endl;
        }
        else {
            std::cout << "else" << std::endl;
        }
    }
    //basic_stringクラスは、あらゆる文字型を使用できる文字列クラスである
    {
        std::cout << "basic_string" << std::endl;
        // C文字列からstringオブジェクトを構築
        std::string s = "hello";

        // 末尾に文字列を追加
        s += " world";

        // 部分文字列を取得(始点:0、始点からの文字数:5)
        std::string hello = s.substr(0, 5);

        // ostreamへの出力
        std::cout << hello << std::endl;

        // C文字列を取得し、const char*を要求するAPIに渡す
        std::printf("%s", s.c_str());
    }
    //stoi:文字列strを数値として読み取って、int型の値に変換する
    {
        std::cout << "stoi" << std::endl;
        // 10進法での変換
        {
            std::cout << "---- base = 10" << std::endl;

            int x = std::stoi("10"); // std::stoi("10", nullptr, 10);
            std::cout << x << std::endl;

            int xw = std::stoi(L"11"); // std::stoi(L"11", nullptr, 10);
            std::cout << xw << std::endl;
        }

        // 2進法での変換
        {
            std::cout << "---- base = 2" << std::endl;

            int x = std::stoi("1001", nullptr, 2);
            std::cout << x << std::endl;

            int xw = std::stoi(L"01001", nullptr, 2); // 先頭に0が付いていてもよい
            std::cout << xw << std::endl;
        }

        // 8進法での変換
        {
            std::cout << "---- base = 8" << std::endl;

            int x = std::stoi("10", nullptr, 8);
            std::cout << x << std::endl;

            int xw = std::stoi(L"10", nullptr, 8);
            std::cout << xw << std::endl;
        }

        // 16進法での変換
        {
            std::cout << "---- base = 16" << std::endl;

            int x = std::stoi("10", nullptr, 16);
            std::cout << x << std::endl;

            int xw = std::stoi(L"11", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // 16進法での変換（プレフィックス付き）
        {
            int x = std::stoi("0x20", nullptr, 16);
            std::cout << x << std::endl;

            int xw = std::stoi(L"0x21", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // base = 0による10進法・8進法・16進法の自動判別
        {
            std::cout << "---- base = 0" << std::endl;

            std::cout << std::stoi("100", nullptr, 0) << std::endl;
            std::cout << std::stoi("0100", nullptr, 0) << std::endl;
            std::cout << std::stoi("0x100", nullptr, 0) << std::endl;

            std::cout << std::stoi(L"100", nullptr, 0) << std::endl;
            std::cout << std::stoi(L"0100", nullptr, 0) << std::endl;
            std::cout << std::stoi(L"0x100", nullptr, 0) << std::endl;
        }

        // 2番目の仮引数の使用例
        {
            std::cout << "---- use of idx parameter" << std::endl;

            std::string s = "30%";
            std::size_t i;
            int x = std::stoi(s, &i);
            std::cout << x << ' ' << s[i] << std::endl;

            std::wstring ws = L"31%";
            std::size_t wi;
            int xw = std::stoi(ws, &wi);
            std::cout << xw << ' ' << wi << std::endl;
        }

        // 文字列先頭に空白がある場合
        {
            std::cout << "---- space character before number" << std::endl;
            std::cout << std::stoi("    -1") << std::endl;
            std::cout << std::stoi(L"    -2") << std::endl;
        }
    }
    //stol:文字列strを数値として読み取って、long型の値に変換する。
    {
        std::cout << "stol" << std::endl;
        // 10進法での変換
        {
            std::cout << "---- base = 10" << std::endl;

            long x = std::stol("10"); // std::stol("10", nullptr, 10);
            std::cout << x << std::endl;

            long xw = std::stol(L"11"); // std::stol(L"11", nullptr, 10);
            std::cout << xw << std::endl;
        }

        // 2進法での変換
        {
            std::cout << "---- base = 2" << std::endl;

            long x = std::stol("1001", nullptr, 2);
            std::cout << x << std::endl;

            long xw = std::stol(L"01001", nullptr, 2); // 先頭に0が付いていてもよい
            std::cout << xw << std::endl;
        }

        // 8進法での変換
        {
            std::cout << "---- base = 8" << std::endl;

            long x = std::stol("10", nullptr, 8);
            std::cout << x << std::endl;

            long xw = std::stol(L"10", nullptr, 8);
            std::cout << xw << std::endl;
        }

        // 16進法での変換
        {
            std::cout << "---- base = 16" << std::endl;

            long x = std::stol("10", nullptr, 16);
            std::cout << x << std::endl;

            long xw = std::stol(L"11", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // 16進法での変換（プレフィックス付き）
        {
            long x = std::stol("0x20", nullptr, 16);
            std::cout << x << std::endl;

            long xw = std::stol(L"0x21", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // base = 0による10進法・8進法・16進法の自動判別
        {
            std::cout << "---- base = 0" << std::endl;

            std::cout << std::stol("100", nullptr, 0) << std::endl;
            std::cout << std::stol("0100", nullptr, 0) << std::endl;
            std::cout << std::stol("0x100", nullptr, 0) << std::endl;

            std::cout << std::stol(L"100", nullptr, 0) << std::endl;
            std::cout << std::stol(L"0100", nullptr, 0) << std::endl;
            std::cout << std::stol(L"0x100", nullptr, 0) << std::endl;
        }

        // 2番目の仮引数の使用例
        {
            std::cout << "---- use of idx parameter" << std::endl;

            std::string s = "30%";
            std::size_t i;
            long x = std::stol(s, &i);
            std::cout << x << ' ' << s[i] << std::endl;

            std::wstring ws = L"31%";
            std::size_t wi;
            long xw = std::stol(ws, &wi);
            std::cout << xw << ' ' << wi << std::endl;
        }

        // 文字列先頭に空白がある場合
        {
            std::cout << "---- space character before number" << std::endl;
            std::cout << std::stol("    -1") << std::endl;
            std::cout << std::stol(L"    -2") << std::endl;
        }
    }
    //stoul:文字列strを数値として読み取って、unsigned long型の値に変換する
    {
        std::cout << "stoul" << std::endl;
        // 10進法での変換
        {
            std::cout << "---- base = 10" << std::endl;

            unsigned long x = std::stoul("10"); // std::stoul("10", nullptr, 10);
            std::cout << x << std::endl;

            unsigned long xw = std::stoul(L"11"); // std::stoul(L"11", nullptr, 10);
            std::cout << xw << std::endl;
        }

        // 2進法での変換
        {
            std::cout << "---- base = 2" << std::endl;

            unsigned long x = std::stoul("1001", nullptr, 2);
            std::cout << x << std::endl;

            unsigned long xw = std::stoul(L"01001", nullptr, 2); // 先頭に0が付いていてもよい
            std::cout << xw << std::endl;
        }

        // 8進法での変換
        {
            std::cout << "---- base = 8" << std::endl;

            unsigned long x = std::stoul("10", nullptr, 8);
            std::cout << x << std::endl;

            unsigned long xw = std::stoul(L"10", nullptr, 8);
            std::cout << xw << std::endl;
        }

        // 16進法での変換
        {
            std::cout << "---- base = 16" << std::endl;

            unsigned long x = std::stoul("10", nullptr, 16);
            std::cout << x << std::endl;

            unsigned long xw = std::stoul(L"11", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // 16進法での変換（プレフィックス付き）
        {
            unsigned long x = std::stoul("0x20", nullptr, 16);
            std::cout << x << std::endl;

            unsigned long xw = std::stoul(L"0x21", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // base = 0による10進法・8進法・16進法の自動判別
        {
            std::cout << "---- base = 0" << std::endl;

            std::cout << std::stoul("100", nullptr, 0) << std::endl;
            std::cout << std::stoul("0100", nullptr, 0) << std::endl;
            std::cout << std::stoul("0x100", nullptr, 0) << std::endl;

            std::cout << std::stoul(L"100", nullptr, 0) << std::endl;
            std::cout << std::stoul(L"0100", nullptr, 0) << std::endl;
            std::cout << std::stoul(L"0x100", nullptr, 0) << std::endl;
        }

        // 2番目の仮引数の使用例
        {
            std::cout << "---- use of idx parameter" << std::endl;

            std::string s = "30%";
            std::size_t i;
            unsigned long x = std::stoul(s, &i);
            std::cout << x << ' ' << s[i] << std::endl;

            std::wstring ws = L"31%";
            std::size_t wi;
            unsigned long xw = std::stoul(ws, &wi);
            std::cout << xw << ' ' << wi << std::endl;
        }

        // 文字列先頭に空白がある場合
        {
            std::cout << "---- space character before number" << std::endl;
            std::cout << std::stoul("    1") << std::endl;
            std::cout << std::stoul(L"    2") << std::endl;
        }
    }
    //stoul:文字列strを数値として読み取って、unsigned long long型の値に変換する。
    {
        std::cout << "stoul" << std::endl;
        // 10進法での変換
        {
            std::cout << "---- base = 10" << std::endl;

            unsigned long long x = std::stoull("10"); // std::stoull("10", nullptr, 10);
            std::cout << x << std::endl;

            unsigned long long xw = std::stoull(L"11"); // std::stoull(L"11", nullptr, 10);
            std::cout << xw << std::endl;
        }

        // 2進法での変換
        {
            std::cout << "---- base = 2" << std::endl;

            unsigned long long x = std::stoull("1001", nullptr, 2);
            std::cout << x << std::endl;

            unsigned long long xw = std::stoull(L"01001", nullptr, 2); // 先頭に0が付いていてもよい
            std::cout << xw << std::endl;
        }

        // 8進法での変換
        {
            std::cout << "---- base = 8" << std::endl;

            unsigned long long x = std::stoull("10", nullptr, 8);
            std::cout << x << std::endl;

            unsigned long long xw = std::stoull(L"11", nullptr, 8);
            std::cout << xw << std::endl;
        }

        // 16進法での変換
        {
            std::cout << "---- base = 16" << std::endl;

            unsigned long long x = std::stoull("10", nullptr, 16);
            std::cout << x << std::endl;

            unsigned long long xw = std::stoull(L"11", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // 16進法での変換（プレフィックス付き）
        {
            unsigned long long x = std::stoull("0x20", nullptr, 16);
            std::cout << x << std::endl;

            unsigned long long xw = std::stoull(L"0x21", nullptr, 16);
            std::cout << xw << std::endl;
        }

        // base = 0による10進法・8進法・16進法の自動判別
        {
            std::cout << "---- base = 0" << std::endl;

            std::cout << std::stoull("100", nullptr, 0) << std::endl;
            std::cout << std::stoull("0100", nullptr, 0) << std::endl;
            std::cout << std::stoull("0x100", nullptr, 0) << std::endl;

            std::cout << std::stoull(L"100", nullptr, 0) << std::endl;
            std::cout << std::stoull(L"0100", nullptr, 0) << std::endl;
            std::cout << std::stoull(L"0x100", nullptr, 0) << std::endl;
        }

        // 2番目の仮引数の使用例
        {
            std::cout << "---- use of idx parameter" << std::endl;

            std::string s = "30%";
            std::size_t i;
            unsigned long long x = std::stoull(s, &i);
            std::cout << x << ' ' << s[i] << std::endl;

            std::wstring ws = L"31%";
            std::size_t wi;
            unsigned long long xw = std::stoull(ws, &wi);
            std::cout << xw << ' ' << wi << std::endl;
        }

        // 文字列先頭に空白がある場合
        {
            std::cout << "---- space character before number" << std::endl;
            std::cout << std::stoull("    1") << std::endl;
            std::cout << std::stoull(L"    2") << std::endl;
        }
    }
    //stof:文字列strを数値として読み取って、float型の値に変換する
    {
        std::cout << "stof" << std::endl;
        // 10進法での変換
        std::cout << "---- decimal point" << std::endl;

        float a = std::stof("1.5"); // std::stof("1.5", nullptr);
        std::cout << a << std::endl;

        float aw = std::stof(L"1."); // std::stof(L"1.", nullptr);
        std::cout << aw << std::endl;

        // 指数表記の変換
        std::cout << "---- base = 8" << std::endl;

        float b = std::stof("0.5e3", nullptr);
        std::cout << b << std::endl;

        float bw = std::stof(L".25e3", nullptr);
        std::cout << bw << std::endl;

        // 16進法での変換
        std::cout << "---- base = 16" << std::endl;

        float c = std::stof("0x1.2P3", nullptr);
        std::cout << c << std::endl;

        float cw = std::stof(L"0x1.2P4", nullptr);
        std::cout << cw << std::endl;

        // 2番目の仮引数の使用例
        std::cout << "---- use of idx parameter" << std::endl;

        std::string es = "30.75%";
        std::size_t ei;
        float e = std::stof(es, &ei);
        std::cout << e << ' ' << es[ei] << std::endl;

        std::wstring ews = L"32%";
        std::size_t ewi;
        float ew = std::stof(ews, &ewi);
        std::cout << ew << ' ' << ewi << std::endl;

        // 文字列先頭に空白がある場合
        std::cout << "---- space character before number" << std::endl;
        std::cout << std::stof("    -1") << std::endl;
        std::cout << std::stof(L"    -.25") << std::endl;
    }
    //stod:文字列strを数値として読み取って、double型の値に変換する。
    {
        std::cout << "stod" << std::endl;
        // 10進法での変換
        std::cout << "---- decimal point" << std::endl;

        double a = std::stod("1.5"); // std::stod("1.5", nullptr);
        std::cout << a << std::endl;

        double aw = std::stod(L"1."); // std::stod(L"1.", nullptr);
        std::cout << aw << std::endl;

        // 指数表記の変換
        std::cout << "---- base = 8" << std::endl;

        double b = std::stod("0.5e3", nullptr);
        std::cout << b << std::endl;

        double bw = std::stod(L".25e3", nullptr);
        std::cout << bw << std::endl;

        // 16進法での変換
        std::cout << "---- base = 16" << std::endl;

        double c = std::stod("0x1.2P3", nullptr);
        std::cout << c << std::endl;

        double cw = std::stod(L"0x1.2P4", nullptr);
        std::cout << cw << std::endl;

        // 2番目の仮引数の使用例
        std::cout << "---- use of idx parameter" << std::endl;

        std::string es = "30.75%";
        std::size_t ei;
        double e = std::stod(es, &ei);
        std::cout << e << ' ' << es[ei] << std::endl;

        std::wstring ews = L"32%";
        std::size_t ewi;
        double ew = std::stod(ews, &ewi);
        std::cout << ew << ' ' << ewi << std::endl;

        // 文字列先頭に空白がある場合
        std::cout << "---- space character before number" << std::endl;
        std::cout << std::stod("    -1") << std::endl;
        std::cout << std::stod(L"    -.25") << std::endl;
    }
    //stold:文字列strを数値として読み取って、long double型の値に変換する
    {
        std::cout << "stold" << std::endl;
        // 10進法での変換
        std::cout << "---- decimal point" << std::endl;

        long double a = std::stold("1.5"); // std::stold("1.5", nullptr);
        std::cout << a << std::endl;

        long double aw = std::stold(L"1."); // std::stold(L"1.", nullptr);
        std::cout << aw << std::endl;

        // 指数表記の変換
        std::cout << "---- base = 8" << std::endl;

        long double b = std::stold("0.5e3", nullptr);
        std::cout << b << std::endl;

        long double bw = std::stold(L".25e3", nullptr);
        std::cout << bw << std::endl;

        // 16進法での変換
        std::cout << "---- base = 16" << std::endl;

        long double c = std::stold("0x1.2P3", nullptr);
        std::cout << c << std::endl;

        long double cw = std::stold(L"0x1.2P4", nullptr);
        std::cout << cw << std::endl;

        // 2番目の仮引数の使用例
        std::cout << "---- use of idx parameter" << std::endl;

        std::string es = "30.75%";
        std::size_t ei;
        long double e = std::stold(es, &ei);
        std::cout << e << ' ' << es[ei] << std::endl;

        std::wstring ews = L"32%";
        std::size_t ewi;
        long double ew = std::stold(ews, &ewi);
        std::cout << ew << ' ' << ewi << std::endl;

        // 文字列先頭に空白がある場合
        std::cout << "---- space character before number" << std::endl;
        std::cout << std::stold("    -1") << std::endl;
        std::cout << std::stold(L"    -.25") << std::endl;
    }
    //to_string:数値valをstring型文字列に変換する
    {
        std::cout << "to_string" << std::endl;
        std::string s1 = std::to_string(123);
        std::cout << s1 << std::endl;

        std::string s2 = std::to_string(3.14);
        std::cout << s2 << std::endl;
    }
    //to_wstring:数値valをwstring型文字列に変換する。
    {
        std::cout << "to_wstring" << std::endl;
        std::wstring s1 = std::to_wstring(123);
        std::wcout << s1 << std::endl;

        std::wstring s2 = std::to_wstring(3.14);
        std::wcout << s2 << std::endl;
    }
    std::cin.get();
}



#include <iostream>
#include <charconv>
#include <iomanip>
#include <limits>

int main()
{
    //to_chars:与えられた数値（value）を文字列へ変換し、[first, last)内へ出力する。変換に際し、メモリ確保を行わず例外を投げることもない。
    {
        std::cout << "to_chars" << std::endl;
        char out[50]{};
        auto begin = std::begin(out);
        auto end = std::end(out);

        //(1) 10進数文字列へ変換
        if (auto [ptr, ec] = std::to_chars(begin, end, 10); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(1) 2進数文字列へ変換
        if (auto [ptr, ec] = std::to_chars(begin, end, 65535, 2); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(1) 36進数文字列へ変換
        if (auto [ptr, ec] = std::to_chars(begin, end, 35, 36); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }

        //リウヴィル数 
        constexpr double l = 0.11000100000000000000000100000000000;

        //(3) 精度・フォーマット指定なしの浮動小数点数変換
        if (auto [ptr, ec] = std::to_chars(begin, end, l); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }

        //(6) 精度指定なしの浮動小数点数変換、指数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::scientific); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(6) 精度指定なしの浮動小数点数変換、固定小数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::fixed); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(6) 精度指定なしの浮動小数点数変換、16進指数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::hex); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }

        //(9) 精度指定ありの浮動小数点数変換、指数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::scientific, 16); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(9) 精度指定ありの浮動小数点数変換、固定小数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::fixed, 16); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
        //(9) 精度指定ありの浮動小数点数変換、16進指数表記
        if (auto [ptr, ec] = std::to_chars(begin, end, l, std::chars_format::hex, 16); ec == std::errc{}) {
            std::cout << std::string_view(begin, ptr - begin) << std::endl;
        }
        else {
            std::cout << "conversion failed." << std::endl;
        }
    }
    //from_chars:与えられた[first, last)内の文字列から、オーバーロードと基数・フォーマット指定によって決まるパターンにマッチングする最初の数字文字列を見つけて、数値へ変換する
    {
        std::cout << "from_chars" << std::endl;
        {
            const char str[] = "00000123456789 is decimal";
            int value{};

            //(1) 10進数文字列からintへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "1111111111111111 is (65535)_10";
            int value{};

            //(1) 2進数文字列からintへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value, 2); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "Z is (35)_10";
            int value{};

            //(1) 36進数文字列からintへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value, 36); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "255";
            char value{};

            //(1) 失敗する例 MSVCにおけるcharの範囲は-128～127
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        std::cout << std::setprecision(16);

        {
            const char str[] = "3.1415926535897932384626433832795 is pi";
            double value{};

            //(3) 固定小数表記文字列からdoubleへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "1.10001e-01 is Liouville number";
            double value{};

            //(3) 指数表記文字列からdoubleへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "1.c29068986fcdf000p-4 is Liouville number";
            double value{};

            //(3) 16進指数表記文字列からdoubleへ変換
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value, std::chars_format::hex); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = " 3.1415926535897932384626433832795 is pi";
            double value{};

            //(3) 失敗する例 ホワイトスペース読み飛ばし
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "NaN";
            double value{};

            //(3) NaNの読み取り
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }

        {
            const char str[] = "-INF";
            double value{};

            //(3) INFの読み取り
            if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{}) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "conversion failed." << std::endl;
            }
        }
    }
    std::cin.get();
}

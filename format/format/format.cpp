
#include <iostream>
#include <format>

enum color { red, green, blue };

const char* color_names[] = { "red", "green", "blue" };

template<> struct std::formatter<color> : std::formatter<const char*> {
    auto format(color c, format_context& ctx) {
        return formatter<const char*>::format(color_names[c], ctx);
    }
};

int main()
{
    //format:書式文字列fmtに従ったフォーマットでargs...の文字列表現を文字列オブジェクトで返す
    {
        std::cout << "format" << std::endl;
        std::cout << std::format("The answer is {}.", 42) << std::endl;
    }
    //format_to:書式文字列fmtに従ったフォーマットでargs...の文字列表現を出力イテレーターoutに出力する
    {
        std::cout << "format_tot" << std::endl;
        std::string buffer;
        std::format_to(std::back_inserter(buffer), "The answer is {}.", 42);
        std::cout << buffer << std::endl;
    }
    //format_to_n:書式文字列fmtに従ったフォーマットでargs...の文字列表現を、最大でn文字だけ出力イテレーターoutに出力する
    {
        std::cout << "format_to_n" << std::endl;
        char buffer[256];
        auto [end, n] = std::format_to_n(buffer, std::size(buffer) - 1, "The answer is {}.", 42);
        *end = '\0';
        std::cout << buffer << std::endl;
    }
    //formatted_size:書式文字列fmtに従ったフォーマットでargs...の文字列表現を格納するために必要な文字数を返す
    {
        std::cout << "formatted_size" << std::endl;
        std::cout << std::formatted_size("The answer is {}.", 42) << std::endl;
    }
    //formatter:フォーマット引数の個々の型に対応する書式文字列の解析と値のフォーマットを担うクラス
    {
        std::cout << "formatter" << std::endl;
        int variable = 0;
        std::cout << std::format("{}", red) << std::endl;
    }
    std::cin.get();
}

#include <iostream>
#include <regex>
#include <string>
#include <list>
#include <iterator>

std::string code_to_string(std::regex_constants::error_type e)
{
    using namespace std::regex_constants;
    switch (e) {
    case error_collate:    return "error collapse";
    case error_ctype:      return "error ctype";
    case error_escape:     return "error escape";
    case error_backref:    return "error back reference";
    case error_brack:      return "error bracket";
    case error_paren:      return "error paren";
    case error_brace:      return "error brace";
    case error_badbrace:   return "error bad brace";
    case error_range:      return "error range";
    case error_space:      return "error space";
    case error_badrepeat:  return "error bad repeat";
    case error_complexity: return "error complexity";
    case error_stack:      return "error stack";
    default:
        throw std::invalid_argument("invalid error code");
    }
}

int main()
{
    //basic_regexクラステンプレートは、charT型の文字列から構築する正規表現を表す
    {
        std::cout << "basic_regex" << std::endl;
        std::string input = "12345";

        // 全てが数字か判定するための正規表現
        std::regex re(R"(^\d+$)");

        if (std::regex_match(input, re)) {
            std::cout << "全て数字です" << std::endl;
        }
        else {
            std::cout << "数字以外が含まれています" << std::endl;
        }
    }
    //regex_traitsは、正規表現ライブラリ<regex>クラス内で使用される、文字とその変換の規則に関する特性を表すクラス
    {
        std::cout << "regex_traits" << std::endl;
        std::regex_traits<char> traits;

        std::string class_name = "alnum"; // 正規表現中で[[:alnum:]]のように入力するクラス名

        // 文字'a'がアルファベットと数字のクラスに含まれているかを判定する
        if (traits.isctype('a', traits.lookup_classname(class_name.begin(), class_name.end()))) {
            std::cout << "'a' is alpha-numeric class" << std::endl;
        }
        else {
            std::cout << "'a' is not alpha-numeric class" << std::endl;
        }
    }
    //regex_errorクラスは、正規表現ライブラリ<regex>からのエラー報告として送出される、例外オブジェクトの型
    {
        std::cout << "regex_error" << std::endl;
        try {
            std::regex re("("); // 開きカッコに対応する閉じカッコがない正規表現を指定
        }
        catch (std::regex_error& e) {
            std::cout << code_to_string(e.code()) << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
    // sub_match は、文字列が正規表現にマッチした際に、その正規表現がマッチした範囲、あるいは、その正規表現内のあるグループがマッチした範囲を表す
    {
        std::cout << " sub_match" << std::endl;
        const std::string s(" abc 123 def ");
        const std::regex re(R"((?:(\d+)|(\w+))\s+)");
        std::smatch m;
        if (std::regex_search(s, m, re)) {
            for (int i = 0, n = m.size(); i < n; i++) {
                std::ssub_match sub = m[i];
                if (sub.matched) {
                    std::cout << i << ":range = [" << (sub.first - std::begin(s)) << ", " << (sub.second - std::begin(s)) << "), "
                        "length() = " << sub.length() << ", str() = '" << sub.str() << '\'' << std::endl;
                }
                else {
                    std::cout << i << ":not participate" << std::endl;
                }
            }
        }
        else {
            std::cout << "not match" << std::endl;
        }
    }
    //match_results は正規表現によるマッチ結果を格納するコンテナである
    {
        std::cout << "match_results" << std::endl;
        const char s[] = "The C++11 is very cool!!";
        const std::regex re("(\\w+) is (\\w+)");

        std::cmatch m;
        if (std::regex_search(s, m, re)) {
            std::cout << "ready = " << std::boolalpha << m.ready() << ", empty = " << m.empty() << std::endl << std::endl;
            std::cout << "prefix:'" << m.prefix() << '\'' << std::endl;
            for (std::size_t i = 0, n = m.size(); i < n; ++i) {
                std::cout << i << ":'" << m.str(i) << "\', position = " << m.position(i) << ", length = " << m.length(i) << std::endl;
            }
            std::cout << "suffix:'" << m.suffix() << '\'' << std::endl << std::endl;
            std::cout << m.format("$`14 is $2$'") << std::endl;
        }
        else {
            std::cout << "not match" << std::endl;
        }
    }
    //regex_match:指定された文字列全体が、正規表現にマッチするか否かの判定を行う
    {
        std::cout << "regex_match" << std::endl;
        std::cout << std::boolalpha;
        {
            // (1) の形式
            const std::list<char> s = { 'a', 'b', 'c', '1', '2', '3', 'd', 'e', 'f' };
            std::match_results<std::list<char>::const_iterator> m;
            std::cout << "(1) " << std::regex_match(std::begin(s), std::end(s), m, std::regex("\\w+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (2) の形式
            std::cmatch m;
            std::cout << "(2) " << std::regex_match("abc123def", m, std::regex("\\w+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (3) の形式
            const std::string s = "abc123def";
            std::smatch m;
            std::cout << "(3) " << std::regex_match(s, m, std::regex("\\w+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (4) の形式（コメントアウトを外すと C++14 ではエラーになる）
            //std::smatch m;
            //std::cout << "(4) " << std::regex_match(std::string("abc123def"), m, std::regex("\\w+")) << std::endl;
            //std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (5) の形式
            const std::list<char> s = { 'a', 'b', 'c', '1', '2', '3', 'd', 'e', 'f' };
            std::cout << "(5) " << std::regex_match(std::begin(s), std::end(s), std::regex("\\w+")) << std::endl;
        }
        {
            // (6) の形式
            std::cout << "(6) " << std::regex_match("abc123def", std::regex("\\w+")) << std::endl;
        }
        {
            // (7) の形式、その１
            const std::string s = "abc123def";
            std::cout << "(7)-1 " << std::regex_match(s, std::regex("\\w+")) << std::endl;
        }
        {
            // (7) の形式、その２（C++14 でもエラーにならない）
            std::cout << "(7)-2 " << std::regex_match(std::string("abc123def"), std::regex("\\w+")) << std::endl;
        }
    }
    //regex_search:指定された文字列中に、正規表現がマッチする部分が存在するか否かの判定を行う
    {
        std::cout << "regex_search" << std::endl;
        std::cout << std::boolalpha;
        {
            // (1) の形式
            const std::list<char> s = { 'a', 'b', 'c', '1', '2', '3', 'd', 'e', 'f' };
            std::match_results<std::list<char>::const_iterator> m;
            std::cout << "(1) " << std::regex_search(std::begin(s), std::end(s), m, std::regex("\\d+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (2) の形式
            std::cmatch m;
            std::cout << "(2) " << std::regex_search("abc123def", m, std::regex("\\d+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (3) の形式
            const std::string s = "abc123def";
            std::smatch m;
            std::cout << "(3) " << std::regex_search(s, m, std::regex("\\d+")) << std::endl;
            std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (4) の形式（コメントアウトを外すと C++14 ではエラーになる）
            //std::smatch m;
            //std::cout << "(4) " << std::regex_search(std::string("abc123def"), m, std::regex("\\d+")) << std::endl;
            //std::cout << "str = '" << m.str() << "', position = " << m.position() << std::endl;
        }
        {
            // (5) の形式
            const std::list<char> s = { 'a', 'b', 'c', '1', '2', '3', 'd', 'e', 'f' };
            std::cout << "(5) " << std::regex_search(std::begin(s), std::end(s), std::regex("\\d+")) << std::endl;
        }
        {
            // (6) の形式
            std::cout << "(6) " << std::regex_search("abc123def", std::regex("\\d+")) << std::endl;
        }
        {
            // (7) の形式、その１
            const std::string s = "abc123def";
            std::cout << "(7)-1 " << std::regex_search(s, std::regex("\\d+")) << std::endl;
        }
        {
            // (7) の形式、その２（C++14 でもエラーにならない）
            std::cout << "(7)-2 " << std::regex_search(std::string("abc123def"), std::regex("\\d+")) << std::endl;
        }
    }
    //regex_replace:指定された文字列の中で、正規表現にマッチする部分を指定した文字列に置換する
    {
        std::cout << "regex_replace" << std::endl;
        {
            // (1) の形式
            const std::list<char> s = { 'a', 'b', 'c', '0', '1', '2', 'd', 'e', 'f' };
            const std::regex re("\\d+");
            const std::string fmt = "[$&]";
            std::cout << "(1) '";
            std::regex_replace(std::ostream_iterator<char>(std::cout), std::begin(s), std::end(s), re, fmt);
            std::cout << '\'' << std::endl;
        }
        {
            // (2) の形式
            const std::list<char> s = { 'a', 'b', 'c', '0', '1', '2', 'd', 'e', 'f' };
            const std::regex re("\\d+");
            const char fmt[] = "[$&]";
            const std::regex_constants::match_flag_type flags = std::regex_constants::format_no_copy;
            std::cout << "(2) '";
            std::regex_replace(std::ostream_iterator<char>(std::cout), std::begin(s), std::end(s), re, fmt, flags);
            std::cout << '\'' << std::endl;
        }
        {
            // (3) の形式
            const std::string s = "abc123def456ghi";
            const std::regex re("\\d+");
            const std::string fmt = "[$&]";
            std::cout << "(3) '" << std::regex_replace(s, re, fmt) << '\'' << std::endl;
        }
        {
            // (4) の形式
            const std::string s = "abc123def456ghi";
            const std::regex re("\\d+");
            const char fmt[] = "[$&]";
            const std::regex_constants::match_flag_type flags = std::regex_constants::format_first_only;
            std::cout << "(4) '" << std::regex_replace(s, re, fmt, flags) << '\'' << std::endl;
        }
        {
            // (5) の形式
            const char s[] = "abc123def456ghi";
            const std::regex re("(\\d)(\\d)(\\d)");
            const std::string fmt = "[$3$2$1]";
            std::cout << "(5) '" << std::regex_replace(s, re, fmt) << '\'' << std::endl;
        }
        {
            // (6) の形式
            const char s[] = "abc123def456ghi";
            const std::regex re("(\\d)(\\d)(\\d)");
            const char fmt[] = "[\\3\\2\\1]";
            const std::regex_constants::match_flag_type flags = std::regex_constants::format_sed;
            std::cout << "(6) '" << std::regex_replace(s, re, fmt, flags) << '\'' << std::endl;
        }
    }
    std::cin.get();
}

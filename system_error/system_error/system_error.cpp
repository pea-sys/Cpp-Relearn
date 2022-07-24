#include <iostream>
#include <system_error>
#include <string>
#include <future>


class user_defined_error_category : public std::error_category {
public:
    const char* name() const noexcept override
    {
        return "user defined error";
    }

    std::string message(int ev) const override
    {
        return "error message";
    }
};

const std::error_category& user_defined_category()
{
    static user_defined_error_category cat;
    return cat;
}
enum class user_defined_error {
    success = 0,
    error = 1,
};

int main()
{
    //error_category:エラー情報を分類するための基底クラスである。
    {
        std::cout << "error_category";
        const std::error_category& cat = user_defined_category();
        std::cout << cat.name() << std::endl;
    }
    //generic_category:汎用エラーに関するerror_categoryを返す
    {
        std::cout << "generic_category";
        const std::error_category& cat = std::generic_category();

        std::cout << cat.name() << std::endl;
        std::cout << cat.message(static_cast<int>(std::errc::invalid_argument)) << std::endl;
    }
    //system_category:環境固有のエラーに関するerror_categoryを返す
    {
        std::cout << "system_category";
        const std::error_category& cat = std::system_category();

        std::cout << cat.name() << std::endl;
        std::cout << cat.message(static_cast<int>(std::errc::invalid_argument)) << std::endl;
    }
    //error_codeは、OSのAPIで発生するエラー値およびそのエラーメッセージを扱うクラスである
    {
        try {
            std::cout << "error_code";
            // 不正な引数エラー
            std::error_code ec(static_cast<int>(std::errc::invalid_argument),
                std::generic_category());

            throw std::system_error(ec, "system error!");
        }
        catch (std::system_error& e) {
            // 例外オブジェクトからerror_codeを取得
            const std::error_code& ec = e.code();

            // エラー値とメッセージを出力
            std::cout << ec.value() << std::endl;
            std::cout << ec.message() << std::endl;
        }
    }
    //is_error_code_enum:error_codeのエラー値として見なせる列挙型かどうかを判定する
    {
        std::cout << "is_error_code_enum";
        static_assert(std::is_error_code_enum<std::io_errc>::value, "");
        static_assert(std::is_error_code_enum<std::future_errc>::value, "");
        static_assert(!std::is_error_code_enum<user_defined_error>::value, "");
    }
    //is_error_condition_enum:error_conditionのエラー値として見なせる列挙型かどうかを判定する。
    {
        std::cout << "is_error_condition_enum:error_condition";
        static_assert(std::is_error_condition_enum<std::errc>::value, "");
        static_assert(!std::is_error_condition_enum<user_defined_error>::value, "");
    }
    //make_error_code:errc型の列挙値からerror_codeオブジェクトを生成する
    {
        std::cout << "make_error_code";
        std::error_code ec = std::make_error_code(std::errc::invalid_argument);

        std::cout << "category : " << ec.category().name() << std::endl;
        std::cout << "value : " << ec.value() << std::endl;
        std::cout << "message : " << ec.message() << std::endl;
    }
    //make_error_condition:errc型の列挙値からerror_conditionオブジェクトを生成する
    {
        std::cout << "make_error_condition";
        std::error_condition ec = std::make_error_condition(std::errc::invalid_argument);

        std::cout << "category : " << ec.category().name() << std::endl;
        std::cout << "value : " << ec.value() << std::endl;
        std::cout << "message : " << ec.message() << std::endl;
    }
    std::cin.get();
}
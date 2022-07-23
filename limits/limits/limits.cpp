#include <iostream>
#include <limits>
#include <cassert>
std::string denorm_string(std::float_denorm_style e)
{
    switch (e) {
    case std::denorm_indeterminate: return "indeterminate";
    case std::denorm_absent:        return "absent";
    case std::denorm_present:       return "present";
    }
    throw std::invalid_argument("not support value");

}std::string style_string(std::float_round_style e)
{
    switch (e) {
    case std::round_indeterminate:       return "indeterminate";
    case std::round_toward_zero:         return "toward zero";
    case std::round_to_nearest:          return "to nearest";
    case std::round_toward_infinity:     return "toward infinity";
    case std::round_toward_neg_infinity: return "toward negative infinity";
    }
    throw std::invalid_argument("invalid style");
}

int main()
{
    //min:型ごとの値の最小値を取得する
    {
        std::cout << "min" << std::endl;
        constexpr int i = std::numeric_limits<int>::min();
        constexpr double d = std::numeric_limits<double>::min();

        std::cout << i << std::endl;
        std::cout << d << std::endl;
    }

    //max:型Tの値の最大値を取得する
    {
        std::cout << "max" << std::endl;
        constexpr int i = std::numeric_limits<int>::max();
        constexpr double d = std::numeric_limits<double>::max();

        std::cout << i << std::endl;
        std::cout << d << std::endl;
    }
    //lowest:型ごとの値の最小値を取得する
    {
        std::cout << "lowest" << std::endl;
        constexpr int i = std::numeric_limits<int>::lowest();
        constexpr double d = std::numeric_limits<double>::lowest();

        std::cout << i << std::endl;
        std::cout << d << std::endl;
    }
    //digits:基数 radix において表現できる桁数を示す
    {
        std::cout << "digits" << std::endl;
        constexpr int i = std::numeric_limits<int>::digits;
        constexpr int ui = std::numeric_limits<unsigned int>::digits;
        constexpr int d = std::numeric_limits<double>::digits;

        std::cout << i << std::endl;
        std::cout << ui << std::endl;
        std::cout << d << std::endl;
    }
    //digits10:正確に表現可能な桁数を取得する
    {
        std::cout << "digits10" << std::endl;
        std::cout << std::numeric_limits<float>::digits10 << std::endl;
        std::cout << std::numeric_limits<double>::digits10 << std::endl;

        std::cout << std::numeric_limits<int>::digits10 << std::endl;
    }
    //is_signed:型Tが符号付き型であるなら、is_signedはtrueとなり、そうでなければfalseとなる
    {
        std::cout << "is_signed" << std::endl;
        constexpr bool a = std::numeric_limits<int>::is_signed;
        constexpr bool b = std::numeric_limits<unsigned int>::is_signed;

        static_assert(a, "must be signed");
        static_assert(!b, "must be unsigned");
    }
    //is_integer:型Tが整数型であるならis_integerはtrueとなり、そうでなければfalseとなる
    {
        std::cout << "is_integer" << std::endl;
        constexpr bool a = std::numeric_limits<int>::is_integer;
        constexpr bool b = std::numeric_limits<unsigned int>::is_integer;
        constexpr bool c = std::numeric_limits<double>::is_integer;

        static_assert(a, "int must be integer");
        static_assert(b, "unsigned int must be integer");
        static_assert(!c, "double must be not integer");
    }
    //is_exact:型Tが正確(exact)な表現を持つ場合、is_exactはtrueとなり、そうでない場合falseとなる
    {
        std::cout << "is_exact" << std::endl;
        constexpr bool a = std::numeric_limits<int>::is_exact;
        constexpr bool b = std::numeric_limits<char>::is_exact;
        constexpr bool c = std::numeric_limits<double>::is_exact;

        static_assert(a, "int must be exact");
        static_assert(b, "char must be exact");
        static_assert(!c, "double must be not exact");
    }
    //radix:digitsを表現する基数を示す
    {
        std::cout << "radix" << std::endl;
        constexpr int d = std::numeric_limits<int>::radix;
        constexpr int c = std::numeric_limits<char>::radix;
        constexpr int f = std::numeric_limits<double>::radix;

        std::cout << "int : " << d << std::endl;
        std::cout << "char : " << c << std::endl;
        std::cout << "double : " << f << std::endl;
    }
    //epsilon:機械イプシロン。1より大きい数の中で最小の値と1との差
    {
        std::cout << "epsilon" << std::endl;
        constexpr double e = std::numeric_limits<double>::epsilon();

        std::cout << e << std::endl;

        double a = 0.6 - 0.4;
        double b = 0.2;
        if (std::abs(a - b) <= e) {
            std::cout << "equal" << std::endl;
        }
        else {
            std::cout << "not equal" << std::endl;
        }
    }
    //round_error:最大の丸め誤差を取得する
    {
        std::cout << "round_error" << std::endl;
        constexpr float f = std::numeric_limits<float>::round_error();
        constexpr double d = std::numeric_limits<double>::round_error();

        std::cout << f << std::endl;
        std::cout << d << std::endl;
    }
    //min_exponent:浮動小数点数型において、型Tの指数下限値を得る
    {
        std::cout << "min_exponent" << std::endl;
        constexpr int f = std::numeric_limits<float>::min_exponent;
        constexpr int d = std::numeric_limits<double>::min_exponent;

        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //min_exponent10:浮動小数点数型において、型Tの指数下限値を得る
    {
        std::cout << "min_exponent10" << std::endl;
        constexpr int f = std::numeric_limits<float>::min_exponent10;
        constexpr int d = std::numeric_limits<double>::min_exponent10;

        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //max_exponent:浮動小数点数型において、型Tの指数上限値を得る。
    {
        std::cout << "max_exponent" << std::endl;
        constexpr int f = std::numeric_limits<float>::max_exponent;
        constexpr int d = std::numeric_limits<double>::max_exponent;

        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //max_exponent10:浮動小数点数型において、型Tの指数上限値を得る
    {
        std::cout << "max_exponent10" << std::endl;
        constexpr int f = std::numeric_limits<float>::max_exponent10;
        constexpr int d = std::numeric_limits<double>::max_exponent10;

        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //has_infinity:浮動小数点数型において、型Tが正の無限表現を持っているかどうかを判定する
    {
        std::cout << "has_infinity" << std::endl;
        constexpr bool a = std::numeric_limits<int>::has_infinity;
        constexpr bool b = std::numeric_limits<float>::has_infinity;
        constexpr bool c = std::numeric_limits<double>::has_infinity;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "float : " << b << std::endl;
        std::cout << "double : " << c << std::endl;
    }
    //has_quiet_NaN:浮動小数点数型において、型Tがシグナルを投げないNaN (Not a Number)を持っているかを判定
    {
        std::cout << "has_quiet_NaN" << std::endl;
        constexpr bool a = std::numeric_limits<int>::has_quiet_NaN;
        constexpr bool b = std::numeric_limits<float>::has_quiet_NaN;
        constexpr bool c = std::numeric_limits<double>::has_quiet_NaN;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "float : " << b << std::endl;
        std::cout << "double : " << c << std::endl;
    }
    //has_signaling_NaN:浮動小数点数型において、型Tがシグナルを投げるNaN (Not a Number)を持っているかを判定する
    {
        std::cout << "has_signaling_NaN" << std::endl;
        constexpr bool a = std::numeric_limits<int>::has_signaling_NaN;
        constexpr bool b = std::numeric_limits<float>::has_signaling_NaN;
        constexpr bool c = std::numeric_limits<double>::has_signaling_NaN;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "float : " << b << std::endl;
        std::cout << "double : " << c << std::endl;
    }
    //has_denorm:浮動小数点数型において、型Tの非正規化数(Denormal Number)のサポート状況を判定する
    {
        std::cout << "has_denorm" << std::endl;
        constexpr std::float_denorm_style a = std::numeric_limits<int>::has_denorm;
        constexpr std::float_denorm_style b = std::numeric_limits<float>::has_denorm;
        constexpr std::float_denorm_style c = std::numeric_limits<double>::has_denorm;

        std::cout << std::boolalpha;
        std::cout << "int : " << denorm_string(a) << std::endl;
        std::cout << "float : " << denorm_string(b) << std::endl;
        std::cout << "double : " << denorm_string(c) << std::endl;
    }
    //has_denorm_loss:浮動小数点数型において、精度の損失が非正規化数によるものかを判定する
    {
        std::cout << "has_denorm_loss" << std::endl;
        constexpr bool a = std::numeric_limits<int>::has_denorm_loss;
        constexpr bool b = std::numeric_limits<float>::has_denorm_loss;
        constexpr bool c = std::numeric_limits<double>::has_denorm_loss;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "float : " << b << std::endl;
        std::cout << "double : " << c << std::endl;
    }
    //infinity:浮動小数点数型における、正の無限表現を取得する
    {
        std::cout << "infinity" << std::endl;
        constexpr float i = std::numeric_limits<int>::infinity();
        constexpr float f = std::numeric_limits<float>::infinity();
        constexpr double d = std::numeric_limits<double>::infinity();

        std::cout << "int : " << i << std::endl;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //quiet_NaN:浮動小数点数型において、シグナルを発生させないNaN (Not a Number)を取得する
    {
        std::cout << "quiet_NaN" << std::endl;
        constexpr float i = std::numeric_limits<int>::quiet_NaN();
        constexpr float f = std::numeric_limits<float>::quiet_NaN();
        constexpr double d = std::numeric_limits<double>::quiet_NaN();

        std::cout << "int : " << i << std::endl;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //signaling_NaN:浮動小数点数型において、シグナルを発生させるNaN (Not a Number)を取得する
    {
        std::cout << "signaling_NaN" << std::endl;
        constexpr float i = std::numeric_limits<int>::signaling_NaN();
        constexpr float f = std::numeric_limits<float>::signaling_NaN();
        constexpr double d = std::numeric_limits<double>::signaling_NaN();

        std::cout << "int : " << i << std::endl;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //denorm_min:浮動小数点数型において、最小の正の非正規化数(denormalized value)を取得する
    {
        std::cout << "denorm_min" << std::endl;
        constexpr int i = std::numeric_limits<int>::denorm_min();
        constexpr float f = std::numeric_limits<float>::denorm_min();
        constexpr double d = std::numeric_limits<double>::denorm_min();

        std::cout << "int : " << i << std::endl;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //is_iec559:浮動小数点数型において、型TがIEC 559 (IEEE 754) に準拠しているかを判定する
    {
        std::cout << "is_iec559" << std::endl;
        constexpr bool f = std::numeric_limits<float>::is_iec559;
        constexpr bool d = std::numeric_limits<double>::is_iec559;

        std::cout << std::boolalpha;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //is_bounded:型Tの値のなす集合が有限かを判定する
    {
        std::cout << "is_bounded" << std::endl;
        constexpr bool a = std::numeric_limits<int>::is_bounded;
        constexpr bool b = std::numeric_limits<float>::is_bounded;
        constexpr bool c = std::numeric_limits<double>::is_bounded;
        constexpr bool d = std::numeric_limits<char>::is_bounded;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "float : " << b << std::endl;
        std::cout << "double : " << c << std::endl;
        std::cout << "char : " << d << std::endl;
    }
    //is_modulo:加算 (+) ・減算 (-) ・乗算 (*) における数学的な値と、その型での値との間に (max() - min() + 1) を法として常に合同関係があるかを判定する。
    {
        std::cout << "is_modulo" << std::endl;
        std::cout << std::boolalpha;
        std::cout << "int : " << std::numeric_limits<int>::is_modulo << std::endl;
        std::cout << "unsigned int : " << std::numeric_limits<unsigned int>::is_modulo << std::endl;
        std::cout << "float : " << std::numeric_limits<float>::is_modulo << std::endl;
        std::cout << "char : " << std::numeric_limits<char>::is_modulo << std::endl;
        std::cout << "char : " << std::numeric_limits<bool>::is_modulo << std::endl;

        assert(std::numeric_limits<unsigned int>::max() + 1 == 0u);
    }
    //trap:算術演算によってトラップが発生する型かを判定する
    {
        std::cout << "trap" << std::endl;
        constexpr bool a = std::numeric_limits<int>::traps;
        constexpr bool b = std::numeric_limits<char>::traps;
        constexpr bool c = std::numeric_limits<float>::traps;
        constexpr bool d = std::numeric_limits<double>::traps;

        std::cout << std::boolalpha;
        std::cout << "int : " << a << std::endl;
        std::cout << "char : " << b << std::endl;
        std::cout << "float : " << c << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //tinyness_before:浮動小数点数型において、型Tが丸めが行われる前に小さな値になることを検出できる場合はtrue、そうでなければfalseとなる
    {
        std::cout << "tinyness_before" << std::endl;
        constexpr bool f = std::numeric_limits<float>::tinyness_before;
        constexpr bool d = std::numeric_limits<double>::tinyness_before;

        std::cout << std::boolalpha;
        std::cout << "float : " << f << std::endl;
        std::cout << "double : " << d << std::endl;
    }
    //round_style:浮動小数点数の丸めスタイルを取得する
    {
        std::cout << "round_style" << std::endl;
        constexpr std::float_round_style f = std::numeric_limits<float>::round_style;
        constexpr std::float_round_style d = std::numeric_limits<double>::round_style;

        std::cout << "float : " << style_string(f) << std::endl;
        std::cout << "double : " << style_string(d) << std::endl;
    }
    std::cin.get();
}


// iostream.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    //cin は、標準入力に対する入力ストリームオブジェクトである
    {
        std::cout << "名前を入力してください: ";

        std::string s;     // std::cin.tie() == &std::cout であるため、
        std::cin >> s;     // std::cout を明示的に flush しなくても
                           // 上記の出力が flush されることが保証されている

        std::cout << "あなたの名前は「" << s << "」ですね。" << std::endl;
    }
    //coutもwcoutも、標準出力に対する出力ストリームオブジェクトである。
    {
        std::cout << "Hello world" << std::endl;
    }
    //clogもwclogも、標準エラー出力に対する出力ストリームオブジェクトである
    {
        char const* filename = "../test.txt";
        if (filename != nullptr)
        {
            std::clog << "ファイル名: " << filename << std::endl;
            std::fstream f(filename);
            std::cout << f.rdbuf() << std::endl;
        }
        else
        {
            std::clog << "ファイル名を指定してください" << std::endl;
            return 1;
        }
    }
    //cerrもwcerrも、標準エラー出力に対する出力ストリームオブジェクトである
    {
        try
        {
            std::vector<int> v;
            v.at(42) = 1;
        }
        catch (const std::exception& e)
        {
            std::cerr << "問題発生: " << e.what() << std::endl;
        }
    }

    std::cin.get();
}


#include <iostream>
#include <iomanip>
#include <sstream>

int main()
{
	//boolalpha:bool値を文字列として入出力することを指示するマニピュレータ
	{
		std::cout << "boolalpha" << std::endl;
		std::cout << std::boolalpha << true << std::endl;
	}
	//noboolalpha:bool値を数値として入出力することを指示するマニピュレータ
	{
		std::cout << "noboolalpha" << std::endl;
		std::cout << std::boolalpha << false << std::endl;
		std::cout << std::noboolalpha << false << std::endl;
	}
	//showbase:整数出力時に基数を表すプレフィックスを付与することを指示するマニピュレータ
	{
		std::cout << "showbase" << std::endl;
		std::cout << std::hex;
		std::cout << 15 << ' ' << 0 << std::endl;
		std::cout << std::showbase << 15 << ' ' << 0 << std::endl;

		std::cout << std::noshowbase;

		std::cout << std::oct;
		std::cout << 15 << ' ' << 0 << std::endl;
		std::cout << std::showbase << 15 << ' ' << 0 << std::endl;
	}
	//noshowbase:数出力時に基数を表すプレフィックスを付与しないことを指示するマニピュレータ
	{
		std::cout << "noshowbase" << std::endl;
		std::cout << std::hex;
		std::cout << 15 << std::endl;
		std::cout << std::showbase << 15 << std::endl;
		std::cout << std::noshowbase << 15 << std::endl;
	}
	//showpoint:浮動小数点数出力時に小数点を必ず出力させることを指示するマニピュレータ
	{
		std::cout << "showpoint" << std::endl;
		std::cout << std::showpoint;
		std::cout << std::setprecision(1) << 1. << ' ' << 1.e-10 << std::endl;
		std::cout << std::setprecision(2) << 1. << ' ' << 1.e-10 << std::endl;
	}
	//noshowpoint:浮動小数点数出力時に小数点を不要なら出力させないことを指示するマニピュレータ
	{
		std::cout << "noshowpoint" << std::endl;
		std::cout << std::showpoint << std::setprecision(1);
		std::cout << 1. << ' ' << 1.e+100 << std::endl;
		std::cout << std::noshowpoint;
		std::cout << 1. << ' ' << 1.e+100 << std::endl;
		std::cout << 0.0001 << std::endl;
	}
	//showpos:数値出力時に符号を必ず出力させることを指示するマニピュレータ
	{
		std::cout << "showpos" << std::endl;
		std::cout << std::showpos << 1 << ' ' << 0 << std::endl;
	}
	//noshowpos:数値出力時に正符号を出力させないことを指示するマニピュレータ
	{
		std::cout << "noshowpos" << std::endl;
		std::cout << std::showpos << 1 << ' ' << 0 << ' ' << 1 << std::endl;
		std::cout << std::noshowpos << 1 << ' ' << 0 << ' ' << 1 << std::endl;
	}
	//skipws:書式入力を開始する際に空白の読み飛ばすことを指示するマニピュレータ
	{
		std::cout << "skipws" << std::endl;
		std::istringstream s("1 2");

		char a, b, c;
		s >> std::noskipws >> a >> b >> c;
		std::cout << a << b << c << std::endl;

		s.seekg(0, std::ios_base::beg);
		s >> std::skipws >> a >> b;
		std::cout << a << b << std::endl;
		
	}
	//uppercase:出力時に英大文字を使用することを指示するマニピュレータ
	{
		std::cout << "uppercase" << std::endl;
		std::cout << std::uppercase;
		std::cout << std::hex << std::showbase << 0xbeef << std::endl;
		std::cout << std::scientific << 1e+23 << std::endl;
		std::cout << std::hexfloat << 1234.5 << std::endl;
	}
	//nouppercase:出力時に英小文字を使用することを指示するマニピュレータ
	{
		std::cout << "nouppercase" << std::endl;
		std::cout << std::hex;
		std::cout << std::uppercase << 0xbeef << std::endl;
		std::cout << std::nouppercase << 0xbeef << std::endl;
	}
	//unitbuf:出力操作の都度バッファを吐き出すことを指示するマニピュレータ
	{
		std::cout << "unitbuf" << std::endl;
		std::ios_base::sync_with_stdio(false);
		std::clog.tie(nullptr);

		std::cout << std::unitbuf;

		std::cout << '1';
		std::clog << 'A';
		std::cout << '2';
		std::clog << 'B';
		std::cout << '3';
		std::clog << 'C';

		std::cout << std::endl;
		std::clog << std::endl;
	}
	//nounitbuf:unitbufの効果を取り消すことを指示するマニピュレータ
	{
		std::cout << "nounitbuf" << std::endl;
		std::cout << std::unitbuf << "test\n";
		std::cout << std::nounitbuf << "test\n";
	}
	//left center right:出力時に左揃えにすることを指示するマニピュレータ
	{
		std::cout << "left center right" << std::endl;
		std::cout << '|' << std::left << std::setw(6) << -12 << '|' << std::endl;
		std::cout << '|' << std::internal << std::setw(6) << -12 << '|' << std::endl;
		std::cout << '|' << std::right << std::setw(6) << -12 << '|' << std::endl;
	}
	//dec oct hex:整数をN進法で出力することを指示するマニピュレータ
	{
		std::cout << "dec oct hex" << std::endl;
		for (int i = 0; i < 20; ++i) {
			std::cout << std::dec << i << '\t';
			std::cout << std::oct << i << '\t';
			std::cout << std::hex << i << std::endl;
		}
	}
	//defaultfloat:浮動小数点数を初期状態の書式で出力することを指示するマニピュレータ
	{
		std::cout << std::right;
		std::cout << std::setw(12) << "defaultfloat" << ' ';
		std::cout << std::setw(17) << "fixed" << ' ';
		std::cout << std::setw(15) << "scientific" << ' ';
		std::cout << std::setw(15) << "hexfloat" << std::endl;
		for (double i = 1e-6; i < 1e10; i *= 20) {
			std::cout << std::setw(12) << std::defaultfloat << i << ' ';
			std::cout << std::setw(17) << std::fixed << i << ' ';
			std::cout << std::setw(15) << std::scientific << i << ' ';
			std::cout << std::setw(15) << std::hexfloat << i << std::endl;
		}
	}
	std::cin.get();
}

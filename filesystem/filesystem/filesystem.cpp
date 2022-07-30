#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <iostream>
#include <cassert>
#include <fstream>
#include <thread>



namespace fs = std::filesystem;


std::string datetime_to_string(fs::file_time_type tp)
{
    namespace chrono = std::chrono;
    auto sec = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch());

    std::time_t t = sec.count();
    const std::tm* lt = std::localtime(&t);

    std::ostringstream ss;
    ss << std::put_time(lt, "%c");
    return ss.str();
}
std::string file_type_to_string(fs::file_type type) {
    switch (type) {
    case fs::file_type::none:       return "none";
    case fs::file_type::not_found:  return "not found";
    case fs::file_type::regular:    return "regular file";
    case fs::file_type::directory:  return "directory file";
    case fs::file_type::symlink:    return "symbolic link file";
    case fs::file_type::block:      return "block special file";
    case fs::file_type::character:  return "character special file";
    case fs::file_type::fifo:       return "FIFO or pipe file";
    case fs::file_type::socket:     return "socket file";
    case fs::file_type::unknown:    return "unknown type file";
    default:
        return "implementation-defined file type";
    }
}
void print_space(const char* name, std::uintmax_t bytes)
{
    std::uintmax_t mega_bytes = bytes / (1024 * 1024);
    std::cout << name << " : " << bytes << "[B]"
        << " (" << mega_bytes << "[MB])" << std::endl;
}
int main()
{
    //pathは、ファイルシステムのパスを表すクラスである
    {
        //clear:パスを空にする
        {
            std::cout << "clear" << std::endl;
            fs::path p = "/usr/bin/clang";

            p.clear();
            if (p.empty()) {
                std::cout << "cleared" << std::endl;
            }
        }
        //make_preferred:ディレクトリの区切り文字を推奨する形式に変換する
        {
            std::cout << "make_preferred" << std::endl;
            // Windows環境では推奨のディレクトリ区切り文字が "\\" (バックスラッシュ)。
            // 環境非依存のディレクトリ区切り文字 "/" が "\\" に変換される
            fs::path p = "foo/bar";
            p.make_preferred();
        }
        //remove_filename:パスからファイル名を除去する
        {
            std::cout << "remove_filename" << std::endl;
			fs::path ps[] = {
			    "foo/bar", // ファイル名を含むパス (ディレクトリ区切り文字が残って "foo/" になる)
	            "foo/",    // ディレクトリパス
	            "/foo",    // ルートパス直下のファイルパス
	            "/"        // ルートパスのみ
			};

			for (fs::path& p : ps) {
				const fs::path before = p;

				p.remove_filename();
				std::cout << before << " : " << p << std::endl;
			}
        }
        //replace_filename:パスに含まれるファイル名を置き換える
        {
            std::cout << "replace_filename" << std::endl;
            fs::path ps[] = {
                "foo/bar.txt", // ファイル名を含むパス
                "foo/",        // ディレクトリパス
                "/"            // ルートパスのみ
            };

            for (fs::path& p : ps) {
                const fs::path before = p;

                p.replace_filename("a.md");
                std::cout << before << " : " << p << std::endl;
            }
        }
        //replace_extension:パスに含まれる拡張子を置き換える
        {
            std::cout << "replace_extension" << std::endl;
            fs::path p1 = "/foo.txt";
            p1.replace_extension(".md");
            std::cout << "p1 : " << p1 << std::endl;

            fs::path p2 = "/foo.txt";
            p2.replace_extension("md"); // ドットなしで拡張子を指定しても、ドットが付加される
            std::cout << "p2 : " << p2 << std::endl;

            fs::path p3 = "/foo"; // 拡張子がないファイル名に拡張子を付加する
            p3.replace_extension(".md");
            std::cout << "p3 : " << p3 << std::endl;
        }
        //swap:他のpathオブジェクトとデータを入れ替える
        {
            std::cout << "swap" << std::endl;
            fs::path a = "/foo/a.txt";
            fs::path b = "/foo/b.md";

            a.swap(b);

            std::cout << "a : " << a << std::endl;
            std::cout << "b : " << b << std::endl;
        }
        //native:システムの文字コードとしてパス文字列を取得する
        {
            std::cout << "native" << std::endl;
            fs::path p = "foo/bar";

            // UTF-16エンコーディングで返る
            const std::wstring& s = p.native();
            std::wcout << s << std::endl;
        }
        //c_str:システムの文字コードとしてC言語の文字列表現を取得する
        {
            std::cout << "c_str" << std::endl;
            fs::path p = "foo/bar";

            // UTF-16エンコーディングで返る
            const wchar_t* s = p.c_str();
            std::wcout << s << std::endl;
        }
        //wstring:システムのワイド文字コードで、パス文字列を取得する
        {
            std::cout << "wstring" << std::endl;
            fs::path p = "foo/bar";

            const std::wstring s = p.wstring();
            std::wcout << s << std::endl;
        }
        //u8string:UTF-8エンコードで、パス文字列を取得する
        {
            std::cout << "u8string" << std::endl;
            fs::path p = "foo/bar";
            auto s = p.u8string();

            // システムのマルチバイト文字コードからUTF-8に変換されたパス文字列が返される
            assert(s == u8"foo/bar");
        }
        //u16string:UTF-16エンコードで、パス文字列を取得する
        {
            std::cout << "u16string" << std::endl;
            fs::path p = "foo/bar";
            const std::u16string s = p.u16string();
        }
        //u32string:UTF-32エンコードで、パス文字列を取得する
        {
            std::cout << "u32string" << std::endl;
            fs::path p = "foo/bar";
            const std::u32string s = p.u32string();
        }
        //generic_string:指定された文字型に対応する文字コードで、環境非依存パスフォーマットのパス文字列を取得する
        {
            fs::path p = "foo\\bar"; // ネイティブフォーマットのパス (ディレクトリ区切り文字がバックスラッシュ)
            std::cout << "generic_string" << std::endl;
            const std::string s = p.generic_string<char>();
            const std::wstring ws = p.generic_string<wchar_t>();
            const std::u16string utf16s = p.generic_string<char16_t>();
            const std::u32string utf32s = p.generic_string<char32_t>();

            std::cout << s << std::endl;
        }
        //root_name:パスが保持しているルート名を取得する
        {
            std::cout << "root_name" << std::endl;
            fs::path p = "C:/Program Files/a.txt";
            fs::path root_name = p.root_name();

            std::cout << root_name << std::endl;
        }
        //root_directory:パスが保持しているルートディレクトリを取得する
        {
            std::cout << "root_directory" << std::endl;
            fs::path p = "C:/Program Files/a.txt";
            fs::path root_dir = p.root_directory();

            std::cout << root_dir << std::endl;
        }
        //root_path:パスが保持しているルートパスを取得する
        {
            std::cout << "root_path" << std::endl;
            fs::path p = "C:/Program Files/a.txt";
            fs::path root_p = p.root_path();

            std::cout << root_p << std::endl;
        }
        //relative_path:ルートパスからの相対パスを取得する
        {
            std::cout << "relative_path" << std::endl;
            fs::path p = "C:/Program Files/a.txt";

            fs::path root_p = p.root_path();
            fs::path rel_p = p.relative_path();

            std::cout << root_p << std::endl;
            std::cout << rel_p << std::endl;
        }
        //parent_path:親のパスを取得する
        {
            std::cout << "parent_path" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt", // ファイル名を含むパス
                "C:/foo/bar/",    // ディレクトリパス
                "C:/"             // ルートパスのみ (ルートパスの親はルートパスなのでそのまま返る)
            };

            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.parent_path() << std::endl;
            }
        }
        //filename:パスが保持しているファイル名を取得する
        {
            std::cout << "filename" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt", // ファイル名を含むパス
                "C:/foo/bar/",    // ディレクトリパス
                "C:",             // ルートディレクトリ
                ".",              // カレントディレクトリ
                ".."              // 親ディレクトリ
            };

            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.filename() << std::endl;
            }
        }
        //stem:パスが保持している、拡張子を除いたファイル名を取得する
        {
            std::cout << "stem" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt",    // ファイル名を含むパス
                "C:/foo/bar.tar.gz", // ファイル名に複数のドットが含まれるパス
                "C:/foo/",           // ディレクトリパス
                "C:/foo/.",          // カレントディレクトリ
                "..",                // 親ディレクトリ
                ".file",             // UNIX系OSでの隠しファイル (拡張子なし)
            };

            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.stem() << std::endl;
            }
        }
        //extension:パスが保持している拡張子を取得する
        {
            std::cout << "extension" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt",    // ファイル名を含むパス
                "C:/foo/bar.tar.gz", // ファイル名に複数のドットが含まれるパス
                "C:/foo/",           // ディレクトリパス
                "C:/foo/.",          // カレントディレクトリ
                "..",                // 親ディレクトリ
                ".file",             // UNIX系OSでの隠しファイル (拡張子なし)
            };

            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.extension() << std::endl;
            }
        }
        //empty:パスが空か判定する
        {
            std::cout << "empty" << std::endl;
            fs::path p1;
            if (p1.empty()) {
                std::cout << "p1 : empty" << std::endl;
            }

            fs::path p2 = "/usr/bin/clang";
            if (!p2.empty()) {
                std::cout << "p2 : not empty" << std::endl;
            }
        }
        //has_root_name:パスにルート名が含まれているか判定する
        {
            std::cout << "has_root_name" << std::endl;
            fs::path p = "C:/Program Files/a.txt";

            fs::path root_name = p.root_name();
            std::cout << root_name << std::endl;

            if (p.has_root_name()) {
                std::cout << "has root name" << std::endl;
            }
            else {
                std::cout << "doesn't have root name" << std::endl;
            }
        }
        //has_root_directory:パスにルートディレクトリが含まれているか判定する
        {
            std::cout << "has_root_directory" << std::endl;
            fs::path p = "C:/Program Files/a.txt";

            fs::path root_directory = p.root_directory();
            std::cout << root_directory << std::endl;

            if (p.has_root_directory()) {
                std::cout << "has root directory" << std::endl;
            }
            else {
                std::cout << "doesn't have root directory" << std::endl;
            }
        }
        //has_root_path:パスにルートパスが含まれているか判定する
        {
            std::cout << "has_root_path" << std::endl;
            fs::path p = "C:/Program Files/a.txt";

            fs::path root_path = p.root_path();
            std::cout << root_path << std::endl;

            if (p.has_root_path()) {
                std::cout << "has root path" << std::endl;
            }
            else {
                std::cout << "doesn't have root path" << std::endl;
            }
        }
        //has_relative_path:パスにルートパスからの相対パスが含まれているか判定する
        {
            std::cout << "has_relative_path" << std::endl;
            fs::path p = "C:/Program Files/a.txt";

            fs::path root_p = p.root_path();
            fs::path rel_p = p.relative_path();
            std::cout << "root path : " << root_p << std::endl;
            std::cout << "relative_path : " << rel_p << std::endl;

            if (p.has_relative_path()) {
                std::cout << "has relative path from root path" << std::endl;
            }
            else {
                std::cout << "doesn't have relative path from root path" << std::endl;
            }
        }
        //has_parent_path:パスに親パスが含まれているか判定する
        {
            std::cout << "has_parent_path" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt", // ファイル名を含むパス
                "C:/foo/bar/",    // ディレクトリパス
                "C:/",            // ルートパスのみ
                "bar/",         // ディレクトリパス
                "bar"           // ファイルパス
            };

            std::cout << std::boolalpha;
            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.has_parent_path() << std::endl;
            }
        }
        //has_filename:パスにファイル名が含まれているか判定する
        {
            std::cout << "has_filename" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt", // ファイル名を含むパス
                "C:/foo/bar/",    // ディレクトリパス
                "C:",             // ルートディレクトリ
                ".",              // カレントディレクトリ
                ".."              // 親ディレクトリ
            };

            std::cout << std::boolalpha;
            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.has_filename() << std::endl;
            }
        }
        //has_stem:パスに、拡張子を除いたファイル名が含まれているか判定する
        {
        std::cout << "has_stem" << std::endl;
        fs::path ps[] = {
            "C:/foo/bar.txt",    // ファイル名を含むパス
            "C:/foo/bar.tar.gz", // ファイル名に複数のドットが含まれるパス
            "C:/foo/",           // ディレクトリパス
            "C:/foo/.",          // カレントディレクトリ
            "..",                // 親ディレクトリ
        };

        std::cout << std::boolalpha;
        for (const fs::path& p : ps) {
            std::cout << p << " : " << p.has_stem() << std::endl;
        }
        }
        //has_extension:パスに拡張子が含まれているか判定する
        {
            std::cout << "has_extension" << std::endl;
            fs::path ps[] = {
                "C:/foo/bar.txt",    // ファイル名を含むパス
                "C:/foo/bar.tar.gz", // ファイル名に複数のドットが含まれるパス
                "C:/foo/",           // ディレクトリパス
                "C:/foo/.",          // カレントディレクトリ
                "..",                // 親ディレクトリ
            };

            std::cout << std::boolalpha;
            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.has_extension() << std::endl;
            }
        }
        //is_absolute:パスが絶対パスかを判定する
        {
            std::cout << "is_absolute" << std::endl;
            fs::path ps[] = {
               "C:",                // ルート名のみ
               "C:/",               // ルートパスのみ
               "C:/foo/bar.txt",    // ルートパスを含む
               "C:/foo/../bar.txt", // ルートパスに加えて、親ディレクトリの参照を含む
               "foo/bar.txt",       // ルートパスを含まない
               "C:foo",             // ルート名はあるがルートディレクトリはない
               "/foo"               // ルートディレクトリはあるがルート名はない
            };

            std::cout << std::boolalpha;
            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.is_absolute() << std::endl;
            }
        }
        //is_relative:パスが相対パスかを判定する
        {
            std::cout << "is_relative" << std::endl;
            fs::path ps[] = {
                "C:",                // ルート名のみ
                "C:/",               // ルートパスのみ
                "C:/foo/bar.txt",    // ルートパスを含む
                "C:/foo/../bar.txt", // ルートパスに加えて、親ディレクトリの参照を含む
                "foo/bar.txt",       // ルートパスを含まない
                "C:foo",             // ルート名はあるがルートディレクトリはない
                "/foo"               // ルートディレクトリはあるがルート名はない
            };

            std::cout << std::boolalpha;
            for (const fs::path& p : ps) {
                std::cout << p << " : " << p.is_relative() << std::endl;
            }
        }
        //lexically_normal:文字列レベルで正規化する
        {
            std::cout << "lexically_normal" << std::endl;
            // 自身のディレクトリを表す "./" を削除
            // 親ディレクトリを指す ".." があるため、 "bar/.." を削除
            assert(fs::path("foo/./bar/..").lexically_normal().generic_string() == "foo/");

            // ひとつ前の例とほぼ同じだが、 ./// のようにディレクトリ区切り文字がいくつあっても、
            // ひとつと等価の意味を持つため、まとめて削除される
            assert(fs::path("foo/.///bar/../").lexically_normal().generic_string() == "foo/");

            // (ファイルシステムを介することのない) 文字列だけでは親ディレクトリを解決できないため、
            // 正規化してもそのまま。
            // 末尾のディレクトリ区切り文字だけ削除される
            assert(fs::path("../../").lexically_normal().generic_string() == "../..");

            // 正規化の結果として空文字列になるため、 "." のみのパスとなる
            assert(fs::path("foo/..").lexically_normal().generic_string() == ".");
        }
        //lexically_relative:文字列レベルで相対パスに変換する
        {
            std::cout << "lexically_relative" << std::endl;
            assert(fs::path("/a/d").lexically_relative("/a/b/c") == "../../d");
            assert(fs::path("/a/b/c").lexically_relative("/a/d") == "../b/c");
            assert(fs::path("a/b/c").lexically_relative("a") == "b/c");
            assert(fs::path("a/b/c").lexically_relative("a/b/c/x/y") == "../..");
            assert(fs::path("a/b/c").lexically_relative("a/b/c") == ".");
            assert(fs::path("a/b").lexically_relative("c/d") == "../../a/b");
        }
    }
    //filesystem_errorクラスは、ファイルシステムの操作で発生したエラーを扱う例外クラスである
	{
        std::cout << "filesystem_error" << std::endl;
	    assert(!fs::exists("a/from.txt"));

	    // 存在しないファイルをコピーしようとした
	    try {
		    fs::copy_file("a/from.txt", "b/to.txt");
	    }
	    catch (fs::filesystem_error& err) {
		    std::cout << err.what() << std::endl;
	    }
	}
    //directory_entry:directory_entryは、directory_iteratorクラスもしくはrecursive_directory_iteratorクラスによってディレクトリ走査をした際に取得できる、ディレクトリ内要素を表すクラスである
    {
        //assign:パスを再代入する
        {
            std::cout << "assign" << std::endl;
            fs::create_directory("dir");
            std::ofstream{ "dir/a.txt" };
            std::ofstream{ "dir/b.txt" };

            fs::directory_entry x{ "dir/a.txt" };
            x.assign("dir/b.txt");

            std::cout << x.path() << std::endl;
        }
        //replace_filename:ファイル名を置き換える
        {
            std::cout << "replace_filename" << std::endl;
            fs::create_directory("dir");
            std::ofstream{ "dir/a.txt" };
            std::ofstream{ "dir/b.txt" };

            fs::directory_entry x{ "dir/a.txt" };
            x.replace_filename("b.txt");

            std::cout << x.path() << std::endl;
        }
        //refresh:キャッシュを更新する
        {
            std::cout << "refresh" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            for (fs::directory_entry x : fs::directory_iterator("dir")) {

                // 計算をしている間にファイルの状態が変わっているかもしれないので、キャッシュを更新する
                x.refresh();
            }
        }
        //path:パスを取得する
        {
            std::cout << "path" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            // dirディレクトリ内のファイルを列挙する
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << std::endl;
            }
        }
        //exists:パスが存在するか確認する
        {
            std::cout << "exists" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.exists() << std::endl;
            }
        }
        //file_size:ファイルサイズを取得する
        {
            std::cout << "file_size" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            {
                std::ofstream file{ "dir/a.txt", std::ios::binary };
                std::uint32_t value = 42;
                file.write(reinterpret_cast<char*>(&value), sizeof(value));
            }

            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                if (x.is_regular_file()) {
                    std::cout << x.path() << " : " << x.file_size() << std::endl;
                }
            }
        }
        //hard_link_count:ハードリンク数を取得する
        {
            std::cout << "hard_link_count" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.hard_link_count() << std::endl;
            }
        }
        //is_regular_file:通常ファイルを指しているか確認する
        {
            std::cout << "is_regular_file" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_regular_file() << std::endl;
            }
        }
        //is_directory:ディレクトリを指しているか確認する
        {
            std::cout << "is_directory" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_directory() << std::endl;
            }
        }
        //is_symlink:シンボリックリンクを指しているか確認する
        {
            //std::cout << "is_symlink" << std::endl;
            //fs::create_directory("dir");
            //fs::create_directory("dir/inner_dir");
            //std::ofstream{ "dir/a.txt" };
            //fs::create_symlink("dir/a.txt", "dir/a.symlink");

            //std::cout << std::boolalpha;
            //for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
            //    std::cout << x.path() << " : " << x.is_symlink() << std::endl;
            //}
        }
        //is_block_file:ブロックデバイスのスペシャルファイルを指しているか確認する
        {
            std::cout << "is_block_file" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_block_file() << std::endl;
            }
        }
        //is_character_file:キャラクタデバイスのスペシャルファイルを指しているか確認する
        {
            std::cout << "is_character_file" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_character_file() << std::endl;
            }
        }
        //is_fifo:FIFOまたはパイプを指しているか確認する
        {
            std::cout << "is_fifo" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_fifo() << std::endl;
            }
        }
        //is_socket:ソケットを指しているか確認する
        {
            std::cout << "is_socket" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_socket() << std::endl;
            }
        }
        //is_other:パスが存在していない、もしくはシステム依存の種別のファイルを指しているかを確認する
        {
            std::cout << "is_other" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            std::cout << std::boolalpha;
            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << x.is_other() << std::endl;
            }
        }
        //last_write_time:最終更新日時を取得する
        {
            std::cout << "last_write_time" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                std::cout << x.path() << " : " << datetime_to_string(x.last_write_time()) << std::endl;
            }
        }
        //status:ファイル状態を取得する
        {
            std::cout << "status" << std::endl;
            fs::create_directory("dir");
            fs::create_directory("dir/inner_dir");
            std::ofstream{ "dir/a.txt" };

            for (const fs::directory_entry& x : fs::directory_iterator("dir")) {
                fs::file_status st = x.status();
                std::cout << x.path() << " : " << file_type_to_string(st.type()) << std::endl;
            }
        }

    }
    //filesystem::file_statusは、ファイル状態を表すクラスである。
    {
        std::cout << "file_status" << std::endl;
        std::ofstream{ "a.txt" };

        fs::file_status status = fs::status("a.txt");

        assert(status.type() == fs::file_type::regular);
        assert((status.permissions()& fs::perms::owner_write) == fs::perms::owner_write);
    }
    //space_infoは、ディスク容量を表すクラスである
    {
        std::cout << "space_info" << std::endl;
        fs::path p = "/";
        fs::space_info info = fs::space(p);

        std::cout << p << std::endl;
        print_space("capacity", info.capacity);
        print_space("free", info.free);
        print_space("available", info.available);
    }
    //current_path:現在の作業ディレクトリを取得・設定する
    {
        std::cout << "current_path" << std::endl;
        // 現在の作業ディレクトリを取得
        fs::path p = fs::current_path();
        std::cout << p << std::endl;

        // 現在の作業ディレクトリを設定
        fs::current_path("/");
        std::cout << fs::current_path() << std::endl;
    }
    //temp_directory_path:一時ファイル用ディレクトリのパスを取得する
    {
        std::cout << "temp_directory_path" << std::endl;
        fs::path p = fs::temp_directory_path();
        std::cout << p << std::endl;
    }
    //absolute:パスを絶対パスに変換する
    {
        std::cout << "absolute" << std::endl;
        // 現在の作業ディレクトリ直下の (存在しない) ファイル"a.txt"の、絶対パスを取得する
        fs::path p = fs::absolute("a.txt");
        std::cout << p << std::endl;
    }
    //canonical:パスを正規化する
    {
        //std::cout << "canonical" << std::endl;
        //std::ofstream{ "a.txt" };
        //fs::create_directory("dir");

        //// "." (ドットx1) を解決
        //fs::path p1 = fs::canonical("./a.txt");
        //std::cout << p1 << std::endl;

        //// ".." (ドットx2) を解決
        //fs::path p2 = fs::canonical("dir/../a.txt");
        //std::cout << p2 << std::endl;
    }
    //weakly_canonical:パスを正規化する
    {
        //std::cout << "weakly_canonical" << std::endl;
        //std::ofstream{ "a.txt" };
        //fs::create_directory("dir");

        //// "." (ドットx1) を解決
        //fs::path p1 = fs::weakly_canonical("./a.txt");
        //std::cout << p1 << std::endl;

        //// ".." (ドットx2) を解決
        //fs::path p2 = fs::weakly_canonical("dir/../a.txt");
        //std::cout << p2 << std::endl;

        //// 存在しないファイルへの相対パス文字を解決
        //fs::path p3 = fs::weakly_canonical("dir/../b.txt");
        //std::cout << p3 << std::endl;
    }
    //read_symlink:シンボリックリンクが指すパスを取得する
    {
        //std::cout << "read_symlink" << std::endl;
        //std::ofstream{ "regular.txt" };
        //fs::create_symlink("regular.txt", "regular.symlink");

        //fs::path result = fs::read_symlink("regular.symlink");
        //std::cout << result << std::endl;
    }
    //relative:パスを現在の作業ディレクトリからの相対パスに変換する
    {
        std::cout << "relative" << std::endl;
        // 現在の作業ディレクトリからの相対パスに変換する
        fs::path p1 = fs::relative("/a.txt");
        std::cout << p1 << std::endl;

        // 現在の作業ディレクトリからひとつ上の階層からの相対パスに変換する
        fs::path p2 = fs::relative("/a.txt", "../");
        std::cout << p2 << std::endl;
    }
    std::cin.get();
}

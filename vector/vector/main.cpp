#include <iostream>
#include <vector>

using namespace std;
/*ベクトルは動的配列と同じであり、要素が挿入または削除されたときに
自動的にサイズを変更する機能があり、ストレージはコンテナによって自動的に処理されます。
vectorのメモリ確保参考：http://www.kmonos.net/wlog/111.html#_2334100705*/

int main()
{
    vector<int> g1;


    for (int i = 1; i <= 10; i++)
        g1.push_back(i);

    cout << "Output of begin and end: ";

    for (auto i = g1.begin(); i != g1.end(); ++i) //begin-endオーソドックスなシーケンサ
        cout << *i << " ";

    cout << "\nOutput of cbegin and cend: "; //cbegin--cendは定数を返す。列挙するためだけに走査する場合、cを使う。
    for (auto i = g1.cbegin(); i != g1.cend(); ++i)
        cout << *i << " ";

    cout << "\nOutput of rbegin and rend: "; //reverse
    for (auto ir = g1.rbegin(); ir != g1.rend(); ++ir)
        cout << *ir << " ";

    cout << "\nOutput of crbegin and crend : "; //const reverse
    for (auto ir = g1.crbegin(); ir != g1.crend(); ++ir)
        cout << *ir << " ";

    cout << "\nsize : " << g1.size(); //要素数
    cout << "\nmax_size : " << g1.max_size(); //格納可能な最大の要素数
    cout << "\ncapacity :" << g1.capacity();  //メモリを再確保せずに格納できる最大の要素数
    cout << "\noperator[] : " << g1[g1.size() -1];     //要素アクセス。at()メンバ関数とちがって境界チェックを行うことが規定されない。
    cout << "\nat : " << g1.at(g1.size() -1);          //要素アクセス。要素外にアクセス時の例外スローが保証されている。チェックが入る分パフォーマンスは落ちるはず。
    g1.shrink_to_fit();
    cout << "\nshrink :" << g1.capacity();    //capacityをsizeまで縮小する
    
    int* p = g1.data();

    cout << "\ndata :"  << *p; //先頭要素のポインタ

    g1.pop_back();
    cout << "\nsize : " << g1.size(); //要素数

    g1.insert(g1.begin() + 1, {80, 50 }); //挿入
    erase(g1, 50); //削除
    erase_if(g1, [](int x) { return x > 20; }); //条件付き削除

    for (auto i = g1.cbegin(); i != g1.cend(); ++i)
        cout << *i << " ";

    
    g1.clear();
    cout << "\nclear after size : " << g1.size() << " empty : " << g1.empty();

    cin.get();
    return 0;
}
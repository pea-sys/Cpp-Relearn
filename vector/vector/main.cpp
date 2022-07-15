#include <iostream>
#include <vector>

using namespace std;
/*�x�N�g���͓��I�z��Ɠ����ł���A�v�f���}���܂��͍폜���ꂽ�Ƃ���
�����I�ɃT�C�Y��ύX����@�\������A�X�g���[�W�̓R���e�i�ɂ���Ď����I�ɏ�������܂��B
vector�̃������m�ێQ�l�Fhttp://www.kmonos.net/wlog/111.html#_2334100705*/

int main()
{
    vector<int> g1;


    for (int i = 1; i <= 10; i++)
        g1.push_back(i);

    cout << "Output of begin and end: ";

    for (auto i = g1.begin(); i != g1.end(); ++i) //begin-end�I�[�\�h�b�N�X�ȃV�[�P���T
        cout << *i << " ";

    cout << "\nOutput of cbegin and cend: "; //cbegin--cend�͒萔��Ԃ��B�񋓂��邽�߂����ɑ�������ꍇ�Ac���g���B
    for (auto i = g1.cbegin(); i != g1.cend(); ++i)
        cout << *i << " ";

    cout << "\nOutput of rbegin and rend: "; //reverse
    for (auto ir = g1.rbegin(); ir != g1.rend(); ++ir)
        cout << *ir << " ";

    cout << "\nOutput of crbegin and crend : "; //const reverse
    for (auto ir = g1.crbegin(); ir != g1.crend(); ++ir)
        cout << *ir << " ";

    cout << "\nsize : " << g1.size(); //�v�f��
    cout << "\nmax_size : " << g1.max_size(); //�i�[�\�ȍő�̗v�f��
    cout << "\ncapacity :" << g1.capacity();  //���������Ċm�ۂ����Ɋi�[�ł���ő�̗v�f��
    cout << "\noperator[] : " << g1[g1.size() -1];     //�v�f�A�N�Z�X�Bat()�����o�֐��Ƃ������ċ��E�`�F�b�N���s�����Ƃ��K�肳��Ȃ��B
    cout << "\nat : " << g1.at(g1.size() -1);          //�v�f�A�N�Z�X�B�v�f�O�ɃA�N�Z�X���̗�O�X���[���ۏ؂���Ă���B�`�F�b�N�����镪�p�t�H�[�}���X�͗�����͂��B
    g1.shrink_to_fit();
    cout << "\nshrink :" << g1.capacity();    //capacity��size�܂ŏk������
    
    int* p = g1.data();

    cout << "\ndata :"  << *p; //�擪�v�f�̃|�C���^

    g1.pop_back();
    cout << "\nsize : " << g1.size(); //�v�f��

    g1.insert(g1.begin() + 1, {80, 50 }); //�}��
    erase(g1, 50); //�폜
    erase_if(g1, [](int x) { return x > 20; }); //�����t���폜

    for (auto i = g1.cbegin(); i != g1.cend(); ++i)
        cout << *i << " ";

    
    g1.clear();
    cout << "\nclear after size : " << g1.size() << " empty : " << g1.empty();

    cin.get();
    return 0;
}
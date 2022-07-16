// function template
#include <iostream>
using namespace std;
/*
�e���v���[�g�́A�W�F�l���b�N�^�œ���ł�����ʂȊ֐��ł��B����ɂ��A�^�C�v���ƂɃR�[�h�S�̂��J��Ԃ����ƂȂ��A
�@�\�𕡐��̃^�C�v�܂��̓N���X�ɓK�������邱�Ƃ��ł���֐��e���v���[�g���쐬�ł��܂��B
*/

//�֐��e���v���[�g�F�L�@�͂ǂ���ł��ǂ�
//template <class identifier> function_declaration;
//template <typename identifier> function_declaration;

// �R���Z�v�g�Ńe���v���[�g�Ɍ^�����������(C#��where)
// �^���Z�p�^�ł��邱�Ƃ�v������
//https://cpprefjp.github.io/reference/type_traits/is_arithmetic.html
template<class NumericType>
concept Numeric = (is_arithmetic<NumericType>::value == true);

template <Numeric T>
requires equality_comparable<T> //�W���I�ȃR���Z�v�g�͕W�����C�u�����Œ񋟂���Ă���̂ŁA�܂��͂�������T���̂��ǂ�
T GetMax(T a, T b) {
	T result;
	result = (a > b) ? a : b;
	return (result);
}

//�N���X�e���v���[�g�F
template <class T, int N>
class mysequence {
	T memblock[N];
public:
	void setmember(int x, T value);
	T getmember(int x);
};

template <class T, int N>
void mysequence<T, N>::setmember(int x, T value) {
	memblock[x] = value;
}

template <class T, int N>
T mysequence<T, N>::getmember(int x) {
	return memblock[x];
}


int main() {
	//�e���v���[�g�֐��̌Ăяo��
	int i = 5, j = 6, k;
	long l = 10, m = 5, n;
	double p = 2.2, q = 0.8, r;
	char x = 'a', y = 'b', z;
	k = GetMax<int>(i, j);
	n = GetMax<long>(l, m);
	r = GetMax<double>(p, q);
	z = GetMax<char>(x, y);
	cout << k << endl;
	cout << n << endl;
	cout << r << endl;
	cout << z << endl;

	//�e���v���[�g�N���X�̌Ăяo��
	mysequence <int, 5> myints;
	mysequence <double, 5> myfloats;
	myints.setmember(0, 100);
	myfloats.setmember(3, 3.1416);
	cout << myints.getmember(0) << '\n';
	cout << myfloats.getmember(3) << '\n';
	cin.get();
	return 0;
}
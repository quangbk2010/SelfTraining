#include <iostream>
#include <vector>
using namespace std;

template <class T> void Push_to_vector (vector<T> *v);
template <class T> void Print_vector (vector<T> v);
template <class T1, class T2> void Print_vector (vector<T1> v1, vector<T2> v2 );

int main(int argc, char const *argv[])
{
	vector<int> v1(6);
	vector<char> v2 = {'a', 'b', 'c'};

	Print_vector (v1, v2);
	
	Push_to_vector (&v1);
	Print_vector (v1);

	return 0;
}
template <class T> void Push_to_vector (vector<T> *v) {
	cout << "Push elements to v:" << endl;
	// 1st way
	for (int i = 0; i < 6; ++i)
	{
		(*v).push_back (i);
	}
	Print_vector (*v);
	// 2nd way
	for (int i = 0; i < 4; ++i)
	{
		(*v).at(i) = i + 1;
	}
	Print_vector (*v);
	// 3rd way
	(*v).front() = 8;
	(*v).back() = 9;
	Print_vector (*v);

	// 4th way
	T *pT = (*v).data();
	*pT = 10;
	pT ++;
	pT[4] = 11;
	
}
template <class T> void Print_vector (vector<T> v) {

	cout << "Length of v: " << v.size() << endl;
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << "\t";
	}
	cout << endl;
}

template <class T1, class T2> void Print_vector (vector<T1> v1, vector<T2> v2 ) {

	cout << "Length of v1: " << v1.size() << endl;
	for (int i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << "\t";
	}
	cout << endl;

	cout << "Length of v2: " << v2.size() << endl;
	cout << "Begin of v2: " << *(v2.begin()) << endl;
	cout << "End of v2: " << *(--v2.end()) << endl;
	// It is ok with these lines
	//cout << "Begin of v2: " << *v2.begin() << endl;
	//cout << "End of v2: " << *--v2.end() << endl;
	cout << "Elements of v2:" << endl;
	for (int i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << "\t";
	}
	cout << endl;
}

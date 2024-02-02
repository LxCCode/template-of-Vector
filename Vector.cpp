#include"Vector.hpp"

int main()
{
	Vector<char> s1(100, 10, 0);
	for (int i = 0; i < 10; i++)
	{
		cin >> s1[i];
	}
	s1.deduplicate();
	cout << s1.size() << endl;
	Vector<char> s2 = s1;
	s2.sort();
	cout << s2.search('e') << endl;
	s2.remove(2, 5);
	cout << s2.size() << endl;
}

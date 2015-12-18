#include <iostream>
#include <fstream>

#include "karkkainen_sanders.h"
using namespace std;
int main() {
	string s;
	cin >> s;
	vector<unsigned int> v = getSuffixArray(s);
	for (auto a : v) {
		cout << a << " ";
	}
	cout << "\n";
	return 0;
}

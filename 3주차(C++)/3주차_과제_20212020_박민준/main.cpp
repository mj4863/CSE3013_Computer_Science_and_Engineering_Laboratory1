#include "str.h"
#include <iostream>
using namespace std;

int main(void) {
	Str a("I'm a girl");
	cout << a.contents();
	a="I'm a boy\n";
	cout << a.contents();
	cout << a.compare("I'm a a") << endl;
	return 0;
}

#include <iostream>
#include <chrono>
#include "BigNum.h"

using namespace std;

int main() {

	BigNum A("123");
	BigNum B("431293158491964912412");
	cout << A.NumAsString() << endl;
	cout << B.NumAsString() << endl;

	return 0;
}
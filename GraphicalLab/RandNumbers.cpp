#include <ctime>
#define m 1000003

unsigned int rx = time(0);

long long inverse(long long b) {
	long long ex = m - 2;
	if (b == 1)return 1;
	long long r = 1;
	while (ex) {
		if (ex & 1)r = (r * b) % m;
		ex = ex >> 1;
		b = (b * b) % m;
	}
	return r;
}

unsigned int randMethod() {
	rx = (5 * inverse(rx) + 2) % m;
	return rx;
}

double getRandNum() {
	return (double)randMethod() / m;
}
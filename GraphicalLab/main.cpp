#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "interface.h"

using namespace std;

struct abc {
	abc() {};
	abc(abc*a) :next(a) {};
	~abc() {
		cout << "Destructing" << endl;
	}
	int r = 9;
	abc * next = nullptr;
};

void func(abc** a) {
	abc * b = new abc;
	*a = b;
	cout << "Inside: " << (*a)->r << endl;
}

void main(int argc, char * argv[]){
	glutInit(&argc, argv);

	abc* d=nullptr;
	func(&d);
	cout << d->r << endl;
	startPresentation();
	system("pause");
}
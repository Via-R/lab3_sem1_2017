//#include <vector>
#include <iostream>
#include "Voronoi.h"
using namespace std;
double getRandNum();

EventsVect evQueue;
LeavesCont evTree;
PointsVect initPoints;

float sweepLine;

int evCount = 0;
/*------------------------------------------------------------------------------------*/

void customSet() {
	initPoints.clear();
	initPoints.push_back(Point(0, 1));
	initPoints.push_back(Point(1, 0));
	initPoints.push_back(Point(0, -1));
	printPoints();
}

void randomize(const unsigned int iterations, const unsigned int border) {
	initPoints.clear();
	float rx, ry;
	for (int i = 0; i < iterations; i++) {
		rx = getRandNum() * 2 * border - border;
		ry = getRandNum() * 2 * border - border;
		initPoints.push_back(Point(rx, ry));
	}
	printPoints();
}

void printPoints() {
	cout << "Printing points:\n";
	for (PointsVect::const_iterator i = initPoints.cbegin(); i != initPoints.cend(); ++i) {
		cout << "-----------------\n";
		cout << "x: " << (*i).x << "\ny: " << (*i).y << "\n-----------------\n";
	};
}

void initializeQueue() {
	for (PointsVect::const_iterator i = initPoints.cbegin(); i != initPoints.cend(); ++i) {
		evQueue.push(Event(*i));
	}
	sweepLine = evQueue.top().y;
	evTree.push_back(Leaf(evQueue.top(), evCount++));
	evQueue.pop();
}

bool getArchIntersection(float x1, float y1, float x2, float y2, Point& r1, Point& r2) {
	cout << "\n\nGetting arch intersections:\n";
	//ax^2 + bx + c
	float a = 1. / (2. * y1 - 2. * sweepLine);
	float b = -x1 / (y1 - sweepLine);
	float c = (x1 * x1 + y1 * y1 - sweepLine * sweepLine) / (2. * y1 - 2. * sweepLine);

	//nx^2 + mx + p
	float n = 1. / (2. * y2 - 2. * sweepLine);
	float m = -x2 / (y2 - sweepLine);
	float p = (x2 * x2 + y2 * y2 - sweepLine * sweepLine) / (2. * y2 - 2. * sweepLine);

	printf("a: %f, b: %f, c: %f\n", a, b, c);
	printf("n: %f, m: %f, p: %f\n", n, m, p);
	float k = a - n;
	float l = b - m;
	float t = c - p;

	float D = l*l - 4. * k*t;
	if (D < 0)
		return false;
	float rx1 = (-l + sqrt(D)) / (2. * k);
	float rx2 = (-l - sqrt(D)) / (2. * k);
	float ry1 = a*rx1*rx1 + b*rx1 + c;
	float ry2 = a*rx2*rx2 + b*rx2 + c;
	r1.x = rx1;
	r1.y = ry1;
	r2.x = rx2;
	r2.y = ry2;
	printf("x1: %f, y1: %f\nx2: %f, y2: %f\n", rx1, ry1, rx2, ry2);
	cout << "Arch intersection ended\n\n";
	/*float k = n / a;
	b *= k;
	c *= k;

	float x3 = -(c - p) / (b - m);
	float y3 = n*x3*x3 + m*x3 + p;*/
	//return Point(x3, y3);
}

float getArchNLineIntersection(float x, float y, float lx) {
	cout << "Arch N Line intersection started\n\n";
	float a = 1. / (2. * y - 2. * sweepLine);
	float b = -x / (y - sweepLine);
	float c = (x * x + y * y - sweepLine * sweepLine) / (2. * y - 2. * sweepLine);
	cout << "x: " << lx << endl;

	cout << "Ended\n\n";
	return (a * lx * lx + b * lx + c);
}

void printTree() {
	cout << "\n\nEvents tree:\n";
	for (auto i = evTree.cbegin(); i != evTree.cend(); ++i) {
		cout << ">------------<\n";
		cout << "IsPoint: " << i->isPoint << endl;
		cout << "Number: " << i->n << endl;
		cout << "X: " << i->x << endl;
		cout << "Y: " << i->y << endl;
		cout << ">------------<\n";
	}
	cout << "Print ended\n\n";
}

void recalculateBreakpoints() {
	for (auto i = evTree.begin(); i != evTree.end(); ++i) {
		if (i->isPoint || i->corrArch == nullptr)
			continue;
		Point l, r;
		getArchIntersection(i->corrArch->x, i->corrArch->y, i->x, i->y, l, r);
		--i;
		i->x = l.x;
		i->y = l.y;
		++(++i);
		i->x = r.x;
		i->y = r.y;
		--i;
	}
}

LeavesCont::iterator getArchToBreak() {
	float x = evQueue.top().x;
	for (auto i = evTree.begin(); i != evTree.end(); ++i) {
		if (!i->isPoint)
			continue;
		if (i->x >= x)
			return --i;
	}
	return evTree.end();
}

Point getNextCircleEvent(Leaf a, Leaf b, Leaf c) {
	cout << "Inside finding circle event\n";
	cout << "First dot: " << a.x << " " << a.y << endl;
	cout << "Second dot: " << b.x << " " << b.y << endl;
	cout << "Third dot: " <<c.x << " " << c.y << endl;
	float midp1x = (b.x + a.x) / 2.;
	float midp1y = (b.y + a.y) / 2.;
	float koef1 = b.x - a.x == 0 ? 0 : -1. / ((b.y - a.y) / (b.x - a.x));
	float b1 = midp1y - koef1 * midp1x;

	float midp2x = (c.x + b.x) / 2.;
	float midp2y = (c.y + b.y) / 2.;
	float koef2 = c.x - b.x == 0 ? 0 : -1. / ((c.y - b.y) / (c.x - b.x));
	float b2 = midp2y - koef2 * midp2x;

	float rx, ry;
	rx = (b2 - b1) / (koef1 - koef2);
	ry = koef1 * rx + b1;

	float r = sqrt((a.x - rx)*(a.x - rx) + (a.y - ry)*(a.y - ry));
	cout << "rx: " << rx << ", ry: " << ry << ", r: " << r << endl;
	return Point(rx, ry-r);
}



void startAlgorithm() {
	while (evQueue.size()) {
		sweepLine = evQueue.top().y;
		LeavesCont::iterator currArch;
		if (evTree.size() == 1)
			currArch = evTree.begin();
		else {
			//implement arch searching function
			recalculateBreakpoints();
			printTree();
			currArch = getArchToBreak();
			if (currArch != evTree.end()) {
				cout << "Those are coordinates of arch to break: " << currArch->x << ", " << currArch->y << endl;
			}
			else
				cout << "No arch found to intersect given one\n";

			
			
		}

		//Here goes deleting circle event

		//
		
		//Inserting 5 elements - left arch, bp1, curr arch, bp2, right arch

		evTree.insert(currArch, *currArch);
		auto temp = --currArch;
		++currArch;
		Point * topArch = new Point(currArch->x, currArch->y);

		evTree.insert(currArch, Leaf(evQueue.top().x, getArchNLineIntersection((*currArch).x, (*currArch).y, evQueue.top().x), true, evCount++));
		
		//evTree.insert(currArch, Leaf(evQueue.top().x, evQueue.top().y, 0));
		
		evTree.insert(currArch, Leaf(evQueue.top().x, getArchNLineIntersection((*currArch).x, (*currArch).y, evQueue.top().x), true, evCount++));
		
		evTree.insert(--currArch, Leaf(evQueue.top().x, evQueue.top().y, topArch, evCount++));
	
		//Add tracing of intersections (refreshing breakpoints)
		//между двумя брейкпоинтами в дереве хранятся координаты арки которой они принадлежат,
		//достаточно их связать (или саму арку) с верхней аркой, 
		//с которой пересекается нижняя, и написать функцию которая будет пересчитывать координаты брейкпоинтов

		printTree();
		if (evTree.size() == 1)
			continue;

		currArch = ++(++temp);
		cout << "!!!" << currArch->n << endl;
		cout << "!!!" << (currArch == evTree.cbegin()) << endl;

		//
		/*if (evTree.size() > 1) {
			currArch = temp;
			++currArch;
			++currArch;
		}*/
		//Here goes check if this arch is left one of three to form a site

		//Here goes check if this arch is right one of three to form a site

		//Проверка на наличие 3 арок справа в списке
		int counter = 0;
		bool pass = false;
		LeavesCont::iterator Arch1, Arch2;
		for (auto i = currArch; i != evTree.end(); ++i) {
			if (i->isPoint)
				continue;
			if (counter == 1)
				Arch1 = i;
			if (counter == 2) {
				Arch2 = i;
				pass = true;
				break;
			}
			counter++;
		}
		if (pass) {
			cout << "Inside counter\n";
			Point nextCircleEv = getNextCircleEvent(*currArch, *Arch1, *Arch2);
			evQueue.push(Event(nextCircleEv));

			break;
		}
		cout << "Iteration end\n";
		//Проверка на наличие 3 арок слева в списке
		
		//Сделать проверку типа ивента в начало - чтобы прога различала где сайт ивент а где сёркл
		//Сделать проверку на дублирование ивента - найденная точка для ивента может уже существовать в очереди, либо
		//искомая вершина диграммы может уже существовать - проверять оба случая во время добавления нового сёркл ивента

		//Сделать алгоритм сёркл ивента

		//Сделать алгоритм для сайт ивентов с одинаковой у-координатой

		evQueue.pop();
	}
};
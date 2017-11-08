//#include <vector>
#include <iostream>
#include <iomanip>
#include "Voronoi.h"

#include <cmath>
#define M_PI           3.14159265358979323846

using namespace std;
double getRandNum();

EventsVect evQueue;
LeavesCont evTree;
PointsVect initPoints;

SitesVect diagramPoints;

float sweepLine;

vector<float> falseEventsVect;

int evCount = 0;
/*------------------------------------------------------------------------------------*/
bool isConvergent(float x1, float y1, float x2, float y2) {
	cout << "\nCoords to check: \n";
	cout << x1 << " " << y1 << endl;
	cout << x2 << " " << y2 << endl;
	cout << "End\n\n";
	float l1 = sqrt(x1*x1 + y1*y1);
	float l2 = sqrt(x2*x2 + y2*y2);
	float acos1 = y1 > 0 ? acos(x1 / l1) : -acos(x1 / l1);
	float acos2 = y2 > 0 ? acos(x2 / l2) : -acos(x2 / l2);
	return (acos2 < acos1 && acos2 > acos1 - M_PI) ? true : false;
};


void customSet() {
	initPoints.clear();
	
	/*initPoints.push_back(Point(0, 1));
	initPoints.push_back(Point(1, 0));
	initPoints.push_back(Point(0, -2));*/

	initPoints.push_back(Point(-23, 21));
	initPoints.push_back(Point(-41, -5));
	initPoints.push_back(Point(45, 12));
	initPoints.push_back(Point(5, -36));
	initPoints.push_back(Point(10, 32));
	initPoints.push_back(Point(2, 0));

	/*initPoints.push_back(Point(0, 2));
	initPoints.push_back(Point(1, 1));
	initPoints.push_back(Point(-3, 0));
	initPoints.push_back(Point(0, -3));
	initPoints.push_back(Point(1, -5));
	initPoints.push_back(Point(2, -6));*/

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
	cout << "(sweepline): " << sweepLine << endl;
	printf("x1: %f, y1: %f, x2: %f, y2: %f\n", x1, y1, x2, y2);
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
		/*if (i->isPoint || i->corrArch == nullptr) 
			continue;*/
		if (i->isPoint || i == evTree.begin())
			continue;

		cout << "Refreshing on: " << i->x << " " << i->y << endl;

		auto current = i;
		--(--i);
		Point l, r;
		getArchIntersection(i->x, i->y, current->x, current->y, l, r);
		++i;
		i->x = l.x;
		i->y = l.y;
		++i;
		/*float prior = i->y;
		Point l, r;
		getArchIntersection(i->corrArch->x, i->corrArch->y, i->x, i->y, l, r);
		--i;
		if (prior <= i->priority) {
			i->x = l.x;
			i->y = l.y;
		}
		++(++i);
		if (prior <= i->priority) {
			i->x = r.x;
			i->y = r.y;
		}
		--i;*/
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

Point getNextCircleEvent(Leaf a, Leaf b, Leaf c, bool& okay, float& rad) {
	cout << "Inside finding circle event\n";
	cout << "First dot: " << a.x << " " << a.y << endl;
	cout << "Second dot: " << b.x << " " << b.y << endl;
	cout << "Third dot: " <<c.x << " " << c.y << endl;

	cout << "Checking convergence: ";
	okay = isConvergent(b.x-a.x, b.y-a.y, c.x-a.x, c.y-a.y);
	cout << okay << endl;
	if (!okay) {
		rad = 0;
		return Point();
	}

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
	rad = r;
	if (ry - r > sweepLine)
		okay = false;
	else
		okay = true;
	return Point(rx, ry-r);
}

void pushCircleEvent(bool goRight, LeavesCont::iterator curr) {
	int counter = 0;
	bool pass = false;
	bool addEvent = false;
	float r;
	LeavesCont::iterator Arch1, Arch2;
	Point nextCircleEv;
	cout << "pushing entered on: " << curr->n << ", going right: " << goRight << endl; ;
	if (goRight) {
		for (auto i = curr; i != evTree.end(); ++i) {
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
		if(pass)
			nextCircleEv = getNextCircleEvent(*curr, *Arch1, *Arch2, addEvent, r);
	}
	else {
		evTree.push_front(Leaf());
		for (auto i = curr; i != evTree.begin(); --i){
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
		};
		evTree.erase(evTree.begin());
		if (pass)
			nextCircleEv = getNextCircleEvent(*Arch2, *Arch1, *curr, addEvent, r);
	}
	if (pass) {
		//Point nextCircleEv = getNextCircleEvent(*curr, *Arch1, *Arch2, addEvent, r);
		if (addEvent) {
			Arch1->toDestroy = nextCircleEv;
			Arch1->toDest = true;
			evQueue.push(Event(nextCircleEv, r));
			cout << "\n                       !!!!!! circle event inserted !!!!!!\n";
			cout << nextCircleEv.x << " - " << nextCircleEv.y << " -r- " << r << endl << endl;
		}
	}
	cout << "pushing out\n";
}

void printResults() {
	cout << "Here are the results\n";
	while (diagramPoints.size()) {
		cout << diagramPoints.top().x << ", " << diagramPoints.top().y << endl;
		diagramPoints.pop();
	}
	cout << "End of results\n";
}

bool areEqual(float a, float b, int precision) {
	int t1 = a*pow(10, precision);
	int t2 = b*pow(10, precision);
	return t1 == t2;
}

void startAlgorithm() {
	int ccc = 0;
	float prevSweep;
	float prevCircleX = evQueue.top().x, prevCircleY = evQueue.top().y;
	while (evQueue.size()) {
		sweepLine = evQueue.top().y;
		LeavesCont::iterator currArch;
		
		if (!evQueue.top().isCircleEvent) {

			cout << ">>> Site event on: " << evQueue.top().x << ",  " << evQueue.top().y << "<<<\n\n";
			if (evTree.size() == 1)
				currArch = evTree.begin();
			else {
				//implement arch searching function
				recalculateBreakpoints();
				cout << "Recalculated before site event:\n";
				currArch = getArchToBreak();
				if (currArch != evTree.end()) {
					//cout << "Those are coordinates of arch to break: " << currArch->x << ", " << currArch->y << endl;
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

			evTree.insert(currArch, Leaf(evQueue.top().x, getArchNLineIntersection((*currArch).x, (*currArch).y, evQueue.top().x), true, evQueue.top().y, evCount++));


			evTree.insert(currArch, Leaf(evQueue.top().x, getArchNLineIntersection((*currArch).x, (*currArch).y, evQueue.top().x), true, evQueue.top().y, evCount++));

			evTree.insert(--currArch, Leaf(evQueue.top().x, evQueue.top().y, topArch, evCount++));


			if (evTree.size() == 1)
				continue;

			currArch = ++(++temp);
			//cout << "!!!" << currArch->n << endl;

			//Проверка на наличие 3 арок справа в списке
			
			
			pushCircleEvent(true, currArch);

			//Проверка на наличие 3 арок слева в списке
			pushCircleEvent(false, currArch);

			
			//Сделать проверку на дублирование ивента - найденная точка для ивента может уже существовать в очереди, либо
			//искомая вершина диграммы может уже существовать - проверять оба случая во время добавления нового сёркл ивента


			//Сделать алгоритм для сайт ивентов с одинаковой у-координатой
			//Тут скорее всего просто добавить ссылку на арку слева и арку справа (по ситуации) и находить  функцией 
			//пересечение + обновлять соседние ячейки-листы-брейкпоинты

			evQueue.pop();
			cout << "After site event\n";
			printTree();
			cout << ">>> Site event end <<<\n\n";
		}
		else {
			++ccc;
			cout << "CHECKING: \n";
			cout << /*setprecision(10) << */"prev: " << prevCircleX << " " << prevCircleY << endl;
			cout << "curr: " << evQueue.top().x << " " << evQueue.top().y << endl << endl;
			if (areEqual(prevCircleX, evQueue.top().x, 4) && areEqual(prevCircleY, evQueue.top().y, 4)) {
				cout << "\nSame event found, continuing on "<<ccc<<"\n\n";
				evQueue.pop();
				continue;
			}

			if (find(falseEventsVect.begin(), falseEventsVect.end(), evQueue.top().y) != falseEventsVect.end()) {
				cout << "False event spotted, going on\n\n";
				evQueue.pop();
				continue;
			}

			cout << ">>> Circle event entered on: " << ccc << "<<<\n\n";
			for (auto i = evTree.begin(); i != evTree.end(); ++i) {
				currArch = i;
				if (i->toDest && i->toDestroy.x == evQueue.top().x && i->toDestroy.y == evQueue.top().y) {
					cout << "Matched with (this event coords): " << evQueue.top().x << " " << evQueue.top().y << endl;
					cout << "And curr coords (of connected arch) are: " << currArch->x << " " << currArch->y << endl;
					break;
				}
			}

			if (currArch != evTree.end()) {


				//Checking whether there are false events:
				cout << "Checking whether there are false events around (" << currArch->x << "; " << currArch->y << "): " << endl;
				printTree();
				cout << "\nLocal counter: " << currArch->n << endl;
				LeavesCont::iterator tt = currArch;
				if (currArch != evTree.begin()) {
					cout << "    -s-    \n\n";
					--(--currArch);
					cout << "    -e-    \n\n";

					if (currArch->toDest) {
						falseEventsVect.push_back(currArch->toDestroy.y);
						currArch->toDest = false;
					};
					currArch = tt;
				}
				if (currArch != evTree.end()) {
					cout << "    +s+    \n\n";
					++currArch;
					cout << "    ***    \n\n";
					cout << (currArch == evTree.end()) << endl;
					++currArch;
					cout << "    +e+    \n\n";

					if (currArch->toDest) {
						falseEventsVect.push_back(currArch->toDestroy.y);
						currArch->toDest = false;
					};

					currArch = tt;
				}
				Site temp;
				temp.x = currArch->toDestroy.x;
				temp.y = currArch->toDestroy.y + evQueue.top().radius;
				//cout << "y: " << currArch->y << ", r: " << evQueue.top().radius << endl;
				cout << "\n>>> New site: " << temp.x << ", " << temp.y << endl;
				diagramPoints.push(temp);
				cout << "Erasing (#): " << currArch->n << " (" << currArch->x << "; " << currArch->y << "), ";
				currArch = evTree.erase(currArch);
				--currArch;
				cout << currArch->n << " (" << currArch->x << "; " << currArch->y << ")" << endl;
				currArch = evTree.erase(currArch);

				++currArch;
				pushCircleEvent(false, currArch);
				--(--currArch);
				pushCircleEvent(true, currArch);
			}
			if (sweepLine < prevSweep)
				recalculateBreakpoints();
			cout << "\nAfter circle event:";
			printTree();
			cout << ">>> Circle event end <<<\n\n";
			prevCircleX = evQueue.top().x;
			prevCircleY = evQueue.top().y;

			cout << "\nCircle event ended, popping it out\n\n";
			evQueue.pop();
			
		}
		prevSweep = sweepLine;
	}
	printResults();
};
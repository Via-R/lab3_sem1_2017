#include <vector>
#include <queue>
#include <list>

struct Point {
	Point() {};
	Point(float a, float b) :x(a), y(b) {};
	float x, y;
};

struct Event {
	Event() {};
	Event(const Point& a): x(a.x), y(a.y), isCircleEvent(0) {};
	Event(float a, float b, bool c) :x(a), y(b), isCircleEvent(c) {};
	float x, y;
	bool isCircleEvent;
};

struct Leaf {
	Leaf(){};
	Leaf(float a, float b, bool c, int q) : x(a), y(b), isPoint(c), n(q) {};
	Leaf(float a, float b, Point* cA, int q) : x(a), y(b), isPoint(0), corrArch(cA), n(q) {};
	Leaf(const Event& a, int q): x(a.x), y(a.y), isPoint(0), n(q) {};
	float x, y; 
	bool isPoint;
	Point* corrArch = nullptr;
	int n;
};
class CompareEvents {
public:
	bool operator()(Event a, Event b) {
		return a.y < b.y;
	};
};

typedef std::priority_queue<Event, std::vector<Event>, CompareEvents> EventsVect;
typedef std::vector<Point> PointsVect;
typedef std::list<Leaf> LeavesCont;

void customSet();
void randomize(const unsigned int, const unsigned int);
void printPoints();
void initializeQueue();
void startAlgorithm();

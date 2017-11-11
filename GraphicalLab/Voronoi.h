#include <vector>
#include <queue>
#include <list>
struct Point {
	Point() {};
	Point(float a, float b) :x(a), y(b) {};
	float x, y;
};

struct Site {
	Site() {};
	Point a, b, c;
	float x, y;
};

struct Event {
	Event() {};
	Event(const Point& a) : x(a.x), y(a.y), isCircleEvent(0) {};
	Event(const Point& a, float b) : x(a.x), y(a.y), isCircleEvent(1), radius(b) {/* std::cout << "Constructed event, y= " << a.y << std::endl;*/ };
	Event(float a, float b, bool c) :x(a), y(b), isCircleEvent(c) {};
	float x, y;
	bool isCircleEvent;
	float radius = -1;
	bool remRight = false;
};

struct Edge {
	Edge() {};
	Edge(Edge* a):next(a), x(0), y(0) {};
	Edge(float a, float b):x(a), y(b) {};
	float x, y;
	Edge* next = nullptr;
};

struct Leaf {
	Leaf(){};
	Leaf(float a, float b, Edge* c, int q) : x(a), y(b), isPoint(1), n(q), corrEdge(c) {};
	Leaf(float a, float b, Point* cA, int q) : x(a), y(b), isPoint(0), corrArch(cA), n(q) {};
	Leaf(const Event& a, int q): x(a.x), y(a.y), isPoint(0), n(q) {};

	float x, y; 
	bool isPoint;
	Point* corrArch = nullptr;
	int n;
	Point toDestroy;
	bool toDest = false;
	Edge * corrEdge = nullptr;
	//float priority;
};

class CompareEvents {
public:
	bool operator()(Event a, Event b) {
		if (fabs(a.y - b.y) < 0.0000000001) return a.x > b.x;
		else return a.y < b.y;
		
	};
	bool operator()(Site a, Site b) {
		if (fabs(a.y - b.y) < 0.0000000001) return a.x > b.x;
		else return a.y < b.y;
		//return a.y < b.y;
		//return (a.y < b.y && a.x > b.x);
	};
};

typedef std::priority_queue<Event, std::vector<Event>, CompareEvents> EventsVect;
typedef std::priority_queue<Site, std::vector<Site>, CompareEvents> SitesVect;
typedef std::vector<Point> PointsVect;
typedef std::list<Leaf> LeavesCont;


void customSet(float, float);
void randomize(const unsigned int, const unsigned int);
void printPoints();
void initializeQueue();

std::vector<Site> startAlgorithm();

std::vector<Edge*> getEdges();
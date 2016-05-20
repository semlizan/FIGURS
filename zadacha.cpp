#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "gtest/gtest.h"



using namespace std;

const int TEST_C = 3;
const double EPS = 1e-10;

class Point {//класс точки 
	
public:
	double x, y;
	Point(){};
	friend Point proek();
	Point(double _x, double _y) :x(_x), y(_y) {}
	Point operator+ (Point a){
		return Point(x + a.x, y + a.y);
	}
	Point operator+ (double a){
		return Point(x + a, y + a);
	}
	Point operator- (Point a){
		return Point(x - a.x, y - a.y);
	}
	Point operator- (double a){
		return Point(x - a, y - a);
	}
	double norm() {
		return sqrt(x * x + y *y);
	}
	bool operator== (Point a){
		return operator-(a).norm() < EPS;
	}
	Point operator/ (double a){
		return Point(x / a, y / a);
	}
	Point operator* (double a){
		return Point(x*a, y*a);
	}
	Point er() {
		return *this / norm();
	}
	Point perpek(){
		return Point(-y, x);
	}

	friend double dot(Point a, Point b);
};

double dot(Point a, Point b){
	return (a.x * b.x + a.y * b.y);
}

Point proek(Point r, Point a){
	//if (r.x == 0 && r.y == 0) { return Point(0, 0); }
	double gip = a.norm();
	double cos = dot(a, r) / gip / r.norm();
	double kat = cos * gip;
	Point er = r.er();
	Point prot = er * kat;
	return prot;
}
	class Segment;
class Line {//класс линия
public:
  virtual double length() = 0;
  virtual Segment progect(Point a) = 0;
    
};
class Segment: public Line {//класс отрезок
   
public:
	Point a,b;
	Segment(Point _a, Point _b) : a(_a), b(_b){}
    Segment(double ax, double ay, double bx, double by): a(ax,ay), b(bx,by) {}
    
	double length(){
        return (a-b).norm();
    }  
	Segment progect(Point c){
		Point f = proek(c, a);
		Point d = proek(c, b);
		return Segment(f,d);
	}
	bool operator==(Segment c){
		return a == c.a && b == c.b;
	}
};
class Circle : public Line {//класс окружности 
	Point c;
	double r;
public:
	Circle(Point _c, double _r) : c(_c), r(_r) {}
	double length(){
		return r * 2 * M_PI;
	}
	Segment progect(Point m){
		Point g = proek(c, m);
		double k = dot(g,m.er());
		return Segment((m.er()*k - r), (m.er()*k + r));
	}
};

class Rectangle : public Line {//класс прямоугольник
	double l;
public:
	Point a, b, c, d;
	Segment g;
	Point v[3];
	Rectangle(Segment _g, double _l) : g(_g), l(_l)
	{
		v[0] = a = g.a;
		v[1] = b = g.b;
		v[2] = c = a + ((b - a).perpek().er())*l;
		v[3] = d = b + ((b - a).perpek().er())*l;
	};
	double length(){
		return ((b - a).norm() + l) * 2;
	}
	Segment progect(Point r){
		double min = dot(v[0], r.er());
		double max = min;
		for (int i = 1; i <= 3; ++i){
			double scalar = dot(v[i], r.er());
			if (scalar > max){
				max = scalar;
			}
			else if (scalar < min){
				min = scalar;
			}
		}
		return Segment(r.er()*min, r.er()*max);
	}
};


class Square : public Rectangle { //класс квадрат
public:
	Square(Segment _a) : Rectangle(_a, _a.length()){};
	Square(double ax, double ay, double bx, double by) :Square(Segment(ax, ay, bx, by)){};

	double length() {
		return Rectangle::length();
	}
	Segment progect(Point m){
		return Rectangle::progect(m);
	}
};
TEST(Segment, length){
	ASSERT_TRUE(Segment(Point(0, 0), Point(4,0)).length() == 4);
	ASSERT_TRUE(Segment(Point(1, 0), Point(4, 0)).length() == 3);
	ASSERT_TRUE(Segment(Point(0, 1), Point(4, 1)).length() == 4);
	ASSERT_TRUE(Segment(Point(-1,-2 ), Point(-3, 0)).length() == 2*sqrt(2));
};
TEST(Circle, length){
	ASSERT_TRUE(Circle(Point(0, 0), 1).length() == 2 * M_PI);
	ASSERT_TRUE(Circle(Point(1, 0), 2).length() == 4 * M_PI);
	ASSERT_TRUE(Circle(Point(2, 3), 3).length() == 6 * M_PI);
	ASSERT_TRUE(Circle(Point(-3, -2), 6).length() == 12 * M_PI);
};
TEST(Rectangle, length){
	ASSERT_TRUE(Rectangle(Segment(Point(1, 2),Point(5, 2)),2).length() == 12);
	ASSERT_TRUE(Rectangle(Segment(Point(0, 2), Point(3, 2)), 2).length() == 10);
	ASSERT_TRUE(Rectangle(Segment(Point(-3, -2), Point(1, -2)), 2).length() == 12);
};
TEST(Square, length){
	ASSERT_TRUE(Square(Segment(Point(1, 2), Point(5, 2))).length() == 16);
	ASSERT_TRUE(Square(Segment(Point(-1, 2), Point(5, 2))).length() == 24);
	ASSERT_TRUE(Square(Segment(Point(-1, -1), Point(1, -1))).length() == 8);
};
TEST(Point, proek){
	ASSERT_TRUE(proek(Point(1, 1), Point (2, 3)) == Point(2.5, 2.5));
	ASSERT_TRUE(proek(Point(1, 1), Point(-1, 2)) == Point(0.5, 0.5));
	ASSERT_TRUE(proek(Point(1, 2), Point(0, 4)) == Point(1.6, 3.2));
};
TEST(Segment, progect){
	ASSERT_TRUE(Segment(Point(3, 1), Point(4, 6)).progect(Point(1,1)) == Segment(Point(2, 2), Point(5, 5)));
	ASSERT_TRUE(Segment(Point(3, 1), Point(6, 4)).progect(Point(1, 1)) == Segment(Point(2, 2), Point(5, 5)));
	ASSERT_TRUE(Segment(Point(-2, -3), Point(-3, 1)).progect(Point(-1, -1)) == Segment(Point(-2.5, -2.5), Point(-1, -1)));
}
TEST(Circle, progect){
	ASSERT_TRUE(Circle(Point(4, 2), 2).progect(Point(4,4)) == Segment(Point(1.6,1.6), Point(5.6,5.6)));
	ASSERT_TRUE(Circle(Point(-2, -2), 2).progect(Point(1,1)) == Segment(Point(-1,-1), Point(3,3)));
}
TEST(Rectangle, progect){
	ASSERT_TRUE(Rectangle(Segment(Point(1, 3), Point(6, 3)), 2).progect(Point(1, 1)) == Segment(Point(2, 2), Point(5.5, 5.5)));
	ASSERT_TRUE(Rectangle(Segment(Point(-4, -3), Point(-1, -3)), 2).progect(Point(-1, -1)) == Segment(Point(-1, -1), Point(-3.5, -3.5)));
}
TEST(Square, progect){
	ASSERT_TRUE(Square(Segment(Point(1, 2), Point(4, 2))).progect(Point(1, 1)) == Segment(Point(1.5, 1.5), Point(4.5, 4.5)));
	ASSERT_TRUE(Square(Segment(Point(-3, -4), Point(0, -4))).progect(Point(-1, -1)) == Segment(Point(-0.5, -0.5), Point(-3.5, -3.5)));
}

int main(int argc, char *argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();	
}
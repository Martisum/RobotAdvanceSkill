#ifndef PLANE_GEOMETRY_H
#define PLANE_GEOMETRY_H
#include <iostream>
#include <type_traits>
#include <utility>
#include "utils/vector.h"
using namespace std;
namespace PlaneGeometry {
	template <class Num, typename = typename std::enable_if<std::is_arithmetic<Num>::value>::type>
	Num Sign(Num iNum, bool iUseZero = false) { return iNum >= 0 ? iUseZero ? iNum > 0 : 1 : -1; }
	class Point {
	private:
		double _X{}, _Y{};
	public:
		double X() const { return _X; }
		double Y() const { return _Y; }
		Point() = default;
		Point(const double iX, const double iY) : _X(iX), _Y(iY) {}
		Point(const pair<double, double> iPoint) : _X(iPoint.first), _Y(iPoint.second) { }
		Point(const Point& iPoint) : _X(iPoint.X()), _Y(iPoint.Y()) { }
		Point(const point2f& iPoint) : _X(iPoint.x), _Y(iPoint.y) { }
		~Point() = default;
		Point& operator=(const Point& iPoint) = default;
		Point& operator=(const point2f& iPoint) { return Set(iPoint); };
		Point& operator=(const pair<double, double>& iPoint) { return Set(iPoint); }
		operator pair<double, double>() const { return pair<double, double>(_X, _Y); }
		operator point2f() const { return point2f(_X, _Y); }
		Point& SetX(const double iX) {
			_X = iX;
			return *this;
		}
		Point& SetY(const double iY) {
			_Y = iY;
			return *this;
		}
		Point& Set(const double iX, const double iY) {
			_X = iX;
			_Y = iY;
			return *this;
		}
		Point& Set(const Point& iPoint) { return *this; }
		Point& Set(const pair<double, double>& iPoint) {
			_X = iPoint.first;
			_Y = iPoint.second;
			return *this;
		}
		Point& Set(const point2f& iPoint) {
			_X = iPoint.x;
			_Y = iPoint.y;
			return *this;
		}
		Point operator+(const Point& iPoint) const { return Point(_X + iPoint.X(), Y() + iPoint.Y()); }
		Point operator-(const Point& iPoint) const { return Point(_X - iPoint.X(), Y() - iPoint.Y()); }
		double Dot(const Point& iPoint) const { return _X * iPoint.X() + _Y * iPoint.Y(); }
		double Cross(const Point& iPoint) const { return _X * iPoint.Y() - _X * iPoint.X(); }
		double Length() const { return sqrt(_X * _X + _Y * _Y); }
		double LengthSqr() const { return _X * _X + _Y * _Y; }
		double Angle() const { return atan2(_Y, _X); }
		double Distance(const Point& iPoint) const { return (*this - iPoint).Length(); }
		Point Unit() const { return Point(_X / Length(), _Y / Length()); }
		Point Rotate(const double iTheta) const { return Point(_X * cos(iTheta) - _Y * sin(iTheta), _Y * cos(iTheta) + _X * sin(iTheta)); }
		friend istream& operator>>(istream& iIstream, Point& iPoint) {
			double x, y;
			iIstream >> x >> y;
			iPoint.Set(x, y);
			return iIstream;
		}
		friend ostream& operator<<(ostream& iOstream, const Point& iPoint) { return iOstream << "( " << iPoint.X() << ", " << iPoint.Y() << ")"; }
	};
	class Line {
	private:
		double _A, _B, _C;
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator +(const Num iNum) const { return Line(_A + iNum, _B + iNum, _C + iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator -(const Num iNum) const { return Line(_A - iNum, _B - iNum, _C - iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator *(const Num iNum) const { return Line(_A * iNum, _B * iNum, _C * iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator /(const Num iNum) const { return Line(_A / iNum, _B / iNum, _C / iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line& operator +=(const Num iNum) { return *this = *this + iNum; }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line& operator -=(const Num iNum) { return *this = *this - iNum; }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line& operator *=(const Num iNum) { return *this = *this * iNum; }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line& operator /=(const Num iNum) { return *this = *this / iNum; }
		Line operator +(const Line& iLine) const { return Line(_A + iLine.A(), _B + iLine.B(), C() + iLine.C()); }
		Line operator -(const Line& iLine) const { return Line(_A - iLine.A(), _B - iLine.B(), C() - iLine.C()); }
		Line operator *(const Line& iLine) const { return Line(_A * iLine.A(), _B * iLine.B(), C() * iLine.C()); }
		Line operator /(const Line& iLine) const { return Line(_A / iLine.A(), _B / iLine.B(), C() / iLine.C()); }
		Line& operator+=(const Line& iLine) { return *this = *this + iLine; }
		Line& operator-=(const Line& iLine) { return *this = *this - iLine; }
		Line& operator*=(const Line& iLine) { return *this = *this * iLine; }
		Line& operator/=(const Line& iLine) { return *this = *this / iLine; }
	public:
		Line() = default;
		Line(const double iA, const double iB, const double iC) : _A(iA), _B(iB), _C(iC) { }
		Line(const Line& iLine) : _A(iLine.A()), _B(iLine.B()), _C(iLine.C()) { }
		Line(const Point& iPoint1, const Point& iPoint2) : _A(iPoint1.Y() - iPoint2.Y()),
			_B(iPoint2.X() - iPoint1.X()),
			_C(iPoint1.X()* iPoint2.Y() - iPoint2.X() * iPoint1.Y()) {}
		~Line() = default;
		double A() const { return _A; }
		double B() const { return _B; }
		double C() const { return _C; }
		Line& Set(const double iA, const double iB, const double iC) {
			_A = iA;
			_B = iB;
			_C = iC;
			return *this;
		}
		Line& operator=(const Line& iLine) = default;
		double Distance(const Point& iPoint) const { return abs(_A * iPoint.X() + _B * iPoint.Y() + _C) / Point(_A, _B).Length(); }
		double Distance(const Line& iLine) const { return IsParallel(iLine) ? 0 : abs(_C - iLine.C() * _A / iLine.A()) / Point(_A, _B).Length(); }
		bool IsParallel(const Line& iLine) const { return _A * iLine.B() == _B * iLine.A(); }
		bool IsIntersect(const Line& iLine) const { return !IsParallel(iLine); }
		bool IsVertical(const Line& iLine) const { return _A * iLine.B() + _B * iLine.A() == 0; }
		Point CrossPoint(const Line& iLine) const {
			return IsParallel(iLine) ? Point() : Point((iLine.C() * _B - _C * iLine.B()) / (_A * iLine.B() - iLine.A() * _B),
				(_C * iLine.A() - iLine.C() * _A) / (_A * iLine.B() - iLine.A() * _B));
		}
		Point CrossPoint(const Point& iPoint1, const Point& iPoint2) const { return CrossPoint(Line(iPoint1, iPoint2)); }
		friend istream& operator>>(istream& iIstream, Line& iLine) {
			int a, b, c;
			iIstream >> a >> b >> c;
			iLine.Set(a, b, c);
			return iIstream;
		}
		friend ostream& operator<<(ostream& iOstream, const Line& iLine) {
			return iOstream << iLine.A() << "x+ " << iLine.B() << "y+ " << iLine.C() << "= 0";
		}
	};
	static Point Polar(const double iTheta, const double iLength) { return Point(iLength * cos(iTheta), iLength * sin(iTheta)); }
	static double Distance(const Point& iPoint1, const Point& iPoint2) { return iPoint1.Distance(iPoint2); }
	static double Distance(const Point& iPoint, const Line& iLine) { return iLine.Distance(iPoint); }
	static double Distance(const Line& iLine, const Point& iPoint) { return iLine.Distance(iPoint); }
	static double Distance(const Line& iLine1, const Line& iLine2) { return iLine1.Distance(iLine2); }
	static bool IsParallel(const Line& iLine1, const Line& iLine2) { return iLine1.IsParallel(iLine2); }
	static bool IsIntersect(const Line& iLine1, const Line& iLine2) { return iLine1.IsIntersect(iLine2); }
	static bool IsVertical(const Line& iLine1, const Line& iLine2) { return iLine1.IsVertical(iLine2); }
	static Point CrossPoint(const Line& iLine1, const Line& iLine2) { return iLine1.CrossPoint(iLine2); }
	static Point CrossPoint(const Point& iPoint1, const Point& iPoint2, const Point& iPoint3, const Point& iPoint4) {
		return CrossPoint(Line(iPoint1, iPoint2), Line(iPoint3, iPoint4));
	}
}
#endif

#ifndef PLANE_GEOMETRY_H
#define PLANE_GEOMETRY_H
namespace PlaneGeometry {
	class Point {
	public:
		double X{}, Y{};
		Point(const double iX, const double iY) : X(iX), Y(iY) {}
		Point(const Point& iPoint) { *this = iPoint; }
		Point(const point2f& iPoint) { *this = iPoint; }
		~Point() = default;
		Point& operator=(const Point& iPoint) = default;
		Point& operator=(const point2f& iPoint) {
			X = iPoint.x;
			Y = iPoint.y;
			return *this;
		};
		explicit operator point2f() const { return point2f(X, Y); }
	};
	class Line {
	public:
		double A{}, B{}, C{};
		Line(const double iA, const double iB, const double iC): A(iA), B(iB), C(iC) {}
		Line(const Line& iLine) { *this = iLine; }
		Line(const Point& iPoint1, const Point& iPoint2) {
			*this = Line(iPoint1.Y - iPoint2.Y, iPoint2.X - iPoint1.X, iPoint1.X * iPoint2.Y - iPoint2.X * iPoint2.Y).Sort();
		}
		~Line() = default;
		Line& operator=(const Line& iLine) = default;
		Line Sort() { return *this *= sign(A); }
		template <class Num, typename =typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator*(Num iNum) { return Line(A * iNum, B * iNum, C * iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator/(Num iNum) { return Line(A / iNum, B / iNum, C / iNum); }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator*=(Num iNum) { return *this = *this * iNum; }
		template <class Num, typename = typename enable_if<is_arithmetic<Num>::value>::type>
		Line operator/=(Num iNum) { return *this = *this / iNum; }
	};
	inline Point CrossPoint(const Line& iLine1, const Line& iLine2) {
		return Point((iLine2.C * iLine1.B - iLine1.C * iLine2.B) / (iLine1.A * iLine2.B - iLine2.A * iLine1.B),
		             (iLine2.C * iLine1.A - iLine1.C * iLine2.A) / (iLine1.B * iLine2.A - iLine2.B * iLine1.A));
	}
	inline Point CrossPoint(const Point& iPoint1, const Point& iPoint2, const Point& iPoint3, const Point& iPoint4) {
		return CrossPoint(Line(iPoint1, iPoint2), Line(iPoint3, iPoint4));
	}
}
#endif

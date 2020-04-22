#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <cmath>
#include <vector>
using namespace std;
template <typename T>
T Min(const T iLhs, const T iRhs) {
	if(iLhs < iRhs) return iLhs;
	return iRhs;
}
template <typename Num, typename = typename enable_if<is_arithmetic<Num>::value, Num>::type>
class Bound<Num> {
public:
	enum BoundTypes {
		LowerBound,
		UpperBound
	};
	Num Value;
	bool Availability;
	BoundTypes BoundType;
	Bound(const Num iValue = 0, const bool iAvailability = false, const BoundTypes iBoundType = LowerBound) : Value(iValue),
	                                                                                                          Availability(iAvailability),
	                                                                                                          BoundType(iBoundType) {}
	Bound(const Bound<Num> iBound) { this = iBound; }
	Bound(const pair<Num, bool> iBound) { this = iBound; }
	bool operator>(const Bound<Num> iBound) const { return Value > iBound.Value; }
	bool operator<(const Bound<Num> iBound) const { return Value < iBound.Value; }
	bool operator==(const Bound<Num> iBound) const { return Value == iBound.Value && Availability == iBound.Availability; }
	bool operator!=(const Bound<Num> iBound) const { return Value != iBound.Value || Availability != iBound.Availability; }
	bool operator>=(const Bound<Num> iBound) const { return this > iBound || this == iBound; }
	bool operator<=(const Bound<Num> iBound) const { return this < iBound || this == iBound; }
	Bound<Num> operator=(const Bound<Num> iBound) {
		Value = iBound.Value;
		Availability = iBound.Availability;
		return this;
	}
	Bound<Num> operator=(const pair<Num, bool> iBound) {
		Value = iBound.first;
		Availability = iBound.second;
		return this;
	}
};
template <typename Num, typename = typename enable_if<is_arithmetic<Num>::value, Num>::type>
class Range<Num> {
	pair<Bound<Num>, Bound<Num>> _Bounds;
public:
	pair<Bound<Num>, Bound<Num>> Bounds() const { return _Bounds; };
	Bound<Num> LowerBound() const { return _Bounds.first; }
	Bound<Num> UpperBound() const { return _Bounds.second; }
	Range(Bound<Num> iLowerBound, Bound<Num> iUpperBound) {
		if(iLowerBound >= iUpperBound) throw new exception("下界不能比上上界大");
		_Bounds.first = iLowerBound;
		_Bounds.second = iUpperBound;
	}
	Range(pair<Bound<Num>, Bound<Num>> iBounds) { this = iBounds; }
	Range(Range<Num> iRange) { this = iRange; }
	Range<Num> operator=(const Range<Num> iRange) {
		_Bounds = iRange.Bounds;
		return this;
	}
	Range<Num> operator=(const pair<Bound<Num>, Bound<Num>> iRange) {
		if(iRange.first >= iRange.second) throw new exception("下界不能比上上界大");
		_Bounds = iRange;
		return this;
	}
	vector<Range<Num>> operator+(const Range<Num> iRange) const {
		pair<Bound<Num>, Bound<Num>> tBounds = iRange.Bounds();
		vector<Range<Num>> tRanges;
		if(_Bounds.first < tBounds.first) { }
		return tRanges;
	}
};
template <typename Num, typename = typename enable_if<is_arithmetic<Num>::value, Num>::type>
class Aggregate<Num> {
public:
	vector<Range<Num>> Ranges;
	Aggregate(Range<Num> iRanges) : Ranges(iRanges) {}
};
#endif

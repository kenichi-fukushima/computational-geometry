#include <algorithm>
#include <cassert>
#include <cmath>
#include "base/base.h"

using std::min_element;

namespace {

const double kEpsilon = 0.0000000001;

bool InRange(double tested, double endpoint1, double endpoint2) {
  return std::min(endpoint1, endpoint2) - kEpsilon <= tested
      && tested <= std::max(endpoint1, endpoint2) + kEpsilon;
}

}  // namespace

bool Near(double lhs, double rhs) {
  return std::abs(lhs - rhs) < kEpsilon;
}

bool operator==(Point lhs, Point rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(Point lhs, Point rhs) {
  return !(lhs == rhs);
}

bool operator<(Point lhs, Point rhs) {
  return LexicographicLess(lhs, rhs);
}

std::ostream& operator<<(std::ostream& os, Point point) {
  os << point.x << ", " << point.y;
  return os;
}

bool LexicographicLess(Point lhs, Point rhs) {
  if (lhs.x == rhs.x) {
    return lhs.y < rhs.y;
  } else {
    return lhs.x < rhs.x;
  }
}

bool Near(Point lhs, Point rhs) {
  return Near(lhs.x, rhs.x) && Near(lhs.y, rhs.y);
}

bool PointsMakesLeftTurn(Point p1, Point p2, Point p3) {
  return DirectedLine(p1, p3).PointLiesToRight(p2);
}

bool PointsMakesRightTurn(Point p1, Point p2, Point p3) {
  return DirectedLine(p1, p3).PointLiesToLeft(p2);
}

DirectedLine::DirectedLine(Point p1, Point p2)
    : a_(p2.y - p1.y), b_(p1.x - p2.x), c_(p2.x * p1.y - p1.x * p2.y) {}

bool DirectedLine::is_vertical() const {
  return b_ == 0;
}

bool DirectedLine::is_horizontal() const {
  return a_ == 0;
}

double DirectedLine::y_for_x(double x) const {
  assert(!is_vertical());
  return -(a_ * x + c_) / b_;
}

double DirectedLine::x_for_y(double y) const {
  assert(!is_horizontal());
  return -(b_ * y + c_) / a_;
}

bool DirectedLine::PointLiesToLeft(Point p) const {
  return determinant(p) < -kEpsilon;
}

bool DirectedLine::PointLiesToRight(Point p) const {
  return determinant(p) > kEpsilon;
}

double DirectedLine::determinant(Point p) const {
  return a_ * p.x + b_ * p.y + c_;
}

bool DirectedLine::IntersectsDirectedLine(DirectedLine other, Point* p) const {
  double A = other.a_ * b_ - a_ * other.b_;
  if (std::fabs(A) > kEpsilon) {
    double x = (c_ * other.b_ - other.c_ * b_) / A;
    double y = (a_ * other.c_ - other.a_ * c_) / A;
    if (p) {
      *p = Point(x, y);
    }
    return true;
  } else {
    return false;
  }
}

Segment::Segment(Point p1, Point p2)
    : endpoints_{{std::min(p1, p2), std::max(p1,p2)}} {}

const std::array<Point, 2>& Segment::endpoints() const {
  return endpoints_;
}

Point Segment::endpoint(int i) const {
  return endpoints_[i];
}

bool Segment::is_vertical() const {
  return endpoint(0).x == endpoint(1).x;
}

bool Segment::is_horizontal() const {
  return endpoint(0).y == endpoint(1).y;
}

double Segment::y_for_x(double x) const {
  return as_directed_line().y_for_x(x);
}

double Segment::x_for_y(double y) const {
  return as_directed_line().x_for_y(y);
}

DirectedLine Segment::as_directed_line() const {
  return DirectedLine(endpoint(0), endpoint(1));
}

bool Segment::IntersectsSegment(Segment other, Point* p) const {
  Point p1;
  bool intersects =
      as_directed_line().IntersectsDirectedLine(other.as_directed_line(), &p1);
  if (intersects
      && InRange(p1.x, endpoint(0).x, endpoint(1).x)
      && InRange(p1.x, other.endpoint(0).x, other.endpoint(1).x)
      && InRange(p1.y, endpoint(0).y, endpoint(1).y)
      && InRange(p1.y, other.endpoint(0).y, other.endpoint(1).y)) {
    if (p) {
      *p = p1;
    }
    return true;
  } else {
    return false;
  }
}

std::ostream& operator<<(std::ostream& os, Segment segment) {
  os << "(" << segment.endpoint(0) << "), (" << segment.endpoint(1) << ")";
  return os;
}

bool operator==(const Polygon& lhs, const Polygon& rhs) {
  if (lhs.points.size() != rhs.points.size()) {
    return false;
  }
  auto lhs_min =
      min_element(lhs.points.begin(), lhs.points.end(), LexicographicLess);
  auto rhs_min =
      min_element(rhs.points.begin(), rhs.points.end(), LexicographicLess);
  for (int i = 0 ; i < lhs.points.size(); ++i) {
    if (*lhs_min != *rhs_min) {
      return false;
    }
    ++lhs_min;
    if (lhs_min == lhs.points.end()) {
      lhs_min = lhs.points.begin();
    }
    ++rhs_min;
    if (rhs_min == rhs.points.end()) {
      rhs_min = rhs.points.begin();
    }
  }
  return true;
}

bool operator!=(const Polygon& lhs, const Polygon& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon) {
  auto iter = min_element(polygon.points.begin(),
                          polygon.points.end(),
                          LexicographicLess);
  for (int i = 0; i < polygon.points.size(); ++i) {
    if (i != 0) {
      os << ", ";
    }
    os << "(" << *iter << ")";
    ++iter;
    if (iter == polygon.points.end()) {
      iter = polygon.points.begin();
    }
  }
  return os;
}

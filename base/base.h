#pragma once

#include <array>
#include <iostream>
#include <vector>

bool Near(double lhs, double rhs);

struct Point {
  double x;
  double y;

  Point() : x(0), y(0) {}
  Point(double x, double y) : x(x), y(y) {}
};

bool operator==(Point lhs, Point rhs);
bool operator!=(Point lhs, Point rhs);
bool operator<(Point lhs, Point rhs);
std::ostream& operator<<(std::ostream& os, Point point);
bool LexicographicLess(Point lhs, Point rhs);
bool Near(Point lhs, Point rhs);
bool PointsMakesLeftTurn(Point p1, Point p2, Point p3);
bool PointsMakesRightTurn(Point p1, Point p2, Point p3);

class DirectedLine {
 public:
  DirectedLine(Point p1, Point p2);

  bool is_vertical() const;
  bool is_horizontal() const;

  double y_for_x(double x) const;
  double x_for_y(double y) const;

  bool PointLiesToLeft(Point p) const;
  bool PointLiesToRight(Point p) const;

  bool IntersectsDirectedLine(DirectedLine other, Point* p) const;

 private:
  double a_, b_, c_;

  double determinant(Point p) const;
};

class Segment {
 public:
  Segment(Point p1, Point p2);

  // It is guaranteed that endpoints are sorted lexicographically.
  const std::array<Point, 2>& endpoints() const;
  Point endpoint(int i) const;

  bool is_vertical() const;
  bool is_horizontal() const;

  double y_for_x(double x) const;
  double x_for_y(double y) const;

  DirectedLine as_directed_line() const;

  bool IntersectsSegment(Segment other, Point* p) const;

private:
  std::array<Point, 2> endpoints_;
};

std::ostream& operator<<(std::ostream& os, Segment segment);

struct Polygon {
  std::vector<Point> points;

  Polygon() {}
  explicit Polygon(const std::vector<Point>& points) : points(points) {}
  Polygon(const std::initializer_list<Point>& points) : points(points) {}
};

bool operator==(const Polygon& lhs, const Polygon& rhs);
bool operator!=(const Polygon& lhs, const Polygon& rhs);
std::ostream& operator<<(std::ostream& os, const Polygon& polygon);

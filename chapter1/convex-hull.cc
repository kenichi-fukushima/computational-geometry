#include "chapter1/convex-hull.h"

#include <utility>
#include <vector>

using std::make_pair;
using std::pair;
using std::vector;

Polygon SlowConvexHull(const vector<Point>& points) {
  vector<pair<Point, Point>> e;
  for (int i = 0; i < points.size(); ++i) {
    Point p = points[i];
    for (int j = 0; j < points.size(); ++j) {
      if (i == j) {
        continue;
      }
      Point q = points[j];
      DirectedLine line(p, q);
      bool valid = true;
      for (int k = 0; k < points.size(); ++k) {
        if (k == i || k == j) {
          continue;
        }
        Point r = points[k];
        if (line.PointLiesToLeft(r)) {
          valid = false;
        }
      }
      if (valid) {
        e.push_back(make_pair(p, q));
      }
    }
  }

  vector<Point> l;
  Point start = e[0].first;
  Point current = e[0].second;
  l.push_back(current);
  // This block of code with nested loops is not the most efficient way to
  // construct the final list of points but the code above is even less
  // efficient dominating the total running time.
  while (current != start) {
    for (int i = 0; i < e.size(); ++i) {
      if (Near(e[i].first, current)) {
        l.push_back(e[i].second);
        current = e[i].second;
        break;
      }
    }
  }
  // Remove the middle of consecutive three colinear points.
  int i = 0;
  while (i < l.size()) {
    Point p0 = l[(i - 1 + l.size()) % l.size()];
    Point p1 = l[i];
    Point p2 = l[(i + 1) % l.size()];
    if (!PointsMakesRightTurn(p0, p1, p2)) {
      l.erase(l.begin() + i);
    } else {
      ++i;
    }
  }
  return Polygon(l);
}

Polygon ConvexHull(const std::vector<Point>& points) {
  vector<Point> ordered = points;
  std::sort(ordered.begin(), ordered.end(), LexicographicLess);
  vector<Point> upper{ordered[0], ordered[1]};
  for (int i = 2; i < ordered.size(); ++i) {
    upper.push_back(ordered[i]);
    while (upper.size() >= 3
           && !PointsMakesRightTurn(upper[upper.size() - 3],
                                    upper[upper.size() - 2],
                                    upper[upper.size() - 1])) {
      upper.erase(upper.end() - 2);
    }
  }
  vector<Point> lower{ordered[ordered.size() - 1],
                      ordered[ordered.size() - 2]};
  for (int i = ordered.size() - 3; i >= 0; --i) {
    lower.push_back(ordered[i]);
    while (lower.size() >= 3
           && !PointsMakesRightTurn(lower[lower.size() - 3],
                                    lower[lower.size() - 2],
                                    lower[lower.size() - 1])) {
      lower.erase(lower.end() - 2);
    }
  }
  std::copy(lower.begin() + 1,
            lower.end() - 1,
            std::back_inserter(upper));
  return Polygon(upper);
}

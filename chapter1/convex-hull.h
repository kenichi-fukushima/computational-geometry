#include <vector>
#include "base/base.h"

// The algorithm on page 3. Its time complexity is O(n^3).
Polygon SlowConvexHull(const std::vector<Point>& points);

// Graham's scan; the algorithm on page 6. Its time complexity is O(n log n).
Polygon ConvexHull(const std::vector<Point>& points);

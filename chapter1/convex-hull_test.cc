#include "chapter1/convex-hull.h"
#include "gtest/gtest.h"

using std::vector;
using testing::TestWithParam;

using ConvexHullFunc = Polygon(*)(const vector<Point>&);

class ConvexHullTest : public TestWithParam<ConvexHullFunc> {
 protected:
  ConvexHullFunc func() {
    return GetParam();
  }
};

TEST_P(ConvexHullTest, Trivial) {
  Polygon expected {{0, 0}, {0, 1}, {1, 0}};
  vector<Point> cw_input{{0, 0}, {0, 1}, {1, 0}};
  EXPECT_EQ(expected, func()(cw_input));
  vector<Point> ccw_input{{0, 0}, {1, 0}, {0, 1}};
  EXPECT_EQ(expected, func()(ccw_input));
}

TEST_P(ConvexHullTest, MoreComplex) {
  Polygon expected {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
  vector<Point> input{{1, 1}, {0.2, 0.2}, {0, 0}, {0.6, 0.1}, {0, 1}, {1, 0}};
  EXPECT_EQ(expected, func()(input));
}

TEST_P(ConvexHullTest, Degeneracy) {
  Polygon expected {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  vector<Point> input{{1, 0}, {0.3, -0.7}, {0, -1}, {-0.2, -0.8},
                      {-1, 0}, {-0.29, 0.71}, {0, 1}, {0.59, 0.41}};
  EXPECT_EQ(expected, func()(input));
}

INSTANTIATE_TEST_CASE_P(ConvexHullImpls,
                        ConvexHullTest,
                        ::testing::Values(SlowConvexHull, ConvexHull));

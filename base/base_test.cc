#include "base/base.h"
#include "gtest/gtest.h"

TEST(DirectedLineTest, Vertical) {
  DirectedLine line0(Point(0, 0), Point(1, 0));
  EXPECT_FALSE(line0.is_vertical());

  DirectedLine line1(Point(0, 0), Point(0, 1));
  EXPECT_TRUE(line1.is_vertical());

  DirectedLine line2(Point(0, 0), Point(1, 1));
  EXPECT_FALSE(line2.is_vertical());
}

TEST(DirectedLineTest, Horizontal) {
  DirectedLine line0(Point(0, 0), Point(1, 0));
  EXPECT_TRUE(line0.is_horizontal());

  DirectedLine line1(Point(0, 0), Point(0, 1));
  EXPECT_FALSE(line1.is_horizontal());

  DirectedLine line2(Point(0, 0), Point(1, 1));
  EXPECT_FALSE(line2.is_horizontal());
}

TEST(DirectedLineTest, YForX) {
  DirectedLine line(Point(0, 0), Point(1, 2));
  EXPECT_DOUBLE_EQ(-0.2, line.y_for_x(-0.1));
  EXPECT_DOUBLE_EQ(0, line.y_for_x(0));
  EXPECT_DOUBLE_EQ(0.6, line.y_for_x(0.3));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(1));
  EXPECT_DOUBLE_EQ(2.2, line.y_for_x(1.1));
}

TEST(DirectedLineTest, YForX_Horizontal) {
  DirectedLine line(Point(0, 2), Point(1, 2));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(-0.1));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(0));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(0.3));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(1));
  EXPECT_DOUBLE_EQ(2, line.y_for_x(1.1));
}

TEST(DirectedLineTest, XForY) {
  DirectedLine line(Point(0, 0), Point(1, 2));
  EXPECT_DOUBLE_EQ(-0.05, line.x_for_y(-0.1));
  EXPECT_DOUBLE_EQ(0, line.x_for_y(0));
  EXPECT_DOUBLE_EQ(0.15, line.x_for_y(0.3));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(2));
  EXPECT_DOUBLE_EQ(1.25, line.x_for_y(2.5));
}

TEST(DirectedLineTest, XForY_Vertical) {
  DirectedLine line(Point(1, 0), Point(1, 2));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(-0.1));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(0));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(0.3));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(2));
  EXPECT_DOUBLE_EQ(1, line.x_for_y(2.5));
}

TEST(DirectedLineTest, PointLiesToLeftRight_PositiveGradient) {
  DirectedLine line(Point(3.8, -5.2), Point(13.2, 4.8));
  EXPECT_FALSE(line.PointLiesToLeft(Point(8.5, -0.3)));
  EXPECT_TRUE(line.PointLiesToRight(Point(8.5, -0.3)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(8.5, -0.2)));
  EXPECT_FALSE(line.PointLiesToRight(Point(8.5, -0.2)));
  EXPECT_TRUE(line.PointLiesToLeft(Point(8.5, -0.1)));
  EXPECT_FALSE(line.PointLiesToRight(Point(8.5, -0.1)));
}

TEST(DirectedLineTest, PointLiesToLeftRight_NegativeGradient) {
  DirectedLine line(Point(-3.8, -5.2), Point(-13.2, 4.8));
  EXPECT_TRUE(line.PointLiesToLeft(Point(-8.5, -0.3)));
  EXPECT_FALSE(line.PointLiesToRight(Point(-8.5, -0.3)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(-8.5, -0.2)));
  EXPECT_FALSE(line.PointLiesToRight(Point(-8.5, -0.2)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(-8.5, -0.1)));
  EXPECT_TRUE(line.PointLiesToRight(Point(-8.5, -0.1)));
}

TEST(DirectedLineTest, PointLiesToLeftRight_Vertical) {
  DirectedLine line(Point(6.7, -0.1), Point(6.7, 5.1));
  EXPECT_TRUE(line.PointLiesToLeft(Point(6.6, 3.2)));
  EXPECT_FALSE(line.PointLiesToRight(Point(6.6, 3.2)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(6.7, 3.2)));
  EXPECT_FALSE(line.PointLiesToRight(Point(6.7, 3.2)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(6.8, 3.2)));
  EXPECT_TRUE(line.PointLiesToRight(Point(6.8, 3.2)));
}

TEST(DirectedLineTest, PointLiesToLeftRight_Horizontal) {
  DirectedLine line(Point(-9.3, -8.2), Point(1.2, -8.2));
  EXPECT_TRUE(line.PointLiesToLeft(Point(6.4, -8.1)));
  EXPECT_FALSE(line.PointLiesToRight(Point(6.4, -8.1)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(6.4, -8.2)));
  EXPECT_FALSE(line.PointLiesToRight(Point(6.4, -8.2)));
  EXPECT_FALSE(line.PointLiesToLeft(Point(6.4, -8.3)));
  EXPECT_TRUE(line.PointLiesToRight(Point(6.4, -8.3)));
}

TEST(DirectedLineTest, IntersectsDirectedLine_Positive) {
  DirectedLine line1(Point(1, 3), Point(3, 5));
  DirectedLine line2(Point(6, 6), Point(8, 4));
  DirectedLine line3(Point(5, 6), Point(5, 4));
  DirectedLine line4(Point(6, 7), Point(8, 7));

  Point p12;
  ASSERT_TRUE(line1.IntersectsDirectedLine(line2, &p12));
  EXPECT_EQ(Point(5, 7), p12);

  Point p13;
  ASSERT_TRUE(line1.IntersectsDirectedLine(line3, &p13));
  EXPECT_EQ(Point(5, 7), p13);

  Point p14;
  ASSERT_TRUE(line1.IntersectsDirectedLine(line4, &p14));
  EXPECT_EQ(Point(5, 7), p14);

  Point p21;
  ASSERT_TRUE(line2.IntersectsDirectedLine(line1, &p21));
  EXPECT_EQ(Point(5, 7), p21);

  Point p23;
  ASSERT_TRUE(line2.IntersectsDirectedLine(line3, &p23));
  EXPECT_EQ(Point(5, 7), p23);

  Point p24;
  ASSERT_TRUE(line2.IntersectsDirectedLine(line4, &p24));
  EXPECT_EQ(Point(5, 7), p24);

  Point p31;
  ASSERT_TRUE(line3.IntersectsDirectedLine(line1, &p31));
  EXPECT_EQ(Point(5, 7), p31);

  Point p32;
  ASSERT_TRUE(line3.IntersectsDirectedLine(line2, &p32));
  EXPECT_EQ(Point(5, 7), p32);

  Point p34;
  ASSERT_TRUE(line3.IntersectsDirectedLine(line4, &p34));
  EXPECT_EQ(Point(5, 7), p34);
}

TEST(DirectedLineTest, IntersectsDirectedLine_Negative) {
  Point out;
  Point p13(1, 3);
  Point p24(2, 4);
  Point p68(6, 8);
  Point p79(7, 9);
  EXPECT_FALSE(
      DirectedLine(p13, p24).IntersectsDirectedLine(DirectedLine(p68, p79), &out));
  EXPECT_FALSE(
      DirectedLine(p13, p68).IntersectsDirectedLine(DirectedLine(p24, p79), &out));
  Point p11(1, 1);
  Point p14(1, 4);
  Point p41(4, 1);
  Point p44(4, 4);
  EXPECT_FALSE(
      DirectedLine(p11, p14).IntersectsDirectedLine(DirectedLine(p41, p44), &out));
  EXPECT_FALSE(
      DirectedLine(p11, p41).IntersectsDirectedLine(DirectedLine(p14, p44), &out));
}

TEST(SegmentTest, Constructor) {
  Segment s1(Point(0, 0), Point(1, 1));
  EXPECT_EQ(Point(0, 0), s1.endpoint(0));
  EXPECT_EQ(Point(1, 1), s1.endpoint(1));
  Segment s2(Point(1, 1), Point(0, 0));
  EXPECT_EQ(Point(0, 0), s2.endpoint(0));
  EXPECT_EQ(Point(1, 1), s2.endpoint(1));
}

TEST(SegmentTest, IntersectsSegment) {
  Segment s(Point(0.3, 0.5), Point(0.5, 0.7));
  Point out;
  EXPECT_FALSE(s.IntersectsSegment(Segment(Point(0.3, 0.3), Point(0.1, 0.5)), &out));
  ASSERT_TRUE(s.IntersectsSegment(Segment(Point(0.4, 0.4), Point(0.2, 0.6)), &out));
  EXPECT_TRUE(Near(Point(0.3, 0.5), out));
  ASSERT_TRUE(s.IntersectsSegment(Segment(Point(0.5, 0.5), Point(0.3, 0.7)), &out));
  EXPECT_TRUE(Near(Point(0.4, 0.6), out));
  ASSERT_TRUE(s.IntersectsSegment(Segment(Point(0.6, 0.6), Point(0.4, 0.8)), &out));
  EXPECT_TRUE(Near(Point(0.5, 0.7), out));
  EXPECT_FALSE(s.IntersectsSegment(Segment(Point(0.7, 0.7), Point(0.5, 0.9)), &out));
}

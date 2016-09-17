#include "chapter2/segment-intersection.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using std::vector;
using testing::Matches;
using testing::UnorderedElementsAre;

namespace segment_intersection_internal {

MATCHER_P(SegmentNear, s, "") {
  return Near(s.endpoint(0), arg.endpoint(0))
      && Near(s.endpoint(1), arg.endpoint(1));
}

TEST(SweepLineStatus, Comprehensive) {
  Segment segments[] = {
    Segment(Point(1.1, 3.3), Point(3.3, 5.5)),
    Segment(Point(1.1, 5.5), Point(3.3, 3.3)),
    Segment(Point(2.2, 2.2), Point(5.5, 5.5)),
    Segment(Point(2.2, 6.6), Point(5.5, 3.3)),
    Segment(Point(3.3, 1.1), Point(3.3, 7.7)),
  };

  SweepLineStatus status;

  status.set_event_point(Point(1.1, 3.3));
  status.InsertSegment(&segments[0]);

  status.set_event_point(Point(1.1, 5.5));
  status.InsertSegment(&segments[1]);

  status.set_event_point(Point(2.2, 2.2));
  status.InsertSegment(&segments[2]);

  status.set_event_point(Point(2.2, 4.4));
  ASSERT_THAT(status.SegmentsPassingCurrentEventPoint(),
              UnorderedElementsAre(&segments[0], &segments[1]));
  status.DeleteSegment(&segments[0]);
  status.DeleteSegment(&segments[1]);
  status.InsertSegment(&segments[0]);
  status.InsertSegment(&segments[1]);

  status.set_event_point(Point(2.2, 6.6));
  status.InsertSegment(&segments[3]);

  status.set_event_point(Point(3.3, 1.1));
  status.InsertSegment(&segments[4]);

  status.set_event_point(Point(3.3, 3.3));
  ASSERT_THAT(status.SegmentsPassingCurrentEventPoint(),
              UnorderedElementsAre(&segments[1], &segments[2], &segments[4]));
  status.DeleteSegment(&segments[1]);
  status.DeleteSegment(&segments[2]);
  status.DeleteSegment(&segments[4]);
  status.InsertSegment(&segments[2]);
  status.InsertSegment(&segments[4]);

  status.set_event_point(Point(3.3, 5.5));
  ASSERT_THAT(status.SegmentsPassingCurrentEventPoint(),
              UnorderedElementsAre(&segments[0], &segments[3], &segments[4]));
  status.DeleteSegment(&segments[0]);
  status.DeleteSegment(&segments[3]);
  status.DeleteSegment(&segments[4]);
  status.InsertSegment(&segments[3]);
  status.InsertSegment(&segments[4]);

  status.set_event_point(Point(3.3, 7.7));
  ASSERT_THAT(status.SegmentsPassingCurrentEventPoint(),
              UnorderedElementsAre(&segments[4]));
  status.DeleteSegment(&segments[4]);

  status.set_event_point(Point(4.4, 4.4));
  ASSERT_THAT(status.SegmentsPassingCurrentEventPoint(),
              UnorderedElementsAre(&segments[2], &segments[3]));
}

TEST(FindIntersectionsTest, Empty) {
  vector<Segment> segments;
  auto intersections = FindIntersections(segments);
  EXPECT_EQ(0, intersections.size());
}

TEST(FindIntersectionsTest, OneSegment) {
  vector<Segment> segments {{{0.2, 0.8}, {1.9, -3.9}}};
  auto intersections = FindIntersections(segments);
  EXPECT_EQ(0, intersections.size());
}

TEST(FindIntersectionsTest, OneSegment_Horizontal) {
  vector<Segment> segments {{{-0.3, 0.3}, {1.8, 0.3}}};
  auto intersections = FindIntersections(segments);
  EXPECT_EQ(0, intersections.size());
}

TEST(FindIntersectionsTest, OneSegment_Vertical) {
  vector<Segment> segments {{{-0.3, -9.0}, {-0.3, 1.8}}};
  auto intersections = FindIntersections(segments);
  EXPECT_EQ(0, intersections.size());
}

TEST(FindIntersectionsTest, TwoSegments) {
  vector<Segment> segments {
    {{3, 3}, {8, 3}},
    {{2, 8}, {8, 2}},
  };
  auto intersections = FindIntersections(segments);
  ASSERT_EQ(1, intersections.size());
  EXPECT_THAT(intersections[Point(7, 3)],
              UnorderedElementsAre(SegmentNear(segments[0]),
                                   SegmentNear(segments[1])));
}

}  // namespace segment_intersection_internal

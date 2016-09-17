#pragma once

#include <map>
#include <vector>
#include "base/base.h"

// A place sweep algorithm to find line segment intersections on page 25.
std::map<Point, std::vector<Segment>> FindIntersections(
    const std::vector<Segment>& segments);

namespace segment_intersection_internal {

using SegmentRef = const Segment*;

class SweepLineStatus {
 public:
  void set_event_point(Point p);

  SegmentRef SegmentAboveCurrentEventPoint();
  SegmentRef SegmentBelowCurrentEventPoint();
  std::vector<SegmentRef> SegmentsPassingCurrentEventPoint();
  SegmentRef SegmentBelowSegment(SegmentRef s);
  SegmentRef SegmentAboveSegment(SegmentRef s);
  void DeleteSegment(SegmentRef s);
  void InsertSegment(SegmentRef s);

private:
  Point event_point_;
  std::vector<SegmentRef> segments_;
};

}  // internal

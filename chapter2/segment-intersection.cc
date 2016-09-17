#include "chapter2/segment-intersection.h"

#include <cassert>
#include <cmath>
#include <limits>
#include "base/stl-utils.h"

using std::make_pair;
using std::map;
using std::numeric_limits;
using std::vector;

namespace segment_intersection_internal {

const double kEpsilon = 0.00000001;

struct Event {
  Point point;
  vector<SegmentRef> starting_segments;
};

class EventQueue {
 public:
  Event* EventAtPoint(Point p) {
    if (!ContainsKey(queue_impl_, p)) {
      auto e = new Event;
      e->point = p;
      queue_impl_[p] = e;
    }
    return FindOrDie(queue_impl_, p);
  };

  Event* NextEvent() const {
    return queue_impl_.begin()->second;
  }

  void RemoveEvent(const Event* e) {
    RemoveKey(&queue_impl_, e->point);
  }

  bool empty() const {
    return queue_impl_.empty();
  }

 private:
  map<Point, Event*> queue_impl_;
};

class SegmentComparator {
 public:
  enum SweepLineShift {
    NONE,
    BACKWARD,
    FORWARD,
  };

  SegmentComparator(Point event_point, SweepLineShift shift)
      : event_point_(event_point), shift_(shift) {}

  bool operator()(SegmentRef lhs, SegmentRef rhs) {
    if (lhs->is_vertical() && rhs->is_vertical()) {
      // Both vertical.
      return false;

    } else if (lhs->is_vertical()) {
      double diff = event_point_.y - rhs->y_for_x(event_point_.x);
      return diff < (shift_ == BACKWARD ? kEpsilon : -kEpsilon);

    } else if (rhs->is_vertical()) {
      double diff = lhs->y_for_x(event_point_.x) - event_point_.y;
      return diff < (shift_ == FORWARD ? kEpsilon : -kEpsilon);

    } else {
      // Neither vertical.
      double diff = lhs->y_for_x(event_point_.x) - rhs->y_for_x(event_point_.x);
      if (std::fabs(diff) > kEpsilon) {
        return diff < 0;
      } else if (shift_ != NONE) {
        double shifted = event_point_.x + ((shift_ == FORWARD) ? 1 : -1);
        diff = lhs->y_for_x(shifted) - rhs->y_for_x(shifted);
        if (std::fabs(diff) > kEpsilon) {
          return diff < 0;
        }
      }
      return false;
    }
  }

 private:
  Point event_point_;
  SweepLineShift shift_;
};

void SweepLineStatus::set_event_point(Point p) {
  event_point_ = p;
}

SegmentRef SweepLineStatus::SegmentAboveCurrentEventPoint() {
  Segment horizontal(Point(event_point_.x - 1, event_point_.y),
                     Point(event_point_.x + 1, event_point_.y));
  return SegmentAboveSegment(&horizontal);
}

SegmentRef SweepLineStatus::SegmentBelowCurrentEventPoint() {
  Segment horizontal(Point(event_point_.x - 1, event_point_.y),
                     Point(event_point_.x + 1, event_point_.y));
  return SegmentBelowSegment(&horizontal);
}

vector<SegmentRef> SweepLineStatus::SegmentsPassingCurrentEventPoint() {
  Segment horizontal(Point(event_point_.x - 1, event_point_.y),
                     Point(event_point_.x + 1, event_point_.y));
  auto range = std::equal_range(segments_.begin(),
                                segments_.end(),
                                &horizontal,
                                SegmentComparator(event_point_,
                                                  SegmentComparator::NONE));
  vector<SegmentRef> matched_segments;
  copy(range.first, range.second, std::back_inserter(matched_segments));
  return matched_segments;
}

SegmentRef SweepLineStatus::SegmentAboveSegment(SegmentRef s) {
  auto iter = std::upper_bound(segments_.begin(),
                               segments_.end(),
                               s,
                               SegmentComparator(event_point_,
                                                 SegmentComparator::FORWARD));
  if (iter != segments_.end()) {
    return *iter;
  } else {
    return nullptr;
  }
}

SegmentRef SweepLineStatus::SegmentBelowSegment(SegmentRef s) {
  auto iter = std::lower_bound(segments_.begin(),
                               segments_.end(),
                               s,
                               SegmentComparator(event_point_,
                                                 SegmentComparator::FORWARD));
  if (iter != segments_.begin()) {
    return *(iter - 1);
  } else {
    return nullptr;
  }
}

void SweepLineStatus::DeleteSegment(SegmentRef s) {
  auto iter = std::lower_bound(segments_.begin(),
                               segments_.end(),
                               s,
                               SegmentComparator(event_point_,
                                                 SegmentComparator::BACKWARD));
  assert(iter != segments_.end());
  segments_.erase(iter);
}

void SweepLineStatus::InsertSegment(SegmentRef s) {
  auto iter = std::upper_bound(segments_.begin(),
                               segments_.end(),
                               s,
                               SegmentComparator(event_point_,
                                                 SegmentComparator::FORWARD));
  segments_.insert(iter, s);
}

class IntersectionFinder {
 public:
  IntersectionFinder(const vector<Segment>& segments)
      : segments_(segments) {}

  map<Point, vector<Segment>> Find() {
    for (const Segment& s : segments_) {
      Point lower = s.endpoint(0);
      Event* event = queue_.EventAtPoint(lower);
      event->starting_segments.push_back(&s);
      Point upper = s.endpoint(1);
      queue_.EventAtPoint(upper);
    }

    while (!queue_.empty()) {
      Event* event = queue_.NextEvent();
      queue_.RemoveEvent(event);
      HandleEvent(event);
    }
    return intersections_;
  }

 private:
  const vector<Segment>& segments_;
  map<Point, vector<Segment>> intersections_;
  EventQueue queue_;
  SweepLineStatus status_;

  void HandleEvent(Event* e) {
    Point p = e->point;
    status_.set_event_point(p);
    const vector<SegmentRef>& L = e->starting_segments;
    vector<SegmentRef> R;
    vector<SegmentRef> C;
    for (auto s : status_.SegmentsPassingCurrentEventPoint()) {
      if (s->endpoint(1) == p) {
        R.push_back(s);
      } else {
        C.push_back(s);
      }
    }
    if (L.size() + R.size() + C.size() > 1) {
      vector<Segment> all_segments;
      for (auto s : L) { all_segments.push_back(*s); }
      for (auto s : R) { all_segments.push_back(*s); }
      for (auto s : C) { all_segments.push_back(*s); }
      intersections_.insert(make_pair(p, all_segments));
    }
    for (auto s : R) { status_.DeleteSegment(s); }
    for (auto s : C) { status_.DeleteSegment(s); }
    vector<SegmentRef> LC;
    std::copy(L.begin(), L.end(), std::back_inserter(LC));
    std::copy(C.begin(), C.end(), std::back_inserter(LC));
    for (auto s : LC) { status_.InsertSegment(s); }
    if (LC.empty()) {
      SegmentRef sb = status_.SegmentBelowCurrentEventPoint();
      SegmentRef sa = status_.SegmentAboveCurrentEventPoint();
      if (sa && sb) {
        FindNewEvent(sb, sa, p);
      }
    } else {
      std::sort(LC.begin(),
                LC.end(),
                SegmentComparator(p, SegmentComparator::FORWARD));
      SegmentRef s1 = LC.front();
      SegmentRef sl = status_.SegmentBelowSegment(s1);
      if (sl) {
        FindNewEvent(sl, s1, p);
      }
      SegmentRef s2 = LC.back();
      SegmentRef sr = status_.SegmentAboveSegment(s2);
      if (sr) {
        FindNewEvent(s2, sr, p);
      }
    }
  }

  void FindNewEvent(SegmentRef s1, SegmentRef s2, Point p) {
    Point intersection;
    if (s1->IntersectsSegment(*s2, &intersection)
        && p < intersection) {
      queue_.EventAtPoint(intersection);
    }
  }
};

}  // namespace segment_intersection_internal

map<Point, vector<Segment>> FindIntersections(const vector<Segment>& segments) {
  segment_intersection_internal::IntersectionFinder finder(segments);
  return finder.Find();
}

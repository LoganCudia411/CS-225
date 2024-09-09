#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"
using namespace std; 

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point); 
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
     work_list.push_front(point); 
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    
    work_list.pop_back(); 
   
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
   
    work_list.pop_front(); 
   
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.back(); //queue
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.front(); //stack
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    curr_png = png;
    start_ = start;
    t = tolerance;
    fns_.add = fns.add;
    fns_.pop = fns.pop;
    fns_.peek = fns.peek;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(curr_png, start_, t, fns_);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    //default, do nothing
  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
    //list of current neighbors to each immediate direction 
     Point down(curr.x, curr.y+1);
    Point left(curr.x-1, curr.y);
    Point up(curr.x, curr.y-1);
    Point right(curr.x+1, curr.y);
   
    

    //if right neighbor is valid 
    if(right.x < curr_png.width() && !(visited_[right.y][right.x]) && calculateDelta(curr_png.getPixel(start_.x,start_.y), curr_png.getPixel(right.x, right.y)) <= t) {
        fns_.add(work_list_, right);
    }
    //if down neightbor is valid
    if(down.y < curr_png.height() && !(visited_[down.y][down.x]) && calculateDelta(curr_png.getPixel(start_.x,start_.y), curr_png.getPixel(down.x, down.y)) <= t) {
        fns_.add(work_list_, down);
    }
    //if left neighbor is valid 
    if(curr.x != 0) {
      if(left.x >= 0 && !(visited_[left.y][left.x]) && calculateDelta(curr_png.getPixel(start_.x,start_.y), curr_png.getPixel(left.x, left.y)) <= t) {
          fns_.add(work_list_, left);
        }
      }
    //if up neighbor is valid 
    if(curr.y != 0) {
      if(up.y >= 0 && !(visited_[up.y][up.x]) && calculateDelta(curr_png.getPixel(start_.x,start_.y), curr_png.getPixel(up.x, up.y)) <= t) {
          fns_.add(work_list_, up);
      }
    }
    
    while(!(work_list_.empty())) {
      curr = (fns_.peek(work_list_));
      fns_.pop(work_list_);
      if(!(visited_[curr.y][curr.x])){
        visited_[curr.y][curr.x] = true;
        return *this;
      }
    }

    return *this;
  }

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return curr;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if(work_list_.empty() && other.work_list_.empty())
      return false;
    else 
      return true;
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }


  ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {
   curr_png = png;
    start_ = start;
    curr = start; 
    t = tolerance;
    fns_.add = fns.add;
    fns_.pop = fns.pop;
    fns_.peek = fns.peek;

    std:: vector<bool> thickness(png.width(), false);
    visited_.resize(png.height(), thickness);

    visited_[start.y][start.x] = true;

    fns.add(work_list_, start);
  } 
}



  
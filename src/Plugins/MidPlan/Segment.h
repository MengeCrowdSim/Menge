/*

Sergio Lozano Mateos
UNED, Spain

*/

#ifndef SEGMENT_H
#define SEGMENT_H

#include "MengeCore/Math/Vector2.h"
#include <iostream>
#include <iomanip>
using namespace std;

using Menge::Math::Vector2;

// Segment struct used in calculus
struct Segment {
  Vector2 start;
  Vector2 end;
  float length;
  double time;
  double eTime;
  double cost;
  double eCost;
  int collision;
  int expansions;

  /*!
 @brief		Segment constructor.
 */
  Segment();
  /*!
 @brief		Segment constructor.

 @param		instart         The start point of the vector.
 @param		inend           The end point of the vector.
 @param     intime          The time to travel the segment.
 @param     incost          The cost of the segment.
 */
  Segment(Vector2 instart, Vector2 inend, double intime, double incost) {
    start = instart;
    end = inend;
    length = (end - start).Length();
    eTime = intime;
    eCost = incost;
  }

  /*!
  @brief		Get orientation.
  */
  Vector2 getOrient() {
    Vector2 seg = end - start;
    seg.normalize();
    return seg;
  }

  /*!
  @brief		Get vector.
  */
  Vector2 getVector() {
    Vector2 vec = end - start;
    return vec;
  }

  /*!
  @brief		Get pair of points
  */
  pair<Vector2,Vector2> getVectors() {
    Vector2 vec = end - start;
    return make_pair(start, end);
  }

  /*!
  @brief		Check if the segment has the same points than other
  @param Segment
  */
  bool equal(Segment ab) {
    //cout << "Comparacion " << start << end << "  " << ab.start << ab.end;
    if ((abs(start._x - ab.start._x) < 0.001f) && (abs(start._y - ab.start._y) < 0.001f) &&
        (abs(end._x - ab.end._x) < 0.001f) && (abs(end._y - ab.end._y) < 0.001f)) {
      //cout << " Iguales " << endl;
      return true;
    }
    //cout << " Diferentes " << endl;
    return false;
    
  }
};
#endif

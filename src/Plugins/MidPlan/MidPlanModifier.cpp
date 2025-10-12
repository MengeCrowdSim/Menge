/*

Sergio Lozano Mateos
UNED, Spain

*/

#include "MidPlanModifier.h"
#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryStructs.h"
#include "MengeCore/Math/Line.h"
#include "Segment.h"
#include <list>
#include <map>
#include <unordered_set>
#include <iterator>
#include <random>
#include <mutex>
#include <condition_variable>

namespace MPModifier {

using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::VelModFactory;
using Menge::BFSM::VelModifier;
using Menge::Math::FloatGenerator;
using Menge::Math::Line;
using Menge::Math::Vector2;

// Params
float mass = 1;
float detourDistance = 10;
bool trace;
int maxPeriod = 10;
int minPeriod = 0;

// Shared variables
unordered_map<int, pair<int, Vector2>> freqTab;
int stepCount = 1;

mutex m;

void incrementCounter() {
  m.lock(); 
  stepCount++;
  m.unlock();
}

int getCounter() {
  int readCount;
  m.lock();
  readCount=stepCount;
  m.unlock();
  return readCount;
}

void writeOnTab(size_t id, int per, Vector2 vel) {
  m.lock();
  freqTab[id] = make_pair(per, vel);
  m.unlock();
}

Vector2 decrementOnTab(size_t id) {
  m.lock();
  Vector2 sameVel = freqTab[id].second;
  freqTab[id].first = freqTab[id].first - 1;
  m.unlock();
  return sameVel;
}

bool isInTab(size_t id) {
  m.lock();
  auto tabNumber = freqTab.find(id);
  if (tabNumber == freqTab.end()) {
    m.unlock();
    return false;
  }
  m.unlock();
  return true;
}

bool zeroOnTab(size_t id) {
  m.lock();
  if (freqTab[id].first != 0) {
    m.unlock();
    return false;
  }
  m.unlock();
  return true;
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of MPModifier
/////////////////////////////////////////////////////////////////////

MPModifier::MPModifier()
    : VelModifier(), _followDist(0.5f), _maxExpansions(5), _maxMITneigh(5), _trace(false) {}

/////////////////////////////////////////////////////////////////////

MPModifier::MPModifier(float followDist, int maxExpansions, int _maxMITneigh, bool trace)
    : VelModifier(),
      _followDist(followDist),
      _maxExpansions(maxExpansions),
      _maxMITneigh(5), _trace(trace) {}

/////////////////////////////////////////////////////////////////////

VelModifier* MPModifier::copy() const {
  return new MPModifier(_followDist, _maxExpansions, _maxMITneigh, _trace);
};

float dotProduct(Vector2 u, Vector2 v) { return (u._x * v._x) + (u._y * v._y); }

float vecProduct(Vector2 u, Vector2 v) { return (u._x * v._y) - (u._y * v._x); }

/////////////////////////////////////////////////////////////////////


// Function to obtain the intersection points between two lines
list<Vector2> interLines(Line l1, Line l2) {
  list<Vector2> foundInt;
  float a1 = -l1._direction._y;
  float b1 = l1._direction._x;
  float c1 = l1._direction._y * l1._point._x - l1._direction._x * l1._point._y;
  float a2 = -l2._direction._y;
  float b2 = l2._direction._x;
  float c2 = l2._direction._y * l2._point._x - l2._direction._x * l2._point._y;
  float x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
  float y = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1);
  if (x == x && y == y) {
    foundInt.push_back(Vector2(x, y));
  }
  return foundInt;
}

// Function to obtain the intersection points between circumference and line
list<Vector2> interCircLine(Vector2 pos, Vector2 rad, Line line) {
  list<Vector2> foundInt;
  float radius = rad.Length();
  // Intersection between line and circumference
  Vector2 nearestPt = line.nearestPt(pos);
  Vector2 d1 = nearestPt - pos;
  float minDist = d1.Length();
  // Check if there is intersection
  // if no intersection
  if (minDist > radius) return foundInt;
  // if is tangent
  if (minDist == radius) {
    foundInt.push_back(nearestPt);
    return foundInt;
  }
  // if is secant
  float c = (float)sqrt(pow(radius, 2) - pow(minDist, 2));
  // We have magnitude, getting vector
  Vector2 vc = line._direction * c;
  // Add and substract normal vector
  Vector2 s1 = d1 + vc + pos;
  Vector2 s2 = d1 - vc + pos;
  foundInt.push_back(s1);
  foundInt.push_back(s2);
  return foundInt;
}

// Function to get de minimal distance two moving objects will be
pair<double, double> minDistance(Vector2 pos1, Vector2 pos2, Vector2 vel1, Vector2 vel2) {
  // Distance vector from 2 to 1
  Vector2 d21 = pos2 - pos1;
  float distance12 = d21.Length();
  // Relative velocity of 2 wrt 1
  Vector2 v21 = (vel2 - vel1);
  // Detect if they move away or are parallel to rule out subsequent calculations
  Vector2 ptest1 = pos1 + vel1;
  Vector2 ptest2 = pos2 + vel2;
  float distTest = (ptest1 - ptest2).Length();
  if (distTest >= distance12) {
    return pair<double, double>(distance12, 0);
  }
  // Minimal distance between 1 and 2
  double minDist = abs(vecProduct(d21, v21) / v21.Length());
  double distance = sqrt(pow(distance12, 2) - pow(minDist, 2));
  double ttca21 = abs(distance / v21.Length());
  return pair<double, double>(minDist, ttca21);
}

// Function to get the normal of a vector
Vector2 getNormal(Vector2 v) { return Vector2(-v._y, v._x); }


// Calculates the cosen of the angle between two vectors. If they are paralel, cos > 0, they are on
// the same direction
float vectorsCos(Vector2 u, Vector2 v) { return dotProduct(u, v) / (u.Length() * v.Length()); }

// Function to check if two points are on the same side of a line
bool sameSide(Line l, Vector2 a, Vector2 b) {
  Vector2 nearestToA = l.nearestPt(a);
  Vector2 nearestToB = l.nearestPt(b);
  Vector2 distA = a - nearestToA;
  Vector2 distB = b - nearestToB;
  if (vectorsCos(distA,distB) > 0)
    return true;
  return false;
}

// Function to filter neighbours according to EACS
void MPModifier::FilterNeighbours(const BaseAgent* evalAgent, vector<const BaseAgent*> &filteredNeighbours, Vector2 prefVel, double &minMIT, int maxMITneigh) {
  // Variables for back space delimit
  Vector2 pos = evalAgent->_pos;
  float maxSpeed = evalAgent->_maxSpeed;
  Vector2 vel = prefVel;
  Vector2 orient = vel;
  orient.normalize();
  Vector2 frontPoint = pos + orient;
  Line backLine(pos, getNormal(orient));  // Line delimiting back space
  multimap<double, int> filtered;
  for (int i = 0; i < evalAgent->_nearAgents.size(); ++i) {
    const BaseAgent* const other = evalAgent->_nearAgents[i].agent;
    Vector2 otherVel = other->_velPref.getPreferredVel();
    // Filtering the agent
    // First step. Visibility
    float distance = Vector2(other->_pos - pos).Length();
    // Discard backwards agents
    if (!sameSide(backLine, frontPoint, other->_pos) && distance >= 1.f) continue;
    // Second step. Calculate MIT
    Vector2 centerVel = other->_pos + otherVel;
    Vector2 p1p2 = other->_pos - pos;
    Vector2 dir12 = p1p2;
    dir12.normalize();
    Line p1p2Line(other->_pos, dir12);
    Vector2 ndir12(-dir12._y, dir12._x);
    Line perpLine(other->_pos, ndir12);
    list<Vector2> interPoints = interCircLine(centerVel, orient * maxSpeed, p1p2Line);
    // Discard when none intersection points found
    if (interPoints.size() == 0) continue;
    double MIT = 100;
    for (Vector2 item : interPoints) {
      float distTo = (item - pos).Length();
      // Discard when the intersection point is at the other side of de perp line
      if (!sameSide(perpLine, pos, item)) continue;
      Vector2 IS = centerVel - item;
      Vector2 v2v1 = otherVel - IS;
      double newMIT = p1p2.Length() / v2v1.Length();
      if (newMIT < MIT) MIT = newMIT;
    }
    // insert agent in the list with their MIT
    filtered.insert({MIT, i});
  }
  // Define the minMIT to further calculation of the frecuency
  if (filtered.empty()) {
    minMIT = evalAgent->_neighborDist / (evalAgent->_maxSpeed - evalAgent->_prefSpeed);
  } else {
    minMIT = filtered.begin()->first;
  }
  // Limit to max MIT neighbours number.
  multimap<double, int>::iterator itr;
  int j = 1;
  for (itr = filtered.begin(); itr != filtered.end(); ++itr) {
    filteredNeighbours.push_back(evalAgent->_nearAgents[itr->second].agent);
    j = ++j;
    if (j > maxMITneigh) break;
  }
}

// Function to check if the evaluation agent collides with any other
bool MPModifier::Collision(Segment bc, const BaseAgent* evalAgent, const BaseAgent*& collAgent,
                           vector<const BaseAgent*> filteredNeighbours, double &ttca) {
  bool collision = false;
  // Variables
  Vector2 pos1 = bc.start;
  Vector2 vel1 = bc.length / (float)bc.time * bc.getOrient();
  for (size_t i = 0; i < filteredNeighbours.size(); ++i) {
    const BaseAgent* other = filteredNeighbours[i];
    Vector2 vel2 = other->_velPref.getPreferredVel();
    Vector2 pos2 = other->_pos + vel2 * (float)bc.eTime;
    float critDist = 0.98f * (other->_radius + evalAgent->_radius);
    pair<double, double> minDist = minDistance(pos1, pos2, vel1, vel2);
    Vector2 mindistPoint = pos1 + vel1 * (float)minDist.second;
    if ((minDist.first < critDist) &&
        (bc.length + evalAgent->_radius + other->_radius > (mindistPoint - pos1).Length())) {
      collision = true;
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << "Collision with: " << other->_id << " point " << mindistPoint << " mindist "
           << minDist.first << " ttca " << minDist.second << " vel2 " << other->_vel
           << other->_vel.Length() << endl;
      ttca = minDist.second;
      collAgent = other;
      break;
    }
  }
  return collision;
}

// Function to get the CIW points of a colliding agent
list<pair<Vector2, double>> MPModifier::ComputeCIW(Segment bc, const BaseAgent* evalAgent,
                                                   const BaseAgent* collAgent, double ttca) {
  list<pair<Vector2, double>> lista;
  list<Vector2> intersecPoints;
  list<Segment> CIW;
  Vector2 pos1 = bc.start;
  Vector2 vel1 = bc.getOrient() * bc.length / (float)bc.time;
  Vector2 vel2 = collAgent->_velPref.getPreferredVel();
  Vector2 pos2 = collAgent->_pos + vel2 * bc.eTime;
  Vector2 d21 = (pos2 - pos1);
  double dist21 = d21.Length();
  double critDist = (collAgent->_radius + evalAgent->_radius);
  double sinA = critDist / dist21;
  double cosA = (float)sqrt(1 - pow(sinA, 2));
  double x = d21._x;
  double y = d21._y;
  // Line in the direction of vel1
  Line l(pos1, vel1);
  // end of vel1 placed in pos1
  Vector2 v1end = pos1 + vel1;
  // Tangents to 2 increased through 1 reduced.
  Vector2 t1((float)(x * cosA - y * sinA), (float)(x * sinA + y * cosA));
  Vector2 t2((float)(x * cosA + y * sinA), (float)(-x * sinA + y * cosA));
  t1.normalize();
  t2.normalize();
  Line lt1(pos1 + vel2, t1);
  Line lt2(pos1 + vel2, t2);
  // Intersection between tangent lines and circumference of vel1
  // with lt1
  list<Vector2> distl = interCircLine(pos1, vel1, lt1);
  // If there are two points, discard the one furthest from the tip of vel1
  if (distl.size() > 1) {
    Vector2 sol = distl.front();
    float distTov1end1 = abs((v1end - sol).Length());
    sol = distl.back();
    float distTov1end2 = abs((v1end - sol).Length());
    if (distTov1end1 < distTov1end2)
      distl.pop_back();
    else
      distl.pop_front();
  }
  // Left cut
  intersecPoints.splice(intersecPoints.end(), distl);
  distl = interCircLine(pos1, vel1, lt2);
  // with lt2
  // If there are two points, discard the one furthest from the tip of vel1
  if (distl.size() > 1) {
    Vector2 sol = distl.front();
    float distTov1end1 = abs((v1end - sol).Length());
    sol = distl.back();
    float distTov1end2 = abs((v1end - sol).Length());
    if (distTov1end1 < distTov1end2)
      distl.pop_back();
    else
      distl.pop_front();
  }
  // Rigth cut
  intersecPoints.splice(intersecPoints.end(), distl);
  // Check if there is a following or an opposing case
  bool isFollowing = false;
  bool isOpposition = false;
  double vel1Angle = vectorsCos(vel1, d21);
  // in case of agent 2 is not moving
  double vel2Angle = 1;
  if (vel2.Length() != 0.f) vel2Angle = vectorsCos(vel2, d21);
  
  // Following and facing detection
  if (abs(vel1Angle) >= abs(cosA) && abs(vel2Angle) >= abs(cosA) && vectorsCos(vel1, vel2) > 0) {
    // Case detected
    if (vectorsCos(vel1, vel2) > 0) {
      // Same directions. Following case
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << " Following case " << endl;
      isFollowing = true;
    } else {
      // Oposite directions. Encounter
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << " Facing case " << endl;
      isOpposition = true;
    }
  } else {
    // Intersection between the line of vel1 and t1 and t2
    distl = interLines(l, lt1);
    intersecPoints.splice(intersecPoints.end(), distl);
    distl = interLines(l, lt2);
    intersecPoints.splice(intersecPoints.end(), distl);
  }
  for (Vector2 item : intersecPoints) {
    if (trace)
      cout << "(" << evalAgent->_id << ") "
           << "CIW velocity " << item - pos1 << (item - pos1).Length() << endl;
    // Discard if the new speed is equal to that of agent 2
    if (item - pos1 == vel2) continue;
    Vector2 nvel1 = item - pos1;
    pair<double, double> minDist = minDistance(pos1, pos2, nvel1, vel2);
    double time = minDist.second +
                  min(abs(minDist.first) / nvel1.Length(), abs(minDist.first) / vel2.Length());
    if (time != time) time = 0;
    pair<Vector2, double> newPos(pos1 + nvel1 * (float)time, time);
    // Discard points within obstacles
    bool collision = false;
    for (size_t i = 0; i < evalAgent->_nearObstacles.size(); ++i) {
      Menge::Agents::NearObstacle obstaculo = evalAgent->_nearObstacles[i];
      const Obstacle* const obst = evalAgent->_nearObstacles[i].obstacle;
      Vector2 nearPt;
      float distSq;
      Vector2 paso=nvel1;
      paso.normalize();
      float distancia = (newPos.first - pos1).Length();
      if (distancia > 10) distancia = 10;
      float mintowall = 200;
      for (float a = 0; a < distancia; a = a + 0.1) {
        Vector2 punto = pos1 + paso * a;
        obst->distanceSqToPoint(punto, nearPt, distSq);
        if (sqrt(distSq) < mintowall) mintowall = sqrt(distSq);
      }
      // Discard inside wall
      float tiempo = obst->circleIntersection(nvel1, pos1, evalAgent->_radius);
      obst->distanceSqToPoint(pos1 + nvel1 * tiempo, nearPt, distSq);
      if ( tiempo < time) {
        if (trace)
          cout << "(" << evalAgent->_id << ") "
               << "Discarded CIW point " << newPos.first << " inside obstacle" << endl;
        collision = true;
        break;
      }
    }
    if (!collision) {
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << "Accepted CIW point " << newPos.first << endl;
      lista.push_back(newPos);
    }
  }
  // Watch when no valid points
  if (lista.empty()) {
    if (trace)
      cout << "(" << evalAgent->_id << ") "
           << "No valid turn points" << endl;
    // Add new points when following
    if (isFollowing) {
      if (dist21 - critDist > _followDist) {
        // Approaching
        if (trace)
          cout << "(" << evalAgent->_id << ") "
               << "New aproaching point " << endl;
        Vector2 colPoint = pos1 + vel1 * ttca;
        Vector2 colDist = colPoint - pos1;
        float nspeed = (colDist.Length() - _followDist - critDist) / ttca;
        Vector2 nVel = vel1;
        nVel.normalize();
        nVel = nVel * nspeed;
        if (trace) cout << "(" << evalAgent->_id << ") " << pos1 + nVel * ttca << endl;
        lista.push_back(make_pair(pos1 + nVel * ttca, ttca));
      } else {
        // Equalization
        if (trace)
          cout << "(" << evalAgent->_id << ") "
               << "New equalization points " << endl;
        lista.push_back(make_pair(bc.end, bc.length / vel2.Length()));
        lista.push_back(make_pair(pos1 + vel2 * bc.time, bc.time));
      }
    } else {
      // Possible facing case. Not implemented
    }
  }
  return lista;
}

// Function to get the frecuency from the agent`s speed
int frecuency(const BaseAgent* evalAgent, double prefSpeed, double minMIT) {
  double maxMIT =
      (evalAgent->_neighborDist + evalAgent->_radius * 2) / (evalAgent->_maxSpeed + prefSpeed);
  double freq = minPeriod + (minMIT * (maxPeriod - minPeriod) / maxMIT);
  if (trace)
    cout << "(" << evalAgent->_id << ") "
         << " MaxMIT " << maxMIT << " freq " << freq << endl;
  if (freq > maxPeriod) freq = maxPeriod;
  return freq;
}

// Function to get the cost of a segment
double costCalc(Segment ba, double prefSpeed) {
  if (ba.length == 0) return 0;
  double speed = ba.length / ba.time;
  double ew = 2.23 / pow(prefSpeed, 2);
  double cost = mass * (2.23 / speed + ew * speed) * ba.length;
  return cost;
}

// Function to get the path from the collision-free list
Vector2 MPModifier::buildPath(list<Segment> collisionFreeList, const BaseAgent* evalAgent) {
  Vector2 target = evalAgent->_velPref.getTarget();
  Segment sol = collisionFreeList.front();
  for (auto item : collisionFreeList) {
    if (_trace)
      cout << "(" << evalAgent->_id << ") "
           << " Segment to process " << item.start << item.end << " time " << item.time << endl;
    if (item.end != target) {
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << " No match " << endl;
      continue;
    }
    if (item.start == evalAgent->_pos) {
      if (trace)
        cout << "(" << evalAgent->_id << ") "
             << "Found solution " << endl;
      sol = item;
      break;
    }
    target = item.start;
  }
  if (trace)
    cout << "(" << evalAgent->_id << ") "
         << " Solution segment " << sol.start << sol.end << " time " << sol.time << endl;
  Vector2 newVel = sol.getOrient() * sol.length / (float)sol.time;
  return newVel;
}

// Function to check if a segment is used
bool usedSegment(Segment segment, vector<Segment> usedSegs) {
  for (int i = 0; i < usedSegs.size(); i++) {
    if (usedSegs[i].equal(segment)) return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

// MAIN FUNCTION
void MPModifier::adaptPrefVelocity(const BaseAgent* agent, PrefVelocity& pVel) {
  // Variables
  const BaseAgent* evalAgent = agent;
  const BaseAgent* collAgent = agent;
  vector<const BaseAgent*> filteredNeighbours;
  vector<Segment> usedSegs;
  double minMIT = 0;
  double ttca;

  trace = _trace;
  int maxMITneigh = _maxMITneigh;
  if (trace) cout << "(" << agent->_id << ") " << endl
         << "AGENT: " << agent->_id << " substep " << stepCount << " pos " << agent->_pos << " vel "
         << agent->_vel << agent->_vel.Length() << endl;
  incrementCounter();
  //return;
  
  // Plan remainder

  if (!isInTab(agent->_id)) {
    writeOnTab(agent->_id, 0, agent->_vel);
    if (trace) cout << "(" << agent->_id << ") " << " Inicialized with 0 steps" << endl;
    return;
  } else {
    if (!zeroOnTab(agent->_id)) {
      Vector2 sameVel = decrementOnTab(agent->_id);
      Vector2 sameDir = sameVel;
      sameDir.normalize();
      pVel.setSpeed(sameVel.Length());
      pVel.setSingle(sameDir);
      if (trace) cout << "(" << agent->_id << ") " << " Remaining steps to actualization " << freqTab[agent->_id].first + 1 << endl;
      return;
    }
  }

  // Condition to deactivate midplan when it is already close to the objective
  if ((agent->_pos - agent->_velPref.getTarget()).Length() > 1.5) {
    filteredNeighbours.clear();
    usedSegs.clear();
    double elapsedTime = 0.f;
    Vector2 prefVel = pVel.getPreferredVel();
    Vector2 prefDir = prefVel;
    prefDir.normalize();
    double prefSpeed = prefVel.Length();
    

    // define temporary vars
    list<Segment> collisionFreeList;
    multimap<double, Segment> checkList;
    double collisionCost = mass * 2 * (2.23 / prefSpeed) * detourDistance;
    
    // EACS ALGORITHM
    FilterNeighbours(evalAgent,filteredNeighbours, prefVel, minMIT, maxMITneigh);
    if (trace) cout << "(" << agent->_id << ") " << "Filtered agents " << filteredNeighbours.size() << endl;
    if (filteredNeighbours.size() == 0) return;
    Segment sg(agent->_pos, agent->_velPref.getTarget(), 0, 0);
    sg.time = sg.length / prefSpeed;
    sg.cost = costCalc(sg, prefSpeed);
    sg.collision = -1;
    sg.expansions = 0;
    usedSegs.push_back(sg);
    checkList.emplace(sg.cost, sg);
    bool pathFound = false;
    while (!pathFound) {
      if (trace) cout << "(" << agent->_id << ") " << "Checklist" << endl;
      // If trace, print checklist
      if (trace) {
        for (auto segmento : checkList) {
          cout << "(" << agent->_id << ") "
               << " Segment (" << segmento.second.start << "," << segmento.second.end << ") cost "
                 << segmento.first << endl;
        }
      }
      if (checkList.empty()) {
        collisionFreeList.push_front(sg);
        pathFound = true;
        break;
      }
      Segment bc = checkList.begin()->second;
      checkList.erase(checkList.begin());
      if (trace) cout << "(" << agent->_id << ") " << "Segment to check " << bc.start << "," << bc.end << endl;

      if (Collision(bc, evalAgent, collAgent, filteredNeighbours, ttca)) {

        // Check number of expansions
        if (trace) cout << "(" << agent->_id << ") " << "Expansions: " << bc.expansions << endl;
        if (bc.expansions >= 5) {
          if (trace) cout << "(" << agent->_id << ") " << "No more expasions allowed" << endl;
          continue;
        }
        // Check checklist length
        if (checkList.size() > 30) {
          if (trace) cout << "(" << agent->_id << ") " << "No more segments allowed" << endl;
          continue;
        }
        // Check if the colliding agent is over the target point.
        if ((collAgent->_pos - evalAgent->_velPref.getTarget()).Length() < (evalAgent->_radius + collAgent->_radius)) {
          if (trace) cout << "(" << agent->_id << ") " << " Colliding agent on target point. Accept segment" << endl;
          collisionFreeList.push_front(bc);
          if (bc.end == sg.end) {
            pathFound = true;
          } else {
            // Accept collision adding extracost
            bc.cost = bc.cost + collisionCost;
            Segment cg(bc.end, sg.end, bc.eTime + bc.time, bc.eCost + bc.cost);
            cg.time = cg.length / prefSpeed;
            cg.cost = costCalc(cg, prefSpeed);
            cg.expansions = ++bc.expansions;
            checkList.emplace(cg.eCost + cg.cost, cg);
          }
          continue;
        }
        // Check if the collision is with the same agent than origin of expanssion
        if (collAgent->_id == bc.collision) {
          // Collision with same agent
          if (trace) cout << "(" << agent->_id << ") " << " Issue with same agent as before. Accept segment with extracost and continue" << endl;
          collisionFreeList.push_front(bc);
          if (bc.end == sg.end) {
            pathFound = true;
          } else {
            // Accept collision adding extracost
            bc.cost = bc.cost + collisionCost;
            Segment cg(bc.end, sg.end, bc.eTime + bc.time,
                       bc.eCost + bc.cost);
            cg.time = cg.length / prefSpeed;
            cg.cost = costCalc(cg, prefSpeed);
            cg.expansions = ++bc.expansions;
            checkList.emplace(cg.eCost + cg.cost, cg);
          }
        } else {
          // Collision with new agent
          list<pair<Vector2, double>> pointsList = ComputeCIW(bc, evalAgent, collAgent, ttca);
          // If no valid points
          if (pointsList.empty()) {
            if (trace) cout << "(" << agent->_id << ") " << "No valid points. Create same segment with extracost" << endl;
            bc.collision = collAgent->_id;
            bc.cost = bc.cost + collisionCost;
            checkList.emplace(bc.eCost + bc.cost, bc);
          } else {
            if (bc.expansions >= _maxExpansions) continue;
            for (pair<Vector2, double> item : pointsList) {
              Segment ba(bc.start, item.first, bc.eTime, bc.eCost);
              if (usedSegment(ba, usedSegs)) continue;
              ba.time = item.second;
              ba.collision = collAgent->_id;
              ba.cost = costCalc(ba, prefSpeed);
              ba.expansions = bc.expansions;
              Segment ag(ba.end, sg.end, ba.eTime + ba.time, ba.eCost + ba.cost);
              ag.time = ag.length / prefSpeed;
              ag.cost = costCalc(ag, prefSpeed);
              usedSegs.emplace(usedSegs.begin(),ba);
              if (trace) cout << "(" << agent->_id << ") " << "CIW" << item.first << " Cost to segment " << ag.eCost << " to G " << ag.cost 
                   << " Total " << ag.eCost + ag.cost << endl;
              checkList.emplace(ag.eCost + ag.cost, ba);
            }
          }
        }
      } else {
        if (trace) cout << "(" << agent->_id << ") " << "-Free" << endl;
        collisionFreeList.push_front(bc);
        if (bc.end != sg.end) {
          Segment cg(bc.end, sg.end, bc.eTime + bc.time,
                     bc.eCost + bc.cost);
          cg.time = cg.length / prefSpeed;
          cg.cost = costCalc(cg, prefSpeed);
          cg.collision = bc.collision;
          cg.expansions = ++bc.expansions;
          checkList.emplace(cg.eCost + cg.cost, cg);
        } else
          pathFound = true;
      }
    }
    Vector2 newVel = buildPath(collisionFreeList, evalAgent);
    float newSpeed = newVel.Length();
    Vector2 newDir = newVel;
    newDir.normalize();

    pVel.setSpeed(newSpeed);
    pVel.setSingle(newDir);

    int newPeriod = frecuency(evalAgent, prefSpeed, minMIT);
    writeOnTab(agent->_id, newPeriod, newVel);
    if (trace) cout << "(" << agent->_id << ") " << " New steps to actualization " << newPeriod + 1 << endl;
    if (trace) cout << "(" << agent->_id << ") " << " NewVel" << newVel << " " << newSpeed << endl << endl;
  }
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of MPModFactory
/////////////////////////////////////////////////////////////////////

MPModifierFactory::MPModifierFactory() : VelModFactory() {
  _followDistID = _attrSet.addFloatAttribute("follow_dist", false, 0.5f);
  _maxExpansionsID = _attrSet.addFloatAttribute("max_expan", false, 5);
  _maxMITneighID = _attrSet.addFloatAttribute("max_neigh", false, 5);
  _traceID = _attrSet.addBoolAttribute("trace", false, false);
}

/////////////////////////////////////////////////////////////////////

bool MPModifierFactory::setFromXML(VelModifier* modifier, TiXmlElement* node,
                                   const std::string& behaveFldr) const {
  MPModifier* MPMod = dynamic_cast<MPModifier*>(modifier);
  assert(MPMod != 0x0 && "Trying to set property modifier properties on an incompatible object");

  if (!VelModFactory::setFromXML(modifier, node, behaveFldr)) return false;

  // set the params we need
  MPMod->setFollowDist(_attrSet.getFloat(_followDistID));
  MPMod->setMaxExpansions(_attrSet.getFloat(_maxExpansionsID));
  MPMod->setMaxMITneigh(_attrSet.getFloat(_maxMITneighID));
  MPMod->setTrace(_attrSet.getBool(_traceID));
  return true;
}
}  // namespace MPModifier

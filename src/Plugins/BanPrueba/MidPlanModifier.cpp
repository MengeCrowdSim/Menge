/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation
for educational, research, and non-profit purposes, without fee, and without a
written agreement is hereby granted, provided that the above copyright notice,
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North
Carolina at Chapel Hill. The software program and documentation are supplied "as is,"
without any accompanying services from the University of North Carolina at Chapel
Hill or the authors. The University of North Carolina at Chapel Hill and the
authors do not warrant that the operation of the program will be uninterrupted
or error-free. The end-user understands that the program was developed for research
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MidPlanModifier.h"
#include "SortedList.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryStructs.h"
#include "MengeCore/Math/Line.h"
#include <list>
#include <map>
#include <iterator>



namespace BPModifier {

using Menge::TWOPI;
using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::VelModFactory;
using Menge::BFSM::VelModifier;
using Menge::Math::FloatGenerator;
using Menge::Math::Vector2;
using Menge::Math::Line;

const BaseAgent* evalAgent;
const BaseAgent* collAgent;
vector<const BaseAgent*> filteredNeighbours;
//multimap<float, BPModifier::Segment> mapa;
float prefSpeed;
Vector2 prefVel;
//Vector2 prefDir;
Vector2 critPt;
float norm;
float density;
float areaSq2Inv;
double elapsedTime;

int agtCol;
double ttca;

int contador = 1;


/////////////////////////////////////////////////////////////////////
//                   Implementation of BPModifier
/////////////////////////////////////////////////////////////////////

BPModifier::BPModifier()
    : VelModifier(), _bufferGen(0x0), _factorGen(0x0), _sigmaAgent(1.5f), _sigmaObstacle(0.75f) {}

/////////////////////////////////////////////////////////////////////

BPModifier::BPModifier(FloatGenerator* buffer, FloatGenerator* factor, float sigmaAgent,
                       float sigmaObstacle)
    : VelModifier(),
      _bufferGen(buffer),
      _factorGen(factor),
      _sigmaAgent(sigmaAgent),
      _sigmaObstacle(sigmaObstacle) {}

/////////////////////////////////////////////////////////////////////

VelModifier* BPModifier::copy() const {
  return new BPModifier(_bufferGen->copy(), _factorGen->copy(), _sigmaAgent, _sigmaAgent);
};

float dotProduct(Vector2 u, Vector2 v) { return (u._x * v._x) + (u._y * v._y); }

float vecProduct(Vector2 u, Vector2 v) { return (u._x * v._y) - (u._y * v._x); }


/////////////////////////////////////////////////////////////////////

list<Vector2> interLines(Line l1, Line l2) {
  list<Vector2> foundInt;
  float a1 = l1._direction._y;
  float b1 = -l1._direction._x;
  float c1 = l1._direction._x * l1._point._y - l1._direction._y * l1._point._x;
  float a2 = l2._direction._y;
  float b2 = -l2._direction._x;
  float c2 = l2._direction._x * l2._point._y - l2._direction._y * l2._point._x;
  float x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
  float y = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1);
  cout << " Linea l1 " << l1._direction << " l2 " << l2._direction << endl;
  //cout << "Linea" << x << "," << y << endl;
  if (x != INFINITY && y != INFINITY) {
    foundInt.push_back(Vector2(x, y));
  }
  return foundInt;
}

list<Vector2> interCircLine(Vector2 pos, Vector2 rad, Line line) {
  list<Vector2> foundInt;
  float radius = rad.Length();
  // Intersección entre línea y circunferencia
  Vector2 nearestPt = line.nearestPt(pos);
  Vector2 d1 = nearestPt - pos;
  float minDist = d1.Length();
  // Check if there is intersection
  //cout << "mindist " << minDist << " radio " << radius << endl;
  if (minDist > radius) return foundInt;
  float c = (float) sqrt(pow(radius, 2) - pow(minDist, 2));
  // Tenemos magnitud, sacamos vector
  //cout << "Direccion de la recta " << line._direction << " c " << c << endl;
  Vector2 vc = line._direction * c;
  // Sumamos y restamos
  //cout << "Vector minDist " << d1 << " vector vc " << vc << endl;
  Vector2 s1 = d1 + vc + pos;
  Vector2 s2 = d1 - vc + pos;
  //cout << "Vector minDist " << d1 << " vector vc " << vc << endl;
  foundInt.push_back(s1);
  if (s1 != s2) foundInt.push_back(s2);
  return foundInt;
}

pair<double, double> minDistance(Vector2 pos1, Vector2 pos2, Vector2 vel1, Vector2 vel2) {
  // Distance vector from 2 to 1
  Vector2 d21 = (pos2 - pos1);
  // Relative velocity of 2 wrt 1
  Vector2 v21 = (vel2 - vel1);
  // Minimal distance between 1 and 2
  //float producto = vecProduct(d21, v21);
  //cout << "Producto vec " << producto << " distancia " << v21.Length() << endl;
  double minDist = abs(vecProduct(d21, v21) / v21.Length());
  //cout << " -- Velocidades " << vel1 << vel2 << endl;
  //cout << "    Pos 2 " << pos2 << endl;
  double distance = sqrt(pow(d21.Length(), 2) - pow(minDist, 2));
  //cout << " distance " << distance << endl;
  double ttca21 = abs(distance / v21.Length());
  //cout << " minDist " << minDist << " ttca " << ttca21 << endl;
  // cout << "Colision " << item.first << endl;
  return pair<double, double>(minDist, ttca21);
}

///

int sign(float x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}
bool sameOrient(Vector2 v, Vector2 u) {
  if (sign(v._x) == sign(u._x) && sign(v._y) == sign(u._y)) return true;
  return false;
}

void FilterNeighbours() {
  // Variables for back space delimit
  Vector2 orient = prefVel;
  orient.normalize();
  Vector2 pos = evalAgent->_pos;
  float speed = prefSpeed;
  float maxSpeed = evalAgent->_maxSpeed;
  Vector2 vel = orient * speed;
  Vector2 perp(-orient.y(), orient.x());  // Vector perpendicular to the orient
  Line backLine(pos, perp);              // Line delimiting back space
  //cout << "Linea trasera " << backLine._direction << endl;
  multimap<double, int> filtered;
  //cout << "Agentes a filtrar " << evalAgent->_nearAgents.size() << endl;
  for (int i = 0; i < evalAgent->_nearAgents.size(); ++i) {
    //filtered.emplace(0, i);
    //continue;

    const BaseAgent* const other = evalAgent->_nearAgents[i].agent;
    Vector2 otherVel = other->_velPref.getPreferredVel();

    // Filtering the agent
    // First step. Visibility
    float distance = Vector2(other->_pos - pos).Length();
    Vector2 nearestPt = backLine.nearestPt(other->_pos);
    //cout << "Punto mas cercano " << nearestPt << endl;
    Vector2 distanceToLine = other->_pos - nearestPt;
    distanceToLine.normalize();
    // Discard backwards agents
    // cout << "velocidad " << orient << " vector distancia " << distanceToLine << endl;
    // if (!sameOrient(orient,distanceToLine) && distance >= 1.f) {
    if (!sameOrient(orient, distanceToLine) && distance >= 1.f) continue;

    // Second step. Calculate MIT
  
    Vector2 centerVel = other->_pos + otherVel;
    Vector2 p1p2 = other->_pos - pos;
    p1p2.normalize();
    Line p1p2Line(other->_pos, p1p2);
    Vector2 np1p2(-p1p2._y, p1p2._x);
    np1p2.normalize();
    Line perpLine(other->_pos, np1p2);

    list<Vector2> interPoints = interCircLine(centerVel, orient*maxSpeed, p1p2Line);
    if (interPoints.size() == 0) {
      //cout << "fuera " << endl;
      continue;
    }
    float velPos = 50.f;
    Vector2 point;
    for (Vector2 item : interPoints) {
      float distTo = (item - pos).Length();
      if (distTo < p1p2.Length()) continue;
      if (distTo < velPos) {
        velPos = distTo;
        point = item;
      }
    }
    Vector2 IS = centerVel - point;
    double MIT = minDistance(pos, other->_pos, IS, otherVel).second;
    filtered.emplace(MIT, i);

  }
  //cout << "Agentes filtrados " << filtered.size() << endl;
  // "Best" neighbours number. TODO implement as param
  const int bestNumber = 5;
  multimap<double, int>::iterator itr;
  int j = 1;
  for (itr = filtered.begin(); itr != filtered.end(); ++itr) {
    filteredNeighbours.push_back(evalAgent->_nearAgents[itr->second].agent);
    j = ++j;
    if (j > bestNumber) break;
  }  

}


pair<Vector2, double> CIWpoint(Vector2 pos1, Vector2 pos2, Vector2 vel1, Vector2 vel2) {
  pair<double, double> minDistTime = minDistance(pos1, pos2, vel1, vel2);
  double time =
      minDistTime.second + min(abs(minDistTime.first) / vel1.Length(), abs(minDistTime.first) / vel2.Length());
   cout << " Vel " << vel1 << vel1.Length() << vel2 << " Mindist " << abs(minDistTime.first) << " Tiempo real "
            << minDistTime.second
       << " Tiempo " << time << endl;
  pair<Vector2, double> CIWpoint(pos1 + vel1 * (float) time, time);
  return CIWpoint;
}

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////


void BPModifier::adaptPrefVelocity(const BaseAgent* agent, PrefVelocity& pVel) {
  filteredNeighbours.clear();
  elapsedTime = 0.f;
  evalAgent = agent;
  cout << endl;
  cout << "AGENTE: " << agent->_id << " paso " << contador << " pos " << agent->_pos << " vel " << agent->_vel << agent->_vel.Length() << endl;
  contador++;
  float strideConst, speedConst;
  _paramLock.lock();

  HASH_MAP<size_t, BPParam>::iterator itr = _strideParams.find(agent->_id);
  if (itr == _strideParams.end()) {
    BPParam param(_factorGen->getValue(), _bufferGen->getValue());
    _strideParams[agent->_id] = param;
    strideConst = param._strideConst;
    speedConst = param._speedConst;
  } else {
    strideConst = itr->second._strideConst;
    speedConst = itr->second._speedConst;
  }
  _paramLock.release();


  // define temporary vars

  // TODO param
  float mass = 1;
  prefSpeed = pVel.getSpeed();
  Vector2 prefDir = pVel.getPreferred();
  prefVel = prefDir * prefSpeed;
  Vector2 newVel(1,0.1);
  newVel.normalize();
  float newSpeed = 1.08f;
  
  pVel.setSpeed(newSpeed);
  pVel.setSingle(newVel);
  cout << " NewVel" << newVel*newSpeed << " " << newSpeed << endl;
  cout << " CurrentVel" << agent->_vel << agent->_vel.Length() << endl;
  cout << " AgentPref" << agent->_velPref.getPreferredVel() << agent->_velPref.getSpeed() << endl;
  cout << endl;
  
  // Compute the maximum speed I could take for the available space
  /* float maxSpeed = speedConst * availSpace * availSpace;
  if (maxSpeed < prefSpeed) pVel.setSpeed(maxSpeed);*/
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of MPModFactory
/////////////////////////////////////////////////////////////////////

BPModifierFactory::BPModifierFactory() : VelModFactory() {
  _factorID = _attrSet.addFloatDistAttribute("factor_", true, 0.f, 1.f);
  _bufferID = _attrSet.addFloatDistAttribute("buffer_", true, 0.f, 1.f);
  _sigmaAgentID = _attrSet.addFloatAttribute("sigma_agent", false, 1.5f);
  _sigmaObstacleID = _attrSet.addFloatAttribute("sigma_obstacle", false, 0.75f);
}

/////////////////////////////////////////////////////////////////////

bool BPModifierFactory::setFromXML(VelModifier* modifier, TiXmlElement* node,
                                   const std::string& behaveFldr) const {
  BPModifier* MPMod = dynamic_cast<BPModifier*>(modifier);
  assert(MPMod != 0x0 && "Trying to set property modifier properties on an incompatible object");

  if (!VelModFactory::setFromXML(modifier, node, behaveFldr)) return false;

  // set the params we need
  MPMod->setBuffer(_attrSet.getFloatGenerator(_bufferID));
  MPMod->setFactor(_attrSet.getFloatGenerator(_factorID));
  MPMod->setSigmaAgent(_attrSet.getFloat(_sigmaAgentID));
  MPMod->setSigmaObstacle(_attrSet.getFloat(_sigmaObstacleID));
  return true;
}
}  // namespace BPModifier

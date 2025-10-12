/*

Sergio Lozano Mateos
UNED, Spain

*/

/*!
 @file		MidPlanModifier.h
 @brief		Definiton of a Velocity Modifier to EACS

 Uses the model presented in http://gamma.cs.unc.edu/DenseSense/.
 */
#ifndef __MPMODIFIER_MODIFIER_H__
#define __MPMODIFIER_MODIFIER_H__

#include "MidPlanConfig.h"

#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifierFactory.h"
#include "MengeCore/Runtime/SimpleLock.h"
#include "MengeCore/mengeCommon.h"
#include "Segment.h"


/*!
 @namespace		MPModifier
 @brief			The name space for the EACS adherence model

 This namespace contains a velocity modifier which varies preferred speed based on EACS algorithm
 */
namespace MPModifier {

using Menge::Agents::BaseAgent;
   

class MPModifierFactory;

/*!
 @brief		Velocity modifier that adapts EACS algorithm
 
 This is an implementation of the work EACS: Effective Avoidance Combination Strategy
 J. Bruneau and J. Pettre
 Inria Rennes, France

 To specify a Midplan modifier, use the following syntax:

 ```xml
 <VelModifier type="midterm_planning"
              follow_dist="1"
              max_expan="5"
              max_neigh="5"
              trace="1" />
 ```
 */

class MPMODIFIER_API MPModifier : public Menge::BFSM::VelModifier {
 public:

  /*!
   @brief		Constructor.
   */
  MPModifier();

  /*!
   @brief		Constructor.

   @param		followDist   Distance of following
   */
  MPModifier(float followDist, int maxExpansions, int maxMITneigh, bool trace);

  /*!
   @brief		Copy method for this velocity modifier.
   */
  Menge::BFSM::VelModifier* copy() const;

  /*!
   @brief		Evaluate mid-term collision
   */
  bool Collision(Segment bc, const BaseAgent* evalAgent, const BaseAgent* &collAgent,
                 vector<const BaseAgent*> filteredNeighbours, double &ttca);

  /*!
    @brief		Filter neighbours as EACS
    @param		bc          vector a calcular
  */
  void FilterNeighbours(const BaseAgent* evalAgent, vector<const BaseAgent*> &filteredNeighbours, Vector2 prefVel, double &minMIT, int maxMITneigh);

  /*!
    @brief		Compute the CIW points
  */
  std::list<std::pair<Vector2, double>> ComputeCIW(Segment bc, const BaseAgent* evalAgent,
                                                   const BaseAgent* collAgent, double ttca);

 /*!
   @brief	    Build path from collisionFreeList
 */
  Vector2 MPModifier::buildPath(list<Segment> collisionFreeList, const BaseAgent* evalAgent);

  /*!
   @brief		Adapts the given agent's preferred velocity to adhere EACS.

   @param		agent		The agent on whom we are operating.
   @param		pVel		The input preferred velocity of the agent.
   */
  void adaptPrefVelocity(const Menge::Agents::BaseAgent* agent, Menge::Agents::PrefVelocity& pVel);


  /*!
   @brief		Sets the following distance.

   @param   followDist
   */
  void setFollowDist(float followDist) { _followDist = followDist; }

  /*!
   @brief		Sets the max expansions number.

   @param   followDist
   */
  void setMaxExpansions(float maxExpansions) { _maxExpansions = maxExpansions; }

  /*!
   @brief		Sets the max neighbours number.

   @param   followDist
   */
  void setMaxMITneigh(float maxMITneigh) { _maxMITneigh = maxMITneigh; }

  /*!
   @brief		Sets the trace

   @param   followDist
   */
  void setTrace(bool trace) { _trace = trace; }

  friend class MPModifierFactory;

 protected:
  /*!
   @brief		Defines the parameters which define the Midplan behavior.
   */
  struct MPParam {
    /*!
     @brief		Default constructor
     */
    MPParam() : _followDist(0.5f), _maxExpansions(5), _maxMITneigh(5), _trace(false) {}

    /*!
     @brief		Constructor.

     @param		followDist	The following distance.
     */
    MPParam(float distance, int maxExpansions, int maxMITneigh, bool trace) {
      _followDist = distance;
      _maxExpansions = maxExpansions;
      _maxMITneigh = maxMITneigh;
      _trace = trace;
    }

    /*!
     @brief		Params
     */
    float _followDist;
    int _maxExpansions;
    int _maxMITneigh;
    bool _trace;
    ;
  };

  /*!
   @brief		The readers-writer lock to preserve thread-safety on _strideParams.
   */
  Menge::SimpleLock _paramLock;

   /*!
  @brief		The following distance
  */
  float _followDist;
  /*!
  @brief		The max expansions number
  */
  int _maxExpansions;
  /*!
  @brief		The max neighbours number in MIT
  */
  int _maxMITneigh;
  /*!
  @brief		If the trace should be shown
  */
  bool _trace;
};

////////////////////////////////////////////////////////////////////////

/*!
 @brief		The factory for the MPModifier class.
 */
class MPMODIFIER_API MPModifierFactory : public Menge::BFSM::VelModFactory {
 public:
  /*!
   @brief		Constructor.
   */
  MPModifierFactory();

  /*!
   @brief		The name of the modifier

   The modifier's name must be unique among all registered modifier.
   Each modifier factory must override this function.

   @returns	A string containing the unique modifier name.
   */
  virtual const char* name() const { return "midterm_planning"; }

  /*!
   @brief		A description of the modifier

   Each modifier factory must override this function.

   @returns	A string containing the modifier description.
   */
  virtual const char* description() const {
    return "Adjusts the  agent's preferred speed to adhere to the EACS ";
  };

 protected:
  /*!
   @brief		Create an instance of this class's modifier.

   All modifierFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding modifier type. The various field values of the instance will be set in a
   subsequent call to modifierFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns		A pointer to a newly instantiated modifier class.
   */
  Menge::BFSM::VelModifier* instance() const { return new MPModifier(); }

  /*!
   @brief		Given a pointer to an modifier instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this modifier's type (i.e.
   modifierFactory::thisFactory() has already been called and returned true). If sub-classes of
   modifierFactory introduce *new* modifier parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param		modifier	  A pointer to the modifier whose attributes are to be set.
   @param		node		    The XML node containing the modifier attributes.
   @param		behaveFldr	The path to the behavior file.  If the modifier references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns	True if successful.
   */
  virtual bool setFromXML(Menge::BFSM::VelModifier* modifier, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief		The identifier for the "follow_distance" float attribute
   */
  size_t _followDistID;
  /*!
   @brief		The identifier for the "max_expan" int attribute
   */
  size_t _maxExpansionsID;
  /*!
   @brief		The identifier for the "max_neigh" int attribute
   */
  size_t _maxMITneighID;
  /*!
   @brief		The identifier for the "trace" bool attribute
   */
  size_t _traceID;
};
}  // namespace MPModifier
#endif  // __MPMODIFIER_MODIFIER_H__

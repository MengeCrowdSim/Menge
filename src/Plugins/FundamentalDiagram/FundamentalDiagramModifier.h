/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file		FundamentalDiagramModifier.h
 @brief		Definiton of a Velocity Modifier to enforce FD adherance

 Uses the model presented in http://gamma.cs.unc.edu/DenseSense/.
 */
#ifndef __FDMODIFIER_MODIFIER_H__
#define __FDMODIFIER_MODIFIER_H__

#include "FundamentalDiagramConfig.h"

#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifierFactory.h"
#include "MengeCore/Runtime/SimpleLock.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace		FDModifier
 @brief			The name space for the Fundamental Diagram adherence model

 This namespace contains a velocity modifier which varies preferred speed based on local density
 conditions.
 */
namespace FDModifier {

class FDModifierFactory;

/*!
 @brief		Velocity modifier that adapts preferred velocity to account for local density.
 
 This produces a density-dependent behavior which can conform to the fundamental diagram (depending
 on the settings.)

 This is an implementation of the work found at
 *http://gamma.cs.unc.edu/PEDS/download/curtis12_FD.pdf
 It is a model of the underlying causes of the so-called *fundamental diagram* -- the name for the
 density-dependent behavior observed in pedestrians. As the crowd gets denser, the crowd moves more
 slowly. It uses two parameter: stride factor and stride buffer. The stride *factor* models
 physiological factors that correlate available space to stride length (and therefore walking
 speed). A typical mean value is 1.57. The stride *buffer* models psychological factors which
 determine how the agent perceives the physical space as a *comfortable* space. Typical values are
 in the range 0.5-0.9

 To specify a fundamental velocity modifier, use the following syntax:

 ```xml
 <VelModifier type="fundamental_diagram"
              buffer_dist="c" buffer_value="float"
              factor_dist="c" factor_value="float" />
 ```

 The parameters have the following meanings:
   - The value for `buffer_dist` defines the numerical distribution for the stride *buffer* value.
     In the example given above, it is a constant distribution. For other distributions (i.e.,
     uniform (`u`) or normal (`n`), `buffer_value` would be replaced by the pairs `buffer_min` and
     `buffer_max` or `buffer_mean` and `buffer_stddev`, respectively.
   - The value for `factor_dist` defines the numerical distribution for the stride *factor* value.
     In the example given above, it is a constant distribution. For other distributions (i.e.,
     uniform (`u`) or normal (`n`), `factor_value` would be replaced by the pairs `factor_min` and
     `factor_max` or `factor_mean` and `factor_stddev`, respectively.
 */
class FDMODIFIER_API FDModifier : public Menge::BFSM::VelModifier {
 public:
  /*!
   @brief		Constructor.
   */
  FDModifier();

  /*!
   @brief		Constructor.

   @param		buffer          The stride buffer gemerator. This modifier takes ownership of the
                            provided generator.
   @param		factor          The stride factor gemerator. This modifier takes ownership of the
                            provided generator.
   @param		sigmaAgent      Sigma for agent density estimation
   @param		sigmaObstacle   Sigma for obstacle density estimation
   */
  FDModifier(Menge::Math::FloatGenerator* buffer, Menge::Math::FloatGenerator* factor,
             float sigmaAgent, float sigmaObstacle);

  /*!
   @brief		Copy method for this velocity modifier.
   */
  Menge::BFSM::VelModifier* copy() const;

  /*!
   @brief		Adapts the given agent's preferred velocity to adhere to the fundamental diagram.

   @param		agent		The agent on whom we are operating.
   @param		pVel		The input preferred velocity of the agent.
   */
  void adaptPrefVelocity(const Menge::Agents::BaseAgent* agent, Menge::Agents::PrefVelocity& pVel);

  /*!
   @brief		Sets the stride buffer.

   @param		buffer          The stride buffer for the adherence model
   */
  void setBuffer(Menge::Math::FloatGenerator* buffer) {
    if (_bufferGen) delete _bufferGen;
    _bufferGen = buffer;
  }

  /*!
   @brief		Sets the stride factor.

@param		factor          The stride factor for the adherence model
   */
  void setFactor(Menge::Math::FloatGenerator* factor) {
    if (_factorGen) delete _factorGen;
    _factorGen = factor;
  }

  /*!
   @brief		Sets the agent sigma.

   @param		sigma			Sigma for agent density estimation
   */
  void setSigmaAgent(float sigma) { _sigmaAgent = sigma; }

  /*!
   @brief		Sets the agent sigma.

   @param   sigma   Obstacle sigma for Probability Density Estimation
   */
  void setSigmaObstacle(float sigma) { _sigmaObstacle = sigma; }

  friend class FDModifierFactory;

 protected:
  /*!
   @brief		Defines the parameters which define the density-aware behavior.
   */
  struct FDParam {
    /*!
     @brief		Default constructor
     */
    FDParam() : _strideConst(1.f), _speedConst(1.f) {}

    /*!
     @brief		Constructor.

     @param		factor		The stride factor.
     @param		buffer		The stride buffer.
     */
    FDParam(float factor, float buffer) {
      _strideConst = 0.5f * (1.f + buffer) / factor;
      _speedConst = 1.f / (_strideConst * _strideConst);
    }

    /*!
     @brief		One of two derived constants for fundamental diagram compliance.

     This is a function of the stride factor and stride buffer
     */
    float _strideConst;

    /*!
     @brief		The second of two derived constants for fundamental diagram compliance.

     This is a function of the stride factor and stride buffer
     */
    float _speedConst;
  };

  /*!
   @brief		The readers-writer lock to preserve thread-safety on _strideParams.
   */
  Menge::SimpleLock _paramLock;

  /*!
   @brief		The per-agent parameters.
   */
  HASH_MAP<size_t, FDParam> _strideParams;

  /*!
   @brief		The Stride buffer value generator.
   */
  Menge::Math::FloatGenerator* _bufferGen;

  /*!
   @brief		The Stride factor value generator.
   */
  Menge::Math::FloatGenerator* _factorGen;

  /*!
   @brief		Agent sigma for density calculation
   */
  float _sigmaAgent;

  /*!
   @brief		Sigma for obstacle density estimation
   */
  float _sigmaObstacle;
};

////////////////////////////////////////////////////////////////////////

/*!
 @brief		The factory for the FDModifier class.
 */
class FDMODIFIER_API FDModifierFactory : public Menge::BFSM::VelModFactory {
 public:
  /*!
   @brief		Constructor.
   */
  FDModifierFactory();

  /*!
   @brief		The name of the modifier

   The modifier's name must be unique among all registered modifier.
   Each modifier factory must override this function.

   @returns	A string containing the unique modifier name.
   */
  virtual const char* name() const { return "fundamental_diagram"; }

  /*!
   @brief		A description of the modifier

   Each modifier factory must override this function.

   @returns	A string containing the modifier description.
   */
  virtual const char* description() const {
    return "Adjusts the  agent's preferred speed to adhere to the fundamental diagram ";
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
  Menge::BFSM::VelModifier* instance() const { return new FDModifier(); }

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
   @brief		The identifier for the "stride_buffer" float attribute
   */
  size_t _bufferID;

  /*!
   @brief		The identifier for the "stride_factor" float attribute
   */
  size_t _factorID;

  /*!
   @brief		The identifier for the "sigma_agent" float attribute
   */
  size_t _sigmaAgentID;

  /*!
   @brief		The identifier for the "sigma_obstacle" float attribute
   */
  size_t _sigmaObstacleID;
};
}  // namespace FDModifier
#endif  // __FDMODIFIER_MODIFIER_H__

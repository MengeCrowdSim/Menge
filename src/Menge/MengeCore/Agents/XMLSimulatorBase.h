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
 *  @file       XMLSimulatorBase.h
 *  @brief      The set of operations used by SimXMLLoader to apply
 *				XML-parsed experiment specification to a simulator.
 *
 *	In order to use the SimXMLLoader class to read XML files and set parameters
 *	The simulator must extend this abstract class and implement the given
 *	virtual functions.
 *
 */

#ifndef __XML_SIMULATOR_BASE__
#define __XML_SIMULATOR_BASE__

#include "MengeCore/Agents/SimXMLLoader.h"
#include "MengeCore/mengeCommon.h"

#include <vector>

namespace Menge {

namespace Agents {

class Elevation;
class SpatialQuery;
class BaseAgent;
class SimulatorState;

#ifdef _MSC_VER
/*!
 *	This is visual studio non-ansi compliant functionality
 *	It doesnt like functions which declare specific types of exceptions thrown
 *	This turns off the silly visual studio warning.
 */
#pragma warning(disable : 4290)
#endif

/*!
 *	@brief		Exception for invalid parameters from XML specification.
 */
class MENGE_API XMLParamException : public virtual MengeException {
 public:
  /*!
   *	@brief		Default constructor.
   */
  XMLParamException() : MengeException() {}

  /*!
   *	@brief		Constructor with message.
   *
   *	@param		s		The exception-specific message.
   */
  XMLParamException(const std::string& s) : MengeException(s) {}
};

/*!
 *	@brief		Exception thrown when the xml parameter parser has an error which cannot be
 *				recovered from.
 */
class XMLParamFatalException : public XMLParamException, public MengeFatalException {
 public:
  /*!
   *	@brief		Default constructor.
   */
  XMLParamFatalException() : MengeException(), XMLParamException(), MengeFatalException() {}

  /*!
   *	@brief		Constructor with message.
   *
   *	@param		s		The exception-specific message.
   */
  XMLParamFatalException(const std::string& s)
      : MengeException(s), XMLParamException(), MengeFatalException() {}
};

/*!
 *	@brief		The base class for extracting simulator settings from the XML
 *				specification.
 *
 *	In order to use the SimXMLLoader class to read XML files and set parameters
 *	The simulator must extend this abstract class and implement the given
 *	virtual functions.
 *
 *	A "target"-specific parameter refers to the simulator type.  In other words,
 *	If a simulator expects particular parameters, there should be a tag with
 *	the target name and a list of parameter values.  The functionality of this
 *	class defines that functionality.
 */
class MENGE_API XMLSimulatorBase {
 public:
  /*!
   *	@brief		Constructor.
   */
  XMLSimulatorBase();

  /*!
   *	@brief		Destructor.
   */
  ~XMLSimulatorBase();

  /*!
   *	@brief		Reports if there are any target-specific *experiment* parameters.
   *
   *	The default case is to assume that the simulator *does* have particular
   *	parameters.
   *
   *	@returns	True if this simulator has target-specific *experiment* parameters,
   *				false otherwise.
   */
  virtual bool hasExpTarget() { return true; }

  /*!
   *	@brief		Given the name of a tag which is the child of \<Experiment\>,
   *				reports if it contains target-specific experiment parameters.
   *
   *	It is guaranteed that the labels "AgentSet" and "Obstacle" will never
   *	be given as input.
   *
   *	@param			tagName			The name of the candidate experiment XML
   *tag.
   *	@returns	True if the given tag corresponds to the unique tag defined
   *				for this simulator (and that tag's parameters should be
   *				applied to this simulator), false if not.
   */
  virtual bool isExpTarget(const std::string& tagName) = 0;

  /*!
   *	@brief		Given the name of the xml param and its string value,
   *				sets the parameter in the simulator.
   *
   *	Common parameters and target-specific parameters will be passed into this function.
   *	It is the responsibility of the programmer to make sure the default case of
   *	any parameter should fall through to the SimulatorBase::setExpParam function.
   *
   *	@param		paramName		The name of the XML parameter.
   *	@param		value			The string representation of the parameter value.
   *								It is the responsibility of the particular sub-class
   *to translate the representation as necessary.
   *	@returns	True if the parameter was set successfully, false otherwise.
   *	@throws		An XMLParamException if there was a problem with the parameter
   *				name/value .
   */
  virtual bool setExpParam(const std::string& paramName,
                           const std::string& value) throw(XMLParamException) = 0;

  /*!
   *	@brief		Add an agent with specified position to the simulator whose properties
   *				are defined by the given agent initializer.
   *
   *	It uses the agent initializer to define the values of the remaining agent
   *	parameters.
   *
   *	@param		pos			The 2d vector representing the agent's position
   *	@param		agentInit	The AgentInitializer necessary to parse AgentSet properties
   *	@returns	A pointer to the agent (if initialization was succesful) or NULL if
   *				failed.
   */
  virtual BaseAgent* addAgent(const Math::Vector2& pos, AgentInitializer* agentInit) = 0;

  /*!
   *	@brief		Set the elevation instance of the simulator
   *
   *	@param		elevation		The elevation object.
   */
  virtual void setElevationInstance(Elevation* elevation) = 0;

  /*!
   *	@brief		Reports if the elevation has been set.
   *
   *	@returns	True if the elevation has been set, false otherwise.
   */
  virtual bool hasElevation() const = 0;

  /*!
   *	@brief		Sets the spatial query instance of the simulator.
   *
   *	@param		spatialQuery	The spatial query object.
   */
  virtual void setSpatialQuery(SpatialQuery* spatialQuery) = 0;

  /*!
   *	@brief		Gets the spatial query instance of the simulator.
   *
   *	@returns    pointer to The spatial query object.
   */
  virtual SpatialQuery* getSpatialQuery() = 0;

  /*!
   *	@brief		Reports if the spatial query has been set.
   *
   *	@returns	True if the elevation has been set, false otherwise.
   */
  virtual bool hasSpatialQuery() const = 0;

  /*!
   *	@brief		Initalize spatial query structure.
   */
  virtual bool initSpatialQuery() = 0;

  /*!
   *	@brief	After all agents and all obstacles have been added to the scene
   *			does the work to finish preparing the simulation to be run.
   */
  virtual void finalize(){};

  /*!
   *	@brief		Returns a pointer to the simulator's initial state.
   *
   *	@returns	A pointer to the intial state.
   */
  SimulatorState* getInitialState() { return _initState; }

 protected:
  /*!
   *	@brief		The initial state of the simulator.
   */
  SimulatorState* _initState;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __XML_SIMULATOR_BASE__

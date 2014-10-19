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

/*!
 *	@file		FundamentalDiagramModifier.h
 *	@brief		Definiton of a Velocity Modifier to enforce FD adherance
 *
 *	Uses the model presented in http://gamma.cs.unc.edu/DenseSense/.
 */
#ifndef __FDMODIFIER_MODIFIER_H__
#define __FDMODIFIER_MODIFIER_H__

#include "FundamentalDiagramConfig.h"
#include "VelocityModifiers/VelModifier.h"
#include "VelocityModifiers/VelModifierFactory.h"
#include "FSMEnumeration.h"
#include "SimpleLock.h"
#include "mengeCommon.h"

using namespace Menge;

/*!
 *	@namespace		FDModifier
 *	@brief			The name space for the Fundamental Diagram adherence model
 *
 *	This namespace contains a velocity modifier which varies preferred speed based on local density conditions.
 */
namespace FDModifier {

	class FDModifierFactory;

	/*!
	 *	@brief		Velocity modifier that adapts preferred velocity to account
	 *				for local density.  This produces a density-dependent behavior
	 *				which can conform to the fundamental diagram (depending on the
	 *				settings.)
	 */
	class FDMODIFIER_API FDModifier : public BFSM::VelModifier {
	public:

		/*!
		 *	@brief		Constructor.
		 */
		FDModifier();

		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		buffer          The stride buffer gemerator.  This modifier takes ownership
		 *								of the provided generator.
		 *	@param		factor          The stride factor gemerator.  This modifier takes ownership
		 *								of the provided generator.
		 *	@param		sigmaAgent      Sigma for agent density estimation
		 *	@param		sigmaObstacle   Sigma for obstacle density estimation
		 */
		FDModifier( FloatGenerator * buffer, FloatGenerator * factor, float sigmaAgent, float sigmaObstacle );

		/*!
		 *	@brief		Copy method for this velocity modifier.
		 */
		BFSM::VelModifier* copy() const;

		/*!
		 *	@brief		Adapts the given agent's preferred velocity to adhere to the fundamental diagram.
		 *
		 *	@param		agent		The agent on whom we are operating.
		 *	@param		pVel		The input preferred velocity of the agent.
		 */
		void adaptPrefVelocity( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel );

		/*!
		 *	@brief		Sets the stride buffer.
		 *
		 *	@param		buffer          The stride buffer for the adherence model
		 */
		void setBuffer( FloatGenerator * buffer) { if ( _bufferGen ) delete _bufferGen; _bufferGen = buffer; }

		/*!
		 *	@brief		Sets the stride factor.
		 *
	     *	@param		factor          The stride factor for the adherence model
		 */
		void setFactor( FloatGenerator * factor ) {if ( _factorGen ) delete _factorGen; _factorGen = factor; }

		/*!
		 *	@brief		Sets the agent sigma.
		 *
		 *	@param		sigma			Sigma for agent density estimation
		 */
		void setSigmaAgent( float sigma ) { _sigmaAgent = sigma; }
		
		/*!
		 *	@brief		Sets the agent sigma.
		 *
		 *	@param   sigma   Obstacle sigma for Probability Density Estimation
		 */
		void setSigmaObstacle( float sigma ) { _sigmaObstacle = sigma; }

		friend class FDModifierFactory;

	protected:
		/*!
		 *	@brief		Defines the parameters which define the
		 *				density-aware behavior.
		 */
		struct FDParam {
			/*!
			 *	@brief		Default constructor
			 */
			FDParam() : _strideConst(1.f), _speedConst(1.f) {}

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		factor		The stride factor.
			 *	@param		buffer		The stride buffer.
			 */
			FDParam( float factor, float buffer ) {
				_strideConst = 0.5f * ( 1.f + buffer ) / factor ;
				_speedConst = 1.f / ( _strideConst * _strideConst );
			}

			/*!
			 *	@brief		One of two derived constants for fundamental diagram compliance.
			 *				This is a function of the stride factor and stride buffer
			 */
			float	_strideConst;
			
			/*!
			 *	@brief		The second of two derived constants for fundamental diagram compliance.
			 *				This is a function of the stride factor and stride buffer
			 */
			float	_speedConst;
		};

		/*!
		 *	@brief		The readers-writer lock to preserve thread-safety
		 *				on _strideParams.
		 */
		SimpleLock _paramLock;

		/*!
		 *	@brief		The per-agent parameters.
		 */
		HASH_MAP< size_t, FDParam >	_strideParams;

	    /*!
		 *	@brief		The Stride buffer value generator.
		 */
		FloatGenerator * _bufferGen;

		/*!
		 *	@brief		The Stride factor value generator.
		 */
		FloatGenerator * _factorGen;

		/*!
		 *	@brief		Agent sigma for density calculation
		 */
		float _sigmaAgent;
		
		/*!
		 *	@brief		Sigma for obstacle density estimation
		 */
		float _sigmaObstacle;
	};

	////////////////////////////////////////////////////////////////////////
	
	/*!
	 *	@brief		The factory for the FDModifier class.
	 */
	class FDMODIFIER_API FDModifierFactory : public BFSM::VelModFactory {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		FDModifierFactory();

		/*!
		 *	@brief		The name of the modifier
		 *
		 *	The modifier's name must be unique among all registered modifier.
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the unique modifier name.
		 */
		virtual const char * name() const { return "fundamental_diagram"; }

		/*!
		 *	@brief		A description of the modifier
		 *
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the modifier description.
		 */
		virtual const char * description() const {
			return "Adjusts the  agent's preferred speed to adhere to the fundamental diagram ";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's modifier.
		 *
		 *	All modifierFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding modifier type.  The various field values
		 *	of the instance will be set in a subsequent call to modifierFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated modifier class.
		 */
		BFSM::VelModifier * instance() const { return new FDModifier(); }	
		
		/*!
		 *	@brief		Given a pointer to an modifier instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this modifier's type.
		 *	(i.e. modifierFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of modifierFactory introduce *new* modifier parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		modifier	A pointer to the modifier whose attributes are to be set.
		 *	@param		node		The XML node containing the modifier attributes.
		 *	@param		behaveFldr	The path to the behavior file.  If the modifier references
		 *							resources in the file system, it should be defined relative
		 *							to the behavior file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( BFSM::VelModifier * modifier, TiXmlElement * node, const std::string & behaveFldr ) const;

		/*!
		 *	@brief		The identifier for the "stride_buffer" float attribute
		 */
		size_t	_bufferID;

		/*!
		 *	@brief		The identifier for the "stride_factor" float attribute
		 */
		size_t	_factorID;

		/*!
		 *	@brief		The identifier for the "sigma_agent" float attribute
		 */
		size_t	_sigmaAgentID;

		/*!
		 *	@brief		The identifier for the "sigma_obstacle" float attribute
		 */
		size_t	_sigmaObstacleID;
	};
};
#endif	// __FDMODIFIER_MODIFIER_H__

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
 *	@file		FormationsModifier.h
 *	@brief		definition of a VelocityModifier to enforce formation behavior
 */
#ifndef __FORMATIONS_MODIFIER_H__
#define __FORMATIONS_MODIFIER_H__

#include "FormationsConfig.h"
#include "FreeFormation.h"
#include "VelocityModifiers/VelModifier.h"
#include "VelocityModifiers/VelModifierFactory.h"
#include "FSMEnumeration.h"
#include "ReadersWriterLock.h"

using namespace Menge;

#if defined(_MSC_VER)
    // Visual Studio spews warnings about some members.
	// It wants them to be exposed in the dll IN CASE a client uses it.
    //TODO: remove this warning supression, correct the problem
    #pragma warning(disable:4251)
#endif

//forward declaration
class TiXmlElement;

namespace Formations {

	// forward declaration
	class FormationModifierFactory;

	/*!
	 *	@brief		A velocity modifier which adapts agent preferred velocities
	 *				so that they move in formations.
	 */
	class FORMATIONS_API FormationModifier : public BFSM::VelModifier {
	public:
		/*!
		 *	@brief	Constructor
		 */
		FormationModifier();

		/*!
		 *	@brief	Constructor for given Formation
		 *
		 *	@param		form		A managed pointer to formation data
		 */
		FormationModifier( FormationPtr form );

	protected:
		/*!
		 *	@brief		Destructor.
		 */
		virtual ~FormationModifier();

	public:

		/*!
		 *	@brief		Creates a copy of this velocity modifier.
		 */
		BFSM::VelModifier* copy() const;

		/*!
		 *	@brief		Adapt the input preferred velocity according to the formation.
		 *
		 *	@param		agent		The agent whose preferred velocity is provided.
		 *	@param		pVel		The preferred velocity to modify -- modified in place.
		 */
		void adaptPrefVelocity( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel );

		/*!
		 *	@brief		Provides the task the formation modifier uses to update formations.
		 *
		 *	@returns	A pointer to the supporting task.  The caller is responsible to delete it.
		 */ 
		virtual BFSM::Task * getTask();

		/*!
		 *  @brief		Set the formation data.
		 *
		 *	@param		form		A managed resource pointer to the underlying formation data.
		 */
		void setFormation( FormationPtr form );

		/*!
		 *  @brief		Register an agent to be affected by this velocity modifier.
		 */
		void registerAgent(const Agents::BaseAgent * agent);

		/*!
		 *  @brief		Unregister an agent from being affected by this velocity modifier.
		 */
		void unregisterAgent(const Agents::BaseAgent * agent);

		/*!
		 *	@brief		Provides a display context for interacting with this velocity modifier
		 *
		 *	It is the responsibility of the caller to delete the provided context.
		 *
		 *	@returns	A pointer to a context for this velocity modifier
		 */
		//virtual VelModContext * getContext();

		friend class FormationModifierFactory;

	protected:

	    /*!
		 *	@brief		The underlying formation data.
		 */
		FormationPtr _formation;
		
		/*!
		 *	@brief		Concurrency lock for _formation.
		 */
		ReadersWriterLock	_lock;

	};

	///////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The factory class for the FormationModifier
	 */
	class FORMATIONS_API FormationModifierFactory : public BFSM::VelModFactory {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		FormationModifierFactory();

		/*!
		 *	@brief		The name of the modifier
		 *
		 *	The modifier's name must be unique among all registered modifier.
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the unique modifier name.
		 */
		virtual const char * name() const { return "formation"; }

		/*!
		 *	@brief		A description of the modifier
		 *
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the modifier description.
		 */
		virtual const char * description() const {
			return "Forces agents in this state or machine to move in formation";
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
		BFSM::VelModifier * instance() const { return new FormationModifier(); }	
		
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
		 *	@brief		The identifier for the "file_name" string attribute.
		 */
		size_t	_fileNameID;
	};
};
#endif

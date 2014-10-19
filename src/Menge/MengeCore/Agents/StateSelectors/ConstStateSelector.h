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
 *	@file		ConstStateSelector.h
 *	@brief		The definition of a state selector that assigns all agents
 *				the same initial state.
 */

#ifndef __CONST_STATE_SELECTOR_H__
#define __CONST_STATE_SELECTOR_H__

#include "mengeCommon.h"
#include "StateSelectors/StateSelector.h"
#include "StateSelectors/StateSelectorFactory.h"

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		An initial state selector that stores a single state
		 *				and assigns that state to all agents.
		 */
		class MENGE_API ConstStateSelector : public StateSelector {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ConstStateSelector() : _stateName("") {}

			/*!
			 *	@brief		Provides the name of a state.
			 *
			 *	Ths must be overriden by child classes.
			 *
			 *	@returns:		The name of the state.
			 */
			virtual std::string getState() { return _stateName; }

			/*!
			 *	@brief		Sets the name of the state.
			 *
			 *	@param		name		The name of the selector's state.
			 */
			void setStateName( const std::string & name ) { _stateName = name; }

		protected:
			/*!
			 *	@brief		The name of the profile to use.
			 */
			std::string _stateName;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for the ConstStateSelector.
		 */
		class MENGE_API ConstStateSelectorFactory : public StateSelectorFactory {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ConstStateSelectorFactory();

			/*!
			 *	@brief		The name of the state selector type.
			 *
			 *	The state selector's name must be unique among all registered state selector elements.  
			 *	Each state selector factory must override this function.
			 *
			 *	@returns	A string containing the unique state selector name.
			 */
			virtual const char * name() const { return "const"; }

			/*!
			 *	@brief		A description of the state selector.
			 *
			 *	Each state selector factory must override this function.
			 *
			 *	@returns	A string containing the state selector description.
			 */
			virtual const char * description() const {
				return "State selector which assigns the initial state to all agents.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's state selector implementation.
			 *
			 *	All StateSelectorFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding selector type.  The various field values
			 *	of the instance will be set in a subsequent call to StateSelectorFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated StateSelector class.
			 */
			StateSelector * instance() const { return new ConstStateSelector(); }

			/*!
			 *	@brief		Given a pointer to a StateSelector instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this StateSelector's type.
			 *	(i.e. StateSelectorFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of StateSelectorFactory introduce *new* StateSelector parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		sel				A pointer to the state selector whose attributes are to be set.
			 *	@param		node			The XML node containing the goal attributes.
			 *	@param		specFldr		The path to the specification file.  If the condition references
			 *								resources in the file system, it should be defined relative
			 *								to the behavior file location.  This is the folder containing
			 *								that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( StateSelector * sel, TiXmlElement * node, const std::string & specFldr ) const;

			/*!
			 *	@brief		The identifier for the "name" string parameter.
			 */
			size_t _nameID;
		};

	}	// namespace Agents
}	// namespace Menge
#endif	// __CONST_STATE_SELECTOR_H__
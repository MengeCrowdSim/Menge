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
 *	@file		GoalSelectorFactory.h
 *	@brief		The factory for parsing xml data and instantiating
 *				goals.
 */

#ifndef __GOAL_SELECTOR_FACTORY_H__
#define __GOAL_SELECTOR_FACTORY_H__

#include "CoreConfig.h"
#include "GoalSelectors/GoalSelector.h"
#include <string>

#include "ElementFactory.h"
#include "GoalSelector.h"
#include "tinyxml.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		A class for parsing the xml description of a goal selector
		 *				and instantiating particular instances.
		 */
		class MENGE_API GoalSelectorFactory : public ElementFactory< GoalSelector >  {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			GoalSelectorFactory();

		protected:
			/*!
			 *	@brief		Given a pointer to a Goal Selector instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this Goal Selector's type.
			 *	(i.e. GoalSelectorFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of GoalSelectorFactory introduce *new* GoalSelector parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		selector		A pointer to the goal selector whose attributes are to be set.
			 *	@param		node			The XML node containing the goal attributes.
			 *	@param		behaveFldr		The path to the behavior file.  If the condition references
			 *								resources in the file system, it should be defined relative
			 *								to the behavior file location.  This is the folder containing
			 *								that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( GoalSelector * selector, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "persistent" bool attribute.
			 */
			size_t	_persistentID;
		};
	}	// namespace BFSM
}	// namespace Menge
#endif // __GOAL_SELECTOR_FACTORY_H__
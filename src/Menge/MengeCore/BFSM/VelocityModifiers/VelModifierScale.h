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
 *	@file		VelModifierScale.h
 *	@brief		Provides the definition of a simple velocity modifier that simply
 *				scale's the preferred velocity's speed.
 */

#ifndef __VEL_MOD_SCALE_H__
#define	__VEL_MOD_SCALE_H__

#include "CoreConfig.h"
#include "VelModifier.h"
#include "VelModifierContext.h"  
#include "VelModifierFactory.h"

namespace Menge {

	namespace BFSM {

		/*!
		 *	@brief		A velocity modifier that scales the input velocity's speed.
		 */
		class MENGE_API ScaleVelModifier : public VelModifier {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ScaleVelModifier();

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		scale			The scale factor to apply to the preferred velocity's speed.
			 */
			ScaleVelModifier( const float scale );

			/*!
			 *	@brief		Sets the scale factor.
			 *
			 *	@param		scale			The the scale factor to apply to the preferred velocity's speed.
			 */
			void setScale( const float  scale );

			/*!
			 *	@brief		Scales the input preferred velocity's speed by the pre-defined scale
			 *				factor.	
			 *
			 *	@param		agent		The agent for which a preferred velocity is modified.
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			void adaptPrefVelocity(const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Returns scale factor.
			 *
			 *	@returns	The scale factor stored in this modifier.
			 */
			float getScale() const { return _scale; }

			/*!
			 *	@brief		Creates a copy of this velocity modifier.
			 *
			 *	@returns	A unique, deep copy of this velocity modifier.	
			 */
			VelModifier * copy() const;

			/*!
			 *	@brief		Provides a display context for interacting with this velocity modifier.
			 *
			 *	@returns	A pointer to a context for this vel modifier.
			 */
			virtual VelModContext * getContext();    //TODO: Implement the context

			friend class ScaleVMFactory;

		protected:
			/*!
			 *	@brief		The scale factor to apply.
			 */
			float _scale;

		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the ScaleVelModifier.
		 */
		//TODO: provide a context here
		class MENGE_API ScaleVMContext : public VelModContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vm			A pointer to the underlying fsm velocity modifier.
			 *							The context will *not* delete the velocity modifier.
			 */
			ScaleVMContext( ScaleVelModifier * vm );

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity modifier information.
			 *
			 *	@param		indent		An optional string representing indentation to be
			 *							applied to the text.  It is prefixed at the start
			 *							of each line.
			 *	@returns	The string for printing on the UI layer.
			 */
			virtual std::string getUIText( const std::string & indent="" ) const;

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	This should never be called in select mode.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt);

		protected:
			/*!
			 *	@brief		The underlying finite state machine velocity modifier.
			 */
			ScaleVelModifier * _vm;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the ScaleVelModifier.
		 */
		class MENGE_API ScaleVMFactory : public VelModFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ScaleVMFactory();

			/*!
			 *	@brief		The name of the velocity modifier.
			 *
			 *	The velocity modifiers's name must be unique among all registered 
			 *	velocity modifiers.  Each velocity modifier factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity modifier name.
			 */
			virtual const char * name() const { return "scale"; }

			/*!
			 *	@brief		A description of the velocity modifier.
			 *
			 *	Each velocity modifier factory must override this function.
			 *
			 *	@returns	A string containing the velocity modifier description.
			 */
			virtual const char * description() const {
				return "Scales the input preferred velocity by the given scale factor.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity modifier.
			 *
			 *	All VelModFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity modifier type.  The various field values
			 *	of the instance will be set in a subsequent call to VelModpFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelModifier class.
			 */
			VelModifier * instance() const { return new ScaleVelModifier(); }	
			
			/*!
			 *	@brief		Given a pointer to an VelModifier instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this VelModifier's type.
			 *	(i.e. VelModFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of VelModFactory introduce *new* VelComponent parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		vm			A pointer to the velocity modifier whose attributes are to be set.
			 *	@param		node		The XML node containing the velocity modifier attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the velocity modifier references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( VelModifier * vm, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			// TODO: This should support a numerical distribution
			/*!
			 *	@brief		The identifier for the "scale" float attribute.
			 */
			size_t	_scaleID;
		};
		
	}	// namespace BFSM
}	// namespace Menge
#endif	// __VEL_MOD_SCALE_H
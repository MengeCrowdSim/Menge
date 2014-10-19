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
 *	@file		VelModHeightField.h
 *	@brief		definition of a VelocityModifier to push agents along the gradient of a heightField
 */
#ifndef __VELMOD_HEIGHT_FIELD_H__
#define	__VELMOD_HEIGHT_FIELD_H__

// Resources
#include "HeightField.h"

// Menge Base
#include "VelocityModifiers/VelModifier.h"
#include "VelocityModifiers/VelModifierFactory.h"

//forward declarations
class TiXmlElement;

namespace Menge {
	namespace Agents {
		class BaseAgent;
	}
}

using namespace Menge;

namespace Terrain {

	//forward declare Factory
	class HeightFieldModifierFactory;

	/*!
	 *	@brief		Modify the preferred velocity based on a height field.
	 *
	 *	Agents preferred velocities can be sped up, slowed, and turned based on their attempt
	 *	to traverse a heightfield.  The gradient of the height field is used to determine the
	 *  new preferred velocity.  
	 *
	 *	Direction is determined by a re-normalized, weighted sum of the gradient with the input
	 *	preferred direction.
	 *
	 *	The resultant speed is a scaled factor of the input speed with the steepness of the gradient.
	 *	Uphill and downhill speed changes are controlled separately -- gradients which lie perpindicular
	 *	to the input preferred direction will cause a direction change, but no speed change.
	 *
	 *	The output preferred velocity becomes a single velocity (instead of an arc).  
	 *	If the input preferred velocity spanned an arc, it will be collapsed to a single direction.
	 */
	class EXPORT_API HeightFieldModifier : public BFSM::VelModifier {
	public:
		
		/*!
		 *	@brief		Default constructor
		 */
		HeightFieldModifier();

		/*!
		 *	@brief		Constructor
		 *
		 *	@param		hfPtr		A pointer to a height-field resource.
		 */
		HeightFieldModifier( HeightFieldPtr hfPtr );

		/*!
		 *	@brief		Sets the height field for this velocity modifier object to use.
		 *
		 *	@param		hfPtr		A managed pointer to the height field.
		 */
		void setHeightField( HeightFieldPtr hfPtr ) { _field = hfPtr; }

		/*!
		 *
		 *	@brief		Copy method for this velocity modifier
		 *
		 */
		BFSM::VelModifier* copy() const;

		/*!
		 *	@brief		adapt preferred velocity by pushing it away from the heightfield gradient
		 *
		 *	@param		agent		The agent for which to modify preferred vel
		 *	@param		pVel		The agent's current preferred velocity (having potentially been modified)
		 *
		 */
		void adaptPrefVelocity(const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel );

		/*
		 * identify the factory helper 
		 */
		friend class HeightFieldModifierFactory;

	protected:

	    /*!
		 *	@brief		The height field used to query elevation and gradient
		 */
		HeightFieldPtr	_field;
		
		/*!
		 *	@brief		The weighting factor for how the gradient effects direction.
		 */
		float	_turnWeight;

		/*!
		 *	@brief		The uphill scale factor.
		 */
		float	_upHillScale;

		/*!
		 *	@brief		The downhill scale factor.
		 */
		float _downHillScale;

	};


	//////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Factory for the HeightFieldModifier.
	 */
	class EXPORT_API HeightFieldModifierFactory : public BFSM::VelModFactory {
	public:
		/*!
		 *
		 *		@brief		Constructor.
		 *
		 */
		HeightFieldModifierFactory();

		/*!
		 *	@brief		The name of the modifier
		 *
		 *	The modifier's name must be unique among all registered modifier.
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the unique modifier name.
		 */
		virtual const char * name() const { return "height_field"; }

		/*!
		 *	@brief		A description of the modifier
		 *
		 *	Each modifier factory must override this function.
		 *
		 *	@returns	A string containing the modifier description.
		 */
		virtual const char * description() const {
			return "Pushes the agent's preferred velocity away from the gradient of a height field";
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
		BFSM::VelModifier * instance() const { return new HeightFieldModifier(); }	
		
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

		/*!
		 *	@brief		The identifier for the "dir_weight" float attribute.
		 */
		size_t	_turnID;

		/*!
		 *	@brief		The identifier for the "up_hill_scale" float attribute.
		 */
		size_t	_uphillID;

		/*!
		 *	@brief		The identifier for the "down_hill_scale" float attribute.
		 */
		size_t	_downhillID;




	};
};
#endif

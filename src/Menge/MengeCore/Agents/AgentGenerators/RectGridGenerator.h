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
 *	@file		RectGridGenerator.h
 *	@brief		An agent generator which creates a set of agents based on the definition
 *				of a rectangular lattice, with an agent at each point.
 */

#ifndef __RECT_GRID_GENERATOR_H__
#define __RECT_GRID_GENERATOR_H__

#include "mengeCommon.h"
#include "AgentGenerators/AgentGenerator.h"
#include "AgentGenerators/AgentGeneratorFactory.h"
#include <vector>

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		Definition of an agent generator class which produces agents based on
		 *				the positions of intersections on a lattice--one agent per intersection.
		 */
		class MENGE_API RectGridGenerator : public AgentGenerator {
		public:
			/*!
			 *	@brief		Constructor
			 */
			RectGridGenerator();

			/*!
			 *	@brief		Reports the number of agents created.
			 *
			 *	@returns	The number of agents this generator creates.
			 */
			virtual size_t agentCount() { return _xCount * _yCount; }

			/*!
			 *	@brief		Get the position of the ith agent.
			 *
			 *	@param		i		The index of the requested agent.
			 *	@returns	The 2D position of the agent based on the generator's parameters.
			 *	@throws		AgentGeneratorException if the index, i, is invalid.
			 */
			virtual Vector2 agentPos( size_t i );
			
			/*!
			 *	@brief		Sets the anchor position.
			 *
			 *	@param		p		The anchor position.
			 */
			void setAnchor( const Vector2 & p ) { _anchor.set( p ); }

			/*!
			 *	@brief		Sets the offset value.
			 *
			 *	@param		o		The offset value.
			 */
			void setOffset( const Vector2 & o ) { _offset.set( o ); }

			/*!
			 *	@brief		Sets the number of agents in the local x-direction.
			 *
			 *	@param		count		The count of agents.
			 */
			void setXCount( size_t count ) { _xCount = count; }

			/*!
			 *	@brief		Sets the number of agents in the local y-direction.
			 *
			 *	@param		count		The count of agents.
			 */
			void setYCount( size_t count ) { _yCount = count; }

			/*!
			 *	@brief		Sets the number of agents in the local x- and y-directions.
			 *
			 *	@param		xCount		The count of agents in the x-direction.
			 *	@param		yCount		The count of agents in the y-direction.
			 */
			void setAgentCounts( size_t xCount, size_t yCount ) { _xCount = xCount; _yCount = yCount; }

			/*!
			 *	@brief		Sets the lattice rotation.
			 *
			 *	@param		angle		The rotation angle (in degrees).
			 */
			void setRotationDeg( float angle );

		protected:
			/*!
			 *	@brief		The anchor point of the lattice.  One agent will be positioned
			 *				at this world coordainte.
			 */
			Vector2 _anchor;

			/*!
			 *	@brief		The offset from one agent to the next agent (along the local
			 *				x- and y-axes, respectively.
			 */
			Vector2 _offset;

			/*!
			 *	@brief		The number of agents along the local x-axis.
			 */
			size_t	_xCount;

			/*!
			 *	@brief		The number of agents along the local y-axis.
			 */
			size_t	_yCount;

			/*!
			 *	@brief		The cosine of the amount the lattice is rotated around its anchor point.
			 *				positive rotation values are counter-clockwise rotation.
			 */
			float _cosRot;

			/*!
			 *	@brief		The sine of the amount the lattice is rotated around its anchor point.
			 *				positive rotation values are counter-clockwise rotation.
			 */
			float _sinRot;

		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for RectGridGenerator
		 */
		class MENGE_API RectGridGeneratorFactory : public AgentGeneratorFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			RectGridGeneratorFactory();

			/*!
			 *	@brief		The name of the generator type.
			 *
			 *	The generator's name must be unique among all registered agent generator components.  
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the unique elevation name.
			 */
			virtual const char * name() const { return "rect_grid"; }

			/*!
			 *	@brief		A description of the agent generator.
			 *
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the agent generator description.
			 */
			virtual const char * description() const {
				return "Agent generation is done via the specification of a rectangular grid.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's agent generator implementation.
			 *
			 *	All AgentGeneratorFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding generator type.  The various field values
			 *	of the instance will be set in a subsequent call to AgentGeneratorFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated AgentGenerator class.
			 */
			AgentGenerator * instance() const { return new RectGridGenerator(); }

			/*!
			 *	@brief		Given a pointer to an AgentGenerator instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this AgentGenerator's type.
			 *	(i.e. AgentGenerator::thisFactory has already been called and returned true.)
			 *	If sub-classes of AgentGeneratorFactory introduce *new* AgentGenerator parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		gen			A pointer to the agent generator whose attributes are to be set.
			 *	@param		node		The XML node containing the agent generator attributes.
			 *	@param		specFldr	The path to the specification file.  If the AgentGenerator references
			 *							resources in the file system, it should be defined relative
			 *							to the specification file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( AgentGenerator * gen, TiXmlElement * node, const std::string & specFldr ) const;

			/*!
			 *	@brief		The identifier for the "anchor_x" float parameter.
			 */
			size_t _anchorXID;

			/*!
			 *	@brief		The identifier for the "anchor_y" float parameter.
			 */
			size_t _anchorYID;

			/*!
			 *	@brief		The identifier for the "offset_x" float parameter.
			 */
			size_t _offsetXID;

			/*!
			 *	@brief		The identifier for the "offset_y" float parameter.
			 */
			size_t _offsetYID;

			/*!
			 *	@brief		The identifier for the "count_x" size_t parameter.
			 */
			size_t _xCountID;

			/*!
			 *	@brief		The identifier for the "count_y" size_t parameter.
			 */
			size_t _yCountID;

			/*!
			 *	@brief		The identifier for the "rotation" float parameter.
			 */
			size_t _rotID;
		};
	}	// namespace Agents
}	// namespace Menge

#endif	// __RECT_GRID_GENERATOR_H__
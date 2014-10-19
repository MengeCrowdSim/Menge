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
 *	@file		VelCompConst.h
 *	@brief		Provides the definitions of some simple velocity components
 *				based on constant velocities.
 */

#ifndef __VEL_COMP_CONST_H__
#define	__VEL_COMP_CONST_H__

#include "CoreConfig.h"
#include "VelocityComponents/VelComponent.h"
#include "VelocityComponents/VelComponentFactory.h"
#include "VelocityComponents/VelCompContext.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A velocity component that always returns a constant velocity.
		 */
		class MENGE_API ConstVelComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ConstVelComponent();

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vel			The preferred velocity to return.
			 */
			ConstVelComponent( const Vector2 & vel );

			/*!
			 *	@brief		Sets the constant velocity.
			 *
			 *	@param		velocity		The velocity for this velocity component.
			 */
			void setVelocity( const Vector2 & velocity );

			/*!
			 *	@brief		Computes and sets the agent's preferred velocity.
			 *
			 *	The velocity component directly sets preferred velocity values in the
			 *	the provided preferred velocity instance.  See Agents::PrefVelocity for details.
			 *	Rather than setting the agent's preferred velocity value directly, a reference
			 *	to a preferred velocity instance is passed in to make the use more general.
			 *	This allows the computation of the preferred velocity for the agent, without
			 *	necessarily making changes to it.
			 *
			 *	@param		agent		The agent for which a preferred velocity is computed.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Returns the constant velocity.
			 *
			 *	@returns	The velocity computed by this constant velocity component.
			 */
			Vector2 getConstVelocity() const { return _dir * _speed; }

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();

			friend class ConstVCFactory;
		protected:
			/*!
			 *	@brief		The direction of the constant preferred velocity.
			 */
			Vector2 _dir;

			/*!
			 *	@brief		The speed of the constant preferred velocity.
			 */
			float _speed;
		};


		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the ConstVelComponent.
		 */
		class MENGE_API ConstVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vc			A pointer to the underlying fsm velocity component.
			 *							The context will *not* delete the velocity component.
			 */
			ConstVCContext( ConstVelComponent * vc );

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity component information.
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
			 *	@param		goal		The agent's goal (although this may be ignored).
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, const Goal * goal );

		protected:
			/*!
			 *	@brief		The underlying finite state machine velocity component.
			 */
			ConstVelComponent * _vc;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the ConstVelComponent.
		 */
		class MENGE_API ConstVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ConstVCFactory();

			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "const"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is always a constant value (direction and magnitude).";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field values
			 *	of the instance will be set in a subsequent call to VelCompFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new ConstVelComponent(); }	
			
			/*!
			 *	@brief		Given a pointer to an VelComponent instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this VelComponent's type.
			 *	(i.e. VelCompFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of VelCompFactory introduce *new* VelComponent parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		vc			A pointer to the velocity component whose attributes are to be set.
			 *	@param		node		The XML node containing the velocity component attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the velocity component references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "x" float attribute.
			 */
			size_t	_xID;

			/*!
			 *	@brief		The identifier for the "y" float attribute.
			 */
			size_t	_yID;
		};

		/////////////////////////////////////////////////////////////////////

		// forward declaration
		class ConstDirVCContext;

		/*!
		 *	@brief		A velocity component that always returns a constant direction but
		 *				leaves the preferred speed unchanged.
		 */
		class MENGE_API ConstVelDirComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ConstVelDirComponent();

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vel			The preferred velocity direction to return (vel is *not*
			 *							assumed to be unit-length).
			 */
			ConstVelDirComponent( const Vector2 & vel );

			/*!
			 *	@brief		Computes and sets the agent's preferred velocity.
			 *
			 *	The velocity component directly sets preferred velocity values in the
			 *	the provided preferred velocity instance.  See Agents::PrefVelocity for details.
			 *	Rather than setting the agent's preferred velocity value directly, a reference
			 *	to a preferred velocity instance is passed in to make the use more general.
			 *	This allows the computation of the preferred velocity for the agent, without
			 *	necessarily making changes to it.
			 *
			 *	@param		agent		The agent for which a preferred velocity is computed.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Sets the direction of the velocity component.
			 *
			 *	@param		dir			The direction the velocity should provide.
			 *							The input need not be normalized, as tha will be done
			 *							by this function.
			 */
			void setDirection( const Vector2 & dir );

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();

			friend class ConstDirVCContext;
		protected:
			/*!
			 *	@brief		The direction of the constant preferred velocity.
			 */
			Vector2 _dir;
		};


		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the ConstVelDirComponent.
		 */
		class MENGE_API ConstDirVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vc			A pointer to the underlying fsm velocity component.
			 *							The context will *not* delete the velocity component.
			 */
			ConstDirVCContext( ConstVelDirComponent * vc );

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity component information.
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
			 *	@param		goal		The agent's goal (although this may be ignored).
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, const Goal * goal );

		protected:
			/*!
			 *	@brief		The underlying finite state machine velocity component.
			 */
			ConstVelDirComponent * _vc;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the ConstVelDirComponent.
		 */
		class MENGE_API ConstDirVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ConstDirVCFactory();

			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "const_dir"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is always in a fixed direction but uses the agent's preferred speed.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field values
			 *	of the instance will be set in a subsequent call to VelCompFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new ConstVelDirComponent(); }	
			
			/*!
			 *	@brief		Given a pointer to an VelComponent instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this VelComponent's type.
			 *	(i.e. VelCompFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of VelCompFactory introduce *new* VelComponent parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		vc			A pointer to the velocity component whose attributes are to be set.
			 *	@param		node		The XML node containing the velocity component attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the velocity component references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "x" float attribute.
			 */
			size_t	_xID;

			/*!
			 *	@brief		The identifier for the "y" float attribute.
			 */
			size_t	_yID;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A velocity component that always returns the zero velocity.
		 */
		class MENGE_API ZeroVelComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ZeroVelComponent();

			/*!
			 *	@brief		Computes and sets the agent's preferred velocity.
			 *
			 *	The velocity component directly sets preferred velocity values in the
			 *	the provided preferred velocity instance.  See Agents::PrefVelocity for details.
			 *	Rather than setting the agent's preferred velocity value directly, a reference
			 *	to a preferred velocity instance is passed in to make the use more general.
			 *	This allows the computation of the preferred velocity for the agent, without
			 *	necessarily making changes to it.
			 *
			 *	@param		agent		The agent for which a preferred velocity is computed.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();

		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the ZeroVelComponent.
		 */
		class MENGE_API ZeroVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ZeroVCContext();

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity component information.
			 *
			 *	@param		indent		An optional string representing indentation to be
			 *							applied to the text.  It is prefixed at the start
			 *							of each line.
			 *	@returns	The string for printing on the UI layer.
			 */
			virtual std::string getUIText( const std::string & indent="" ) const;

		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the ZeroVelComponent.
		 */
		class MENGE_API ZeroVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "zero"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is always the zero velocity.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field values
			 *	of the instance will be set in a subsequent call to VelCompFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new ZeroVelComponent(); }	
		};

		//////////////////////////////////////////////////////////////////////////////
		
	}	// namespace BFSM

}	// namespace Menge

#endif	// __VEL_COMP_CONST_H__
/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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
 *	@file		PedVODBEntry.h
 *	@brief		The simulator database entry for the PedVO pedestrian model.
 */

#ifndef __PEDVO_DB_ENTRY_H__
#define	__PEDVO_DB_ENTRY_H__

#include "MengeCore/Runtime/SimulatorDBEntry.h"
#include "MengeCore/PedVO/PedVO.h"

namespace PedVO {
	/*!
	 *	@brief		The simulator database entry for the PedVO simulator.
	 */
	class DBEntry : public Menge::SimulatorDBEntry {
	public:
		/*!
		 *	@brief	Gives a brief description of the simulator.
		 *
		 *	@returns	A brief description of the simulator and pedestrian
		 *				model.
		 */
		virtual ::std::string briefDescription() const;

		/*!
		 *	@brief	Gives a long description of the simulator.
		 *
		 *	@returns	A long description of the simulator and pedestrian
		 *				model.
		 */
		virtual ::std::string longDescription() const;

		/*!
		 *	@brief	Gives a label to apply to the interactive viewer.
		 *
		 *	@returns	The name for display on the interactive viewer.
		 */
		virtual ::std::string viewerName() const;

		/*!
		 *	@brief		Gives a unique name to be used as a command-line parameter.
		 *
		 *	This name MUST satisfy two constraints:
		 *		- It must contain no spaces.
		 *		- It must be unique from that used by all other simulators.
		 *
		 *	@returns	A single string (with no spaces) that can be used as
		 *				a command line parameter to uniquely identify this model.
		 */
		virtual ::std::string commandLineName() const { return "pedvo"; }

		/*!
		 *	@brief		Returns a pointer to this model's Simulator instance.
		 *
		 *	This must be overridden by a derived class
		 *
		 *	@returns	A newly instantiated simulator instance of a type appropriate
		 *				for this database entry.
		 */
		virtual Menge::Agents::SimulatorInterface * getNewSimulator();

		/*!
		 *	@brief		Provides an AgentInitializer appropriate to this simulator class.
		 *
		 *	@returns	A pointer to an agent initializer.  The caller is responsible for
		 *				freeing up the memory.
		 */
		virtual Menge::Agents::AgentInitializer * getAgentInitalizer() const;
#if 0
	protected:
		/*!
		 *	@brief		Returns a pointer to an agent context appropriate to
		 *				the corresponding simulator.
		 *
		 *	If the provided system is not, in fact, a pointer to a SimSystem for the
		 *	appropriate simulator type, this function will report failure.  Furthermore,
		 *	the default implementation is to return a BaseAgentContext.  If the
		 *	simulator comes with a novel context, this function should be overridden
		 *	in the derived SimulatorDBEntry.
		 *
		 *	@param		simSystem	The system which tracks the agents.  This should be
		 *							the same system which was returned by a call to 
		 *							SimulatorDBEntry::getSimulatorSystem.
		 *	@returns	A pointer to the appropriate agent context.  If the system is of
		 *				the wrong type (or if there is any other problem), NULL is returned.
		 */
		virtual Menge::BaseAgentContext * contextFromSystem( Menge::SimSystem * simSystem );
#endif
	};
}	// namespace PedVO

#endif // __PEDVO_DB_ENTRY_H__

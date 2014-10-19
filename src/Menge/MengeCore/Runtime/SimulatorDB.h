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
 *	@file	SimulatorDB.h
 *	@brief	Central database for querying available pedestrian models.
 *
 *	The SimulatorDB provides the mechanism by which new simulation models
 *	can easily be registered and their run-time properties queried by the
 *	main program.
 */

#ifndef __SIMULATOR_DB_H__
#define	__SIMULATOR_DB_H__

#include "SimulatorDBEntry.h"
#include "CoreConfig.h"
#include "MengeException.h"
#include <list>
#include <algorithm>
#include <iostream>
#ifndef _WIN32
#include <typeinfo>
#endif

namespace Menge {

	//////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Exception for SimulatorDB issues.
	 */
	class MENGE_API SimDBException : public virtual MengeException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		SimDBException() : MengeException() {}		

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		SimDBException( const std::string & s ): MengeException(s) {}
	};

	/*!
	 *	@brief		The fatal SimulatorDB exception.
	 */
	class MENGE_API SimDBFatalException : public SimDBException, public MengeFatalException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		SimDBFatalException() : MengeException(), SimDBException(), MengeFatalException() {}

			/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		SimDBFatalException( const std::string & s ): MengeException(s), SimDBException(), MengeFatalException() {}
	};

	//////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		A list of database entries.
	 */
	typedef std::list< SimulatorDBEntry * >	EntryList;

	//////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The simulator database class.
	 *
	 *	The main executable program relies on the simulator database to
	 *	know what models are available for simulation.  Furthermore, it
	 *	uses the database to instantiate the appropriate types as well.
	 */
	class MENGE_API SimulatorDB {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		SimulatorDB();
		
		/*!
		 *	@brief		Destructor.
		 */
		~SimulatorDB(){}

		/*!
		 *	@brief		Reports the number of registered pedestrian models.
		 *
		 *	@returns	The number of pedestrian models.
		 */
		inline size_t modelCount() const { return _entries.size(); }

		/*!
		 *	@brief		Gets a formatted list of valid command-line
		 *				parameters for all registered simulators.
		 *
		 *	@returns	A formatted list of all registered simulators'
		 *				command-line parameter names, 
		 *				e.g., model1, model2, ..., and modelN.
		 */
		::std::string paramList() const;

		/*!
		 *	@brief		Formats all brief descriptions into a single
		 *				string.
		 *
		 *	@returns	A formatted string consisting of all model's and
		 *				their brief descriptions.
		 */
		::std::string briefDescriptions() const;

		/*!
		 *	@brief		Formats all long descriptions into a single
		 *				string.
		 *
		 *	@returns	A formatted string consisting of all model's and
		 *				their long descriptions.
		 */
		::std::string longDescriptions() const;

		/*!
		 *	@brief		Returns the database entry for the given command line parameter.
		 *
		 *	It returns the database entry with a command line parameter value that is the
		 *	same as the provided string.  The test is case <i>in</i>sensitive. 
		 *
		 *	@param		modelName		The command-line parameter for the desired pedestrian model.
		 *	@returns	A pointer to the summary entity.  If the name does not match
		 *				a registered model, NULL is returned
		 */
		SimulatorDBEntry * getDBEntry( const std::string & modelName );

		/*!
		 *	@brief		Registers a database entry.
		 *
		 *	The registration is only valid if the entry's commandLineName is unique
		 *	(see SimulatorDBEntry::commandLineName).  If the given entry
		 *	duplicates a previous entry, the pointer is deleted.
		 *
		 *	@param		entry		A pointer to the entry to register.
		 *	@returns	The same pointer (if valid, null if invalid).
		 */
		SimulatorDBEntry * registerEntry( SimulatorDBEntry * entry );

	private:
		/*!
		 *	@brief		Returns the static list of entries.
		 *
		 *	This is part of C++ voodoo to make static registration of pedestrian
		 *	models possible and "easy".  
		 */
		EntryList _entries;
	};
}	// namespace Menge
#endif	// __SIMULATOR_DB_H__

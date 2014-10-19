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

#include "SimulatorDB.h"
#include "SimulatorDBEntry.h"
#include "Logger.h"
#include <sstream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////////////
	//                     Implementation of SimulatorDB
	/////////////////////////////////////////////////////////////////////////////

	SimulatorDB::SimulatorDB() {
		// TODO: Add explicit initialization of STATIC pedestrian models
	}

	/////////////////////////////////////////////////////////////////////////////

	::std::string SimulatorDB::paramList() const {
		::std::string fmtList;
		size_t i = 0;
		const EntryList & eList = _entries;
		const size_t COUNT = eList.size();
		EntryList::const_iterator itr = eList.begin();
		for ( ; itr != eList.end(); ++itr, ++i ) {
			if ( i == 0 ) {
				// beginning of list
				fmtList = (*itr)->commandLineName();
			} else if ( i < COUNT - 1 ) {
				// middle of list
				fmtList += ", " + (*itr)->commandLineName();
			} else if ( i == COUNT - 1 ) {
				if ( COUNT > 2 ) {
					fmtList += ", ";
				}
				fmtList += "and " + (*itr)->commandLineName();
			}
		}
		return fmtList;
	}

	/////////////////////////////////////////////////////////////////////////////

	::std::string SimulatorDB::briefDescriptions() const {
		::std::stringstream ss;
		ss << "Available pedestrian models:\n";
		const EntryList & eList = _entries;
		EntryList::const_iterator itr = eList.begin();
		for ( ; itr != eList.end(); ++itr ) {
			ss << "\n\tParameter: " << (*itr)->commandLineName() << "\n";
			ss << "\t\t" << (*itr)->briefDescription();
		}
		return ss.str();
	}

	/////////////////////////////////////////////////////////////////////////////

	::std::string SimulatorDB::longDescriptions() const {
		::std::stringstream ss;
		ss << "Available pedestrian models:\n";
		const EntryList & eList = _entries;
		EntryList::const_iterator itr = eList.begin();
		for ( ; itr != eList.end(); ++itr ) {
			ss << "\n\t---------------------------------------------------------\n";
			ss << "\tParameter: " << (*itr)->commandLineName() << "\n";
			ss << "\t" << (*itr)->longDescription() << "\n";
		}
		return ss.str(); 
	}

	/////////////////////////////////////////////////////////////////////////////

	SimulatorDBEntry * SimulatorDB::getDBEntry( const std::string & modelName ) {
		std::string name( modelName );
		std::transform( name.begin(), name.end(), name.begin(), ::tolower );
		EntryList & eList = _entries;
		EntryList::const_iterator itr = eList.begin();
		for ( ; itr != eList.end(); ++itr ) {
			std::string testName = (*itr)->commandLineName();
			std::transform( testName.begin(), testName.end(), testName.begin(), ::tolower );
			if ( modelName == testName ) {
				return *itr;
			}
		}
		return 0x0;
	}

	/////////////////////////////////////////////////////////////////////////////

	SimulatorDBEntry * SimulatorDB::registerEntry( SimulatorDBEntry * entry ) {
		std::string entryName = entry->commandLineName();
		EntryList & eList = _entries;
		EntryList::const_iterator itr = eList.begin();
		for ( ; itr != eList.end(); ++itr ) {
			std::string name = (*itr)->commandLineName();
			std::transform( name.begin(), name.end(), name.begin(), ::tolower );
			if ( entryName == name ) {
				logger << Logger::ERR_MSG << "Failed to register a pedestrian model\n" \
											"\tIt's command-line parameter name (" << name << ") matches the name\n" \
											"\tof a previously registered pedestrian model.\n" \
											"\tThe database entry, " << typeid( entry ).name() << ", will not be registered.\n";
				delete entry;	// TODO: DBEntries should probably be "destroyed" and not deleted.
				return 0x0;
			}
		}
		eList.push_back( entry );
		return entry;
	}
}	 // namespace Menge
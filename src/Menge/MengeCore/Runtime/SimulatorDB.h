/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file  SimulatorDB.h
 @brief  Central database for querying available pedestrian models.

 The SimulatorDB provides the mechanism by which new simulation models can easily be registered and
 their run-time properties queried by the main program.
 */

#ifndef __SIMULATOR_DB_H__
#define __SIMULATOR_DB_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/Runtime/SimulatorDBEntry.h"

#include <algorithm>
#include <iostream>
#include <vector>
#ifndef _WIN32
#include <typeinfo>
#endif

namespace Menge {

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Exception for SimulatorDB issues.
 */
class MENGE_API SimDBException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  SimDBException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SimDBException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal SimulatorDB exception.
 */
class MENGE_API SimDBFatalException : public SimDBException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  SimDBFatalException() : MengeException(), SimDBException(), MengeFatalException() {}

  /*!
@brief    Constructor with message.

@param    s    The exception-specific message.
*/
  SimDBFatalException(const std::string& s)
      : MengeException(s), SimDBException(), MengeFatalException() {}
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    A list of database entries.
 */
typedef std::vector<SimulatorDBEntry*> EntryList;

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    The simulator database class.

 The main executable program relies on the simulator database to know what models are available for
 simulation. Furthermore, it uses the database to instantiate the appropriate types as well.
 */
class MENGE_API SimulatorDB {
 public:
  /*!
   @brief    Constructor.
   */
  SimulatorDB();

  /*!
   @brief    Destructor.
   */
  ~SimulatorDB() {}

  /*!
   @brief    Reports the number of registered pedestrian models.

   @returns  The number of pedestrian models.
   */
  inline size_t modelCount() const { return _entries.size(); }

  /*!
   @brief    Gets a formatted list of valid command-line parameters for all registered simulators.

   @returns  A formatted list of all registered simulators' command-line parameter names, e.g.,
            model1, model2, ..., and modelN.
   */
  ::std::string paramList() const;

  /*!
   @brief    Reports the name of the ith model. Throws SimDBException for an invalid index.
   */
  ::std::string name(int i) const;

  /*!
   @brief    Formats all brief descriptions into a single string.

   @returns  A formatted string consisting of all model's and their brief descriptions.
   */
  ::std::string briefDescriptions() const;

  /*!
   @brief   Reports the brief description of the ith model. Throws SimDBException for an invalid
            index.
   */
  ::std::string briefDescription(int i) const;

  /*!
   @brief    Formats all long descriptions into a single string.

   @returns  A formatted string consisting of all model's and their long descriptions.
   */
  ::std::string longDescriptions() const;

  /*!
   @brief   Reports the long description of the ith model. Throws SimDBException for an invalid
            index.
   */
  ::std::string longDescriptions(int i) const;

  /*!
   @brief    Returns the database entry for the given command line parameter.

   It returns the database entry with a command line parameter value that is the same as the
   provided string. The test is case <i>in</i>sensitive.

   @param    modelName    The command-line parameter for the desired pedestrian model.
   @returns  A pointer to the summary entity. If the name does not match a registered model, NULL
            is returned
   */
  SimulatorDBEntry* getDBEntry(const std::string& modelName);

  /*!
   @brief    Registers a database entry.

   The registration is only valid if the entry's commandLineName is unique (see
   SimulatorDBEntry::commandLineName). If the given entry duplicates a previous entry, the pointer
   is deleted.

   @param    entry    A pointer to the entry to register.
   @returns  The same pointer (if valid, null if invalid).
   */
  SimulatorDBEntry* registerEntry(SimulatorDBEntry* entry);

 private:
  /*!
   @brief    Returns the static list of entries.

   This is part of C++ voodoo to make static registration of pedestrian models possible and "easy".
   */
  EntryList _entries;
};
}  // namespace Menge
#endif  // __SIMULATOR_DB_H__

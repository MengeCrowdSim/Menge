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
 @file    Plugin.h
 @brief    The basic specification of a plug in
 */

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/SharedLibrary.h"
#include "MengeCore/Runtime/Logger.h"

#include <string>

namespace Menge {
namespace PluginEngine {
/*!
 @brief    The base plug-in class
 */
template <typename EngineType>
class Plugin {
 public:
  /*!
   @brief    Plugin function pointer for functions which return strings.
   */
  typedef const char* GetCharPtrFcn();

  /*!
   @brief    Declaration of registration function pointer type
   */
  typedef void RegisterPluginFcn(EngineType*);

  /*!
   @brief    Constructor.

   @param    filename    The filename of the plugin to instantiate.
   @throws    std::exception if there is a problem initializing the plugin.
   */
  Plugin(const std::string& filename)
      : _handle(0), _registerFcnAddr(0x0), _getNameFcnAddr(0x0), _getDescFcnAddr(0x0) {
    // This might throw a std::runtime_error which will immediately propagate upwards
    try {
      _handle = SharedLibrary::Load(filename);
    } catch (std::exception& e) {
      logger << Logger::ERR_MSG << e.what();
      throw;
    }

    try {
      _registerFcnAddr =
          SharedLibrary::GetFunctionPointer<RegisterPluginFcn>(_handle, getRegisterName());
      _getNameFcnAddr = SharedLibrary::GetFunctionPointer<GetCharPtrFcn>(_handle, "getName");
      _getDescFcnAddr = SharedLibrary::GetFunctionPointer<GetCharPtrFcn>(_handle, "getDescription");
    } catch (std::exception& e) {
      logger << Logger::ERR_MSG << e.what();
      SharedLibrary::Unload(_handle);
      throw;
    }
  }

  /*!
   @brief    Destructor.
   */
  ~Plugin() {
    if (_handle != 0) SharedLibrary::Unload(_handle);
  }

  /*!
   @brief    Reports the name of the registraiton function for this plugin type.
   */
  const char* getRegisterName() const {
    throw MengeFatalException("Failed to define plugin register function name.");
  }

  /*!
   @brief    Registers the plugin to the PluginEngine.

   @param    engine    The PluginEngine.
   */
  void registerPlugin(EngineType* engine) { _registerFcnAddr(engine); }

  /*!
   @brief    Returns the name of the plugin.

   @returns  The name of the plug-in.
   */
  const char* getName() { return _getNameFcnAddr(); }

  /*!
   @brief    Returns the description of the plugin.

   @returns  The description of the plug-in.
   */
  const char* getDescription() { return _getDescFcnAddr(); }

 private:
  /*!
   @brief    The shared library handle.
   */
  SharedLibrary::HandleType _handle;

  /*!
   @brief    A function pointer to the plugin registration function.

   This should get initialized in the constructor.
   */
  RegisterPluginFcn* _registerFcnAddr;

  /*!
   @brief    A function pointer to the function which returns the plugin name.
   */
  GetCharPtrFcn* _getNameFcnAddr;

  /*!
   @brief    A function pointer to the function which returns the plugin
   description.
   */
  GetCharPtrFcn* _getDescFcnAddr;
};
}  // namespace PluginEngine
}  // namespace Menge

#endif  // __PLUGIN_H__

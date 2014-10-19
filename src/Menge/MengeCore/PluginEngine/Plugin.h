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
 *	@file		Plugin.h
 *	@brief		The basic specification of a plug in
 */

#ifndef __PLUGIN_H__
#define	__PLUGIN_H__

#include <string>
#include "SharedLibrary.h"

namespace Menge {

	class PluginEngine;

	/*!
	 *	@brief		The base plug-in class
	 */
	class Plugin {
	public:
		/*!
		 *	@brief		Plugin function pointer for functions which return strings.
		 */
		typedef const char * GetCharPtrFcn();
	    
		/*!
		 *	@brief		Declaration of registration function pointer type
		 */
		typedef void RegisterPluginFcn( PluginEngine * );

		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		filename		The filename of the plugin to instantiate.
		 *	@throws		std::exception if there is a problem initializing the plugin.
		 */
		MENGE_API Plugin( const std::string & filename );

		/*!
		 *	@brief		Destructor.
		 */
		MENGE_API ~Plugin();
		
		/*!
		 *	@brief		Registers the plugin to the PluginEngine
		 *
		 *	@param		engine		The PluginEngine
		 */
		MENGE_API void registerPlugin( PluginEngine * engine ) {
			_registerFcnAddr( engine );
		}

		/*!
		 *	@brief		Returns the name of the plugin
		 *
		 *	@returns	The name of the plug-in.
		 */
		MENGE_API const char * getName() {
			return _getNameFcnAddr();
		}
		
		/*!
		 *	@brief		Returns the description of the plugin
		 *
		 *	@returns	The description of the plug-in.
		 */
		MENGE_API const char * getDescription() {
			return _getDescFcnAddr();
		}

	private:
		/*!
		 *	@brief		The shared library handle.
		 */
		SharedLibrary::HandleType		_handle;

		/*!
		 *	@brief		A function pointer to the plugin registration function.
		 *
		 *	This should get initialized in the constructor
		 */
		RegisterPluginFcn *	_registerFcnAddr;

		/*!
		 *	@brief		A function pointer to the function which returns the plugin name
		 */
		GetCharPtrFcn *	_getNameFcnAddr;

		/*!
		 *	@brief		A function pointer to the function which returns the plugin description
		 */
		GetCharPtrFcn *	_getDescFcnAddr;
	};

}	// namespace Menge

#endif	// __PLUGIN_H__
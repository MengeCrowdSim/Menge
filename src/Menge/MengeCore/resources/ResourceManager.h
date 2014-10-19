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
 *	@file		ResourceManager.h
 *	@brief		The basic class for all on-disk resources.
 */

#ifndef __RESOURCE_MANAGER_H__
#define	__RESOURCE_MANAGER_H__

#include <string>
#include <map>
#include "mengeCommon.h"

namespace Menge {

	// Forward declaration
	class Resource;

	/*!
	 *	@brief		Type declaration for a resource map - mapping file names to resource pointers.
	 */
	typedef std::map< std::string, Resource * > ResourceMap;

	/*!
	 *	@brief		Class to handle management of on-disk resources.
	 */
	class MENGE_API ResourceManager {
	public:
		/*!
		 *	@brief		Retrieve a resource from the manager.
		 *
		 *	@param		fileName	The name of the file associated with the resource.
		 *	@param		reader		Pointer to a function for parsing the given file and
		 *							producing a resource instance.
		 *	@param		suffix		The string to append to the file name.  This allows different
		 *							*types* of resources basedon on the same file resource to
		 *							be distinguished.  It is the burden of the programmer to
		 *							make sure each type provides a unique suffix, otherwise
		 *							problems will arise between suffix collisions.
		 *	@returns	A pointer to the reference, if it is loaded, NULL otherwise.
		 *				The caller is responsible for knowing what type of resource it
		 *				should be and test it using a dynamic-cast.
		 */
		static Resource * getResource( const std::string & fileName, Resource * (*reader)(const std::string & ), const std::string & suffix );

		/*!
		 *	@brief		Passes through the resources and removes all unreferenced resources.
		 */
		static void cleanup();

		/*!
		 *	@brief		Remove the given resource
		 *
		 *	@param		rsrc		A pointer to the resource to remove.
		 *	@returns	True if the pointer is successfully removed, false otherwise.
		 *				It can fail if the resource doesn't belong to the manager or
		 *				if the resources reference count is not zero.
		 */
		static bool removeResource( Resource * rsrc );

	protected:
		/*!
		 *	@brief		Mapping from resource name to resource.
		 */
		static ResourceMap	_resources;

	private:
		/*!
		 *	@brief		The string used to concatenate filenames with 
		 *				resource type suffixes.
		 */
		static const std::string CAT_SYMBOL;
	};
}	// namespace Menge
#endif	 // __RESOURCE_MANAGER_H__

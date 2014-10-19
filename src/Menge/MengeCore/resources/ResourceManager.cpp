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

#include "ResourceManager.h"
#include "Resource.h"
#include <iostream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of ResourceManager
	/////////////////////////////////////////////////////////////////////

	ResourceMap	ResourceManager::_resources;
	const std::string ResourceManager::CAT_SYMBOL("|");

	/////////////////////////////////////////////////////////////////////

	Resource * ResourceManager::getResource( const std::string & fileName, Resource * (*reader)(const std::string & ), const std::string & suffix ) {
		Resource * rsrc = 0x0;
		const std::string key = fileName + CAT_SYMBOL + suffix;
		ResourceMap::iterator itr = _resources.find( key );
		if ( itr != _resources.end() ) {
			rsrc = itr->second;
		} else {
			rsrc = reader( fileName );
			if ( rsrc == 0x0 ) {
				logger << Logger::ERR_MSG << "Error loading the resource from: " << fileName << "\n";
			}
			_resources[ key ] = rsrc;
		}
		
		return rsrc;
	}

	/////////////////////////////////////////////////////////////////////

	void ResourceManager::cleanup() {
		ResourceMap::iterator itr = _resources.begin();
		while ( itr != _resources.end() ) {
			Resource * rsrc = itr->second;
			if ( rsrc->isUnreferenced() ) {
				rsrc->destroy();
				ResourceMap::iterator next = itr;
				++next;
				_resources.erase( itr );
				itr = next;
			} else {
				++itr;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool ResourceManager::removeResource( Resource * rsrc ) {
		const std::string key = rsrc->_fileName + CAT_SYMBOL + rsrc->getLabel();
		ResourceMap::iterator itr = _resources.find( key );
		if ( itr == _resources.end() ) {
			logger << Logger::ERR_MSG << "Trying to remove a resource that the ResourceManager doesn't own: " << rsrc->_fileName << "\n";
			return false;
		}
		if ( ! rsrc->isUnreferenced() ) {
			logger << Logger::ERR_MSG << "Trying to remove a resource with a non-zero reference count: " << rsrc->_fileName << "\n";
			return false;
		}
		_resources.erase( itr );
		rsrc->destroy();
		return true;
	}
}	// namespace Menge
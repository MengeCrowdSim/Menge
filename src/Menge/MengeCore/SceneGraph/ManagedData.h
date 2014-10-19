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
 *	@file		ManagedData.h
 *	@brief		The interface for handling resources from a disk system.
 *
 *	ManagedData are heavy-weight on-disk resources.  They are managed in the
 *	sense that they are only read from the disk once and stored in memory as
 *	a single instance.  If multiple entities need the same on-disk resource,
 *	they share the underlying data in opaque containers.
 */

#ifndef __MANAGED_DATA_H__
#define __MANAGED_DATA_H__

#include "CoreConfig.h"
#include <map>
#include <string>

namespace Menge {

	/*!
	 *	@brief		The interface for managed data (essentially smart poitners).
	 */
	class MENGE_API ManagedData {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		ManagedData():_refCount(0){}

		/*!
		 *	@brief		Virtual destructor.
		 *
		 *	Sub-classes of ManagedData, upon their destruction, must remove themselves
		 *	from the resource set.  It does this by performing a call to
		 *	removeResource with appropriate template values and parameters.
		 *	See ImageData::~ImageData as an example.
		 */
		virtual ~ManagedData(){}

		/*!
		 *	@brief		Increment references to the managed data.
		 *
		 *	Any object that carries a pointer to managed data should call this function
		 *	when acquiring and storing a pointer to that data.
		 *
		 *	@returns	The total number of references.
		 */
		int incRef() { return ++_refCount; }

		/*!
		 *	@brief		Decrement references to the managed data.
		 *
		 *	Any object that carries a pointer to managed data should call this function
		 *	upon destruction (thereby removing its reference from the managed data's reference
		 *	count.
		 *
		 *	@returns	The number of remaining references.
		 */
		int decRef() { return --_refCount; }

		/*!
		 *	@brief		Reports if the data is referenced.
		 *
		 *	Any object that carries a pointer to managed data, after dereferencing itself
		 *	upon destruction, should determine if it is no longer referenced and delete
		 *	the object if this function returns true.  NOTE: this is not thread-safe.
		 *
		 *	@returns	True if the underlying managed data is no longer referenced, false
		 *				otherwise.
		 */
		bool isUnreferenced() const { return _refCount == 0; }

	protected:
		/*!
		 *	@brief		The number of data wrappers using this managed data.
		 */
		int	_refCount;
	};

	/*!
	 *	@brief		A wrapper for managed data - automatically handles referencing
	 *				and deletion of managed data.
	 *
	 *	Any class that uses managed data directly (i.e., a class derived from
	 *	ManagedData) should sub-class the ManagedData wrapper and implement its 
	 *	interface.  The wrapper will automatically take care of reference counts.
	 */
	template < class TData >
	class ManagedDataWrapper {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	All classes which must call this constructor in their own constructors.
		 *	See Image::Image for an example.
		 *
		 *	@param		data		A pointer to an instance of ManagedData.
		 */
		ManagedDataWrapper( TData * data ) {
			data->incRef();
			_data = data;
		}

		/*!
		 *	@brief		Technically, decrements the data's reference count
		 *				and garbage collects it when necessary.
		 *
		 *	Sub-classes of ManagedDataWrapper should call this in their destructor
		 *	after destorying all the rest of their memory.  
		 *	See Image::~Image for an example.
		 */
		void freeData() {
			if ( _data->decRef() == 0 ) {
				delete _data;
			}
		}

	protected:
		/*!
		 *	@brief		The managed data for this wrapper.
		 */
		TData *	_data;
	};

	// These templated functions serve as the basis for loading new instances of a resource (or 
	//	grabbing them from the cached database.)
	//
	// To use them the derived class needs to satisfy the following requirements:
	//	1. The class, Data,  must have a PUBLIC static member: std::map< std::string, TData * >
	//		This class is a map that maps filenames to pointers to the corresponding loaded
	//		data.  If the filename is not a key in this map, then the data has never been loaded
	//	2. You must provide a function: Data * readData( const std::string )
	//		This function constructs a new Data, reads the file and populates the data file,
	//		ADDS THE DATA FILE TO THE MAP, and returns the pointer to the data.
	//	3. The wrapper class, T, must instantiate using TData * as the only argument with the
	//		constructor.
	//
	// If I do this, then any final user can simply call "T * myT = loadT( fileName );"  And get
	//		a pointer to the data contained in the fileName which prevents duplicate loading and
	//		automatically tracks references so when all the T's that reference a particular TData
	//		are destroyed.

	/*!
	 *	@brief		Templated function for acquiring a managed data resource from its name.
	 *
	 *	@param		name		The name of the on-disk resource.
	 *	@param		map			The mapping from name to in-memory resource.
	 *	@returns	A pointer to the ManagedData instance if it is in memory, NULL otherwise.
	 */
	template< typename TData >
	TData * getResource( const std::string & name, std::map< std::string, TData * > & map ) {
		if ( map.find( name ) == map.end() ) {
			return 0x0;
		} else {
			return map[ name ];
		}
	}

	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Templated function for loading a managed data resource.
	 *
	 *	For each type of managed data and its wrapper, a function should exist that
	 *	simply calls this function, templated on the wrapper and managed data types
	 *	providing the name and the managed data reader function.  See Image for an example.
	 *
	 *	@param		fileName		The name of the on-disk resource.
	 *	@param		reader			A pointer to the resource reading function.
	 *	@returns	A pointer to the wrapper for the managed data.  NULL if the
	 *				resource couldn't be acquired.
	 */
	template< typename T, typename TData>
	T* loadManagedData( const std::string & fileName, TData * (*reader)(const std::string & ) ) {
		TData * data = getResource<TData>( fileName, TData::RESOURCES );
		if ( data == 0x0 ) {
			data = (*reader)( fileName );
			if ( data != 0x0 ) {
				TData::RESOURCES[ fileName ] = data;
			}
		}
		if ( data == 0x0 ) {
			return 0x0;
		}
		return new T( data );
	}

	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Tempalted function for removing a resource from the managed databse.
	 *
	 *	This *should* be called by the managed data instance in its destructor.  
	 *	The data is no longer valid and should need to be reloaded before re-use.
	 *
	 *	@param		data		A pointer to the managed data.
	 *	@param		map			The map of names to manage data instances of this type.
	 */
	template < typename TData >
	void removeResource( TData * data, std::map< std::string, TData * > & map ) {
		// remove self from the resources set
		typename std::map< std::string, TData *>::iterator itr = map.begin();
		for ( ; itr != map.end(); ++itr ) {
			if ( itr->second == data ) {
				map.erase( itr );
				break;
			}
		}
	}
}	// namespace Menge

#endif

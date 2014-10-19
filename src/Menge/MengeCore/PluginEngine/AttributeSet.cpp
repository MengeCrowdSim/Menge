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

#include "AttributeSet.h"
#include "Attribute.h"
#include "tinyxml.h"
#include <sstream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of AttributeSet
	/////////////////////////////////////////////////////////////////////

	AttributeSet::AttributeSet():_attrs() {
	}

	/////////////////////////////////////////////////////////////////////

	AttributeSet::~AttributeSet() {
		for ( size_t i = 0; i < _attrs.size(); ++i ) {
			delete _attrs[ i ];
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addBoolAttribute( const std::string & name, bool required, bool defValue ) {
		if ( isUniqueName( name ) ) {
			Attribute * attr = new BoolAttribute( name, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add a boolean attribute with the key value: \"" << name << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addIntAttribute( const std::string & name, bool required, int defValue ) {
		if ( isUniqueName( name ) ) {
			Attribute * attr = new IntAttribute( name, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add an int attribute with the key value: \"" << name << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addSizeTAttribute( const std::string & name, bool required, size_t defValue ) {
		if ( isUniqueName( name ) ) {
			Attribute * attr = new SizeTAttribute( name, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add a size_t attribute with the key value: \"" << name << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addFloatAttribute( const std::string & name, bool required, float defValue ) {
		if ( isUniqueName( name ) ) {
			Attribute * attr = new FloatAttribute( name, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add a float attribute with the key value: \"" << name << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addFloatDistAttribute( const std::string & prefix, bool required, float defValue, float scale ) {
		if ( isUniqueName( prefix ) ) {
			Attribute * attr = new FloatDistributionAttribute( prefix, required, defValue, scale );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add a float distribution attribute with the key value: \"" << prefix << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addVec2DDistAttribute( bool required, const Vector2 & defValue, float scale ) {
		// Confirm that this is the only Vec2D generator
		for ( size_t i = 0; i < _attrs.size(); ++i ) {
			if ( dynamic_cast< Vec2DDistributionAttribute * >( _attrs[i] ) != 0x0 ) {
				std::stringstream ss;
				ss << "Trying to add multiple Vector 2D distribution attributes to a factory.  This is not allowed.";
				throw AttributeDefinitionException( ss.str() );
			}
		}
		Attribute * attr = new Vec2DDistributionAttribute( required, defValue, scale );
		_attrs.push_back( attr );
		return _attrs.size() - 1;
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addIntDistAttribute( const std::string & prefix, bool required, int defValue ) {
		if ( isUniqueName( prefix ) ) {
			Attribute * attr = new IntDistributionAttribute( prefix, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add an int distribution attribute with the key value: \"" << prefix << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::addStringAttribute( const std::string & name, bool required, const std::string & defValue ) {
		if ( isUniqueName( name ) ) {
			Attribute * attr = new StringAttribute( name, required, defValue );
			_attrs.push_back( attr );
			return _attrs.size() - 1;
		} else {
			std::stringstream ss;
			ss << "Trying to add a string attribute with the key value: \"" << name << "\".  Previous attribute already using that name.";
			throw AttributeDefinitionException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	void AttributeSet::clear() {
		for ( size_t i = 0; i < _attrs.size(); ++i ) {
			_attrs[i]->clear();
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool AttributeSet::extract( TiXmlElement * node ) {
		bool valid = true;
		// Don't simply die at the first attribute error.  Find multiple errors
		//	and THEN throw an exception.
		for ( size_t i = 0; i < _attrs.size(); ++i ) {
			valid = valid && _attrs[i]->extract( node );
		}
		return valid;
	}

	/////////////////////////////////////////////////////////////////////

	bool AttributeSet::isUniqueName( const std::string & name ) {
		// This requires every name to be unique
		//	This includes distribution prefixes.  A distribution prefix CANNOT be
		//	the same as another attribute name.
		for ( size_t i = 0; i < _attrs.size(); ++i ) {
			if ( _attrs[i]->getName() == name ) return false;
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////////

	int AttributeSet::getInt( size_t propID ) {
		return _attrs[ propID ]->getInt();
	}

	/////////////////////////////////////////////////////////////////////

	size_t AttributeSet::getSizeT( size_t propID ) {
		return _attrs[ propID ]->getSizeT();
	}

	/////////////////////////////////////////////////////////////////////

	bool AttributeSet::getBool( size_t propID ) {
		return _attrs[ propID ]->getBool();
	}

	/////////////////////////////////////////////////////////////////////

	float AttributeSet::getFloat( size_t propID ) {
		return _attrs[ propID ]->getFloat();
	}

	/////////////////////////////////////////////////////////////////////

	std::string	AttributeSet::getString( size_t propID ) {
		return _attrs[ propID ]->getString();
	}

	/////////////////////////////////////////////////////////////////////

	FloatGenerator * AttributeSet::getFloatGenerator( size_t propID ) {
		return _attrs[ propID ]->getFloatGenerator();
	}

	/////////////////////////////////////////////////////////////////////

	Vec2DGenerator * AttributeSet::getVec2DGenerator( size_t propID ) {
		return _attrs[ propID ]->getVec2DGenerator();
	}

	/////////////////////////////////////////////////////////////////////

	IntGenerator * AttributeSet::getIntGenerator( size_t propID ) {
		return _attrs[ propID ]->getIntGenerator();
	}

	/////////////////////////////////////////////////////////////////////

}	// namespace Menge
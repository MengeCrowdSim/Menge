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

#include "Attribute.h"
#include "Logger.h"
#include "Utils.h"
#include "os.h"
#include "Math/RandGenerator.h"
#include "tinyxml.h"
#include <sstream>
#include <cassert>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of Attribute
	/////////////////////////////////////////////////////////////////////

	void Attribute::clear() {
		_valid = false;
		if ( !_required ) {
			_valid = true;
			setDefault();
		}
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of StringAttribute
	/////////////////////////////////////////////////////////////////////

	bool StringAttribute::extract( TiXmlElement * node ) {
		const char * val = node->Attribute( _name.c_str() );
		if ( val ) {
			_value = val;
		} else if ( _required ) {
			// Didn't get a value and it is required
			logger << Logger::ERR_MSG << "Expecting a missing string attribute (" << _name << ") on line " << node->Row() << ".";
			return false;
		} else {
			logger << Logger::WARN_MSG << "Expecting a missing string attribute (" << _name << ") on line " << node->Row() << ".  Using default value: " << _default << ".";
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of IntAttribute
	/////////////////////////////////////////////////////////////////////

	bool IntAttribute::extract( TiXmlElement * node ) {
		int i;
		if ( node->Attribute( _name, &i ) ) {
			_value = i;
		} else if ( _required ) {
			logger << Logger::ERR_MSG << "Expecting a missing int attribute (" << _name << ") on line " << node->Row() << ".";
			return false;
		} else {
			logger << Logger::WARN_MSG << "Expecting a missing int attribute (" << _name << ") on line " << node->Row() << ".  Using default value: " << _default << ".";
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of FloatAttribute
	/////////////////////////////////////////////////////////////////////

	bool FloatAttribute::extract( TiXmlElement * node ) {
		double d;
		if ( node->Attribute( _name, &d ) ) {
			_value = static_cast< float >( d );
		} else if ( _required ) {
			logger << Logger::ERR_MSG << "Expecting a missing float attribute (" << _name << ") on line " << node->Row() << ".";
			return false;
		} else {
			logger << Logger::WARN_MSG << "Expecting a missing float attribute (" << _name << ") on line " << node->Row() << ".  Using default value: " << _default << ".";
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////
	//					Implementation of BoolAttribute
	/////////////////////////////////////////////////////////////////////

	bool BoolAttribute::extract( TiXmlElement * node ) {
		int i;
		if ( node->Attribute( _name, &i ) ) {
			_value = i != 0;
		} else if ( _required ) {
			logger << Logger::ERR_MSG << "Expecting a missing boolean attribute (" << _name << ") on line " << node->Row() << ".";
			return false;
		} else {
			logger << Logger::WARN_MSG << "Expecting a missing boolean attribute (" << _name << ") on line " << node->Row() << ".  Using default value: " << _default << ".";
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////
	//					Implementation of SizeTAttribute
	/////////////////////////////////////////////////////////////////////

	bool SizeTAttribute::extract( TiXmlElement * node ) {
		int i;
		if ( node->Attribute( _name, &i ) ) {
			if ( i >= 0 ) {
				_value = static_cast< size_t >( i );
			} else {
				logger << Logger::ERR_MSG << "Expecting a size_t attribute (" << _name << ") on line " << node->Row() << ". Found a negative number!";
				return false;
			}
		} else if ( _required ) {
			logger << Logger::ERR_MSG << "Expecting a missing size_t attribute (" << _name << ") on line " << node->Row() << ".";
			return false;
		} else {
			logger << Logger::WARN_MSG << "Expecting a missing size_t attribute (" << _name << ") on line " << node->Row() << ".  Using default value: " << _default << ".";
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////
	//					Implementation of FloatDistributionAttribute
	/////////////////////////////////////////////////////////////////////

	FloatDistributionAttribute::~FloatDistributionAttribute() {
		if ( _generator ) {
			delete _generator;
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool FloatDistributionAttribute::extract( TiXmlElement * node ) {
		if ( _generator ) delete _generator;
		_generator = createFloatGenerator( node, _scale, _name );
		if ( _generator == 0x0 ) {
			if ( _required ) {
				logger << Logger::ERR_MSG << "Expecting a missing float distribution specification on line " << node->Row() << ".";
				return false;
			} else {
				logger << Logger::WARN_MSG << "Expecting a missing float distribution specification on line " << node->Row() << ". Using a const distribution with value: " << _default << ".";
				_generator = new ConstFloatGenerator( _default );
			}
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////
	//					Implementation of Vec2DDistributionAttribute
	/////////////////////////////////////////////////////////////////////

	Vec2DDistributionAttribute::~Vec2DDistributionAttribute() {
		if ( _generator ) {
			delete _generator;
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool Vec2DDistributionAttribute::extract( TiXmlElement * node ) {
		if ( _generator ) delete _generator;
		_generator = create2DGenerator( node, _scale );
		if ( _generator == 0x0 ) {
			if ( _required ) {
				logger << Logger::ERR_MSG << "Expecting a missing 2D vector distribution specification on line " << node->Row() << ".";
				return false;
			} else {
				logger << Logger::WARN_MSG << "Expecting a missing 2D vector distribution specification on line " << node->Row() << ". Using a const distribution with value: " << _default << ".";
				_generator = new Const2DGenerator( _default );
			}
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////
	//					Implementation of IntDistributionAttribute
	/////////////////////////////////////////////////////////////////////

	IntDistributionAttribute::~IntDistributionAttribute() {
		if ( _generator ) {
			delete _generator;
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool IntDistributionAttribute::extract( TiXmlElement * node ) {
		if ( _generator ) delete _generator;
		_generator = createIntGenerator( node, _name );
		if ( _generator == 0x0 ) {
			if ( _required ) {
				logger << Logger::ERR_MSG << "Expecting a missing int distribution specification on line " << node->Row() << ".";
				return false;
			} else {
				logger << Logger::WARN_MSG << "Expecting a missing int distribution specification on line " << node->Row() << ". Using a const distribution with value: " << _default << ".";
				_generator = new ConstIntGenerator( _default );
			}
		}
		return true;
	}

}	// namespace Menge
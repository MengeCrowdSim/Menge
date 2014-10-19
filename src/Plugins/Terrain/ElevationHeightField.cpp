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

#include "ElevationHeightField.h"
#include "BaseAgent.h"
#include "tinyxml.h"
#include "os.h"

namespace Terrain {

	////////////////////////////////////////////////////////////////
	//					Implementation of HeightFieldElevation
	////////////////////////////////////////////////////////////////

	HeightFieldElevation::HeightFieldElevation(): Agents::Elevation(), _field(0x0) {
	}

	////////////////////////////////////////////////////////////////

	float HeightFieldElevation::getElevation( const Vector2 & point ) const {
		return _field->getHeightAt( point._x, point._y );
	}

	////////////////////////////////////////////////////////////////

	float HeightFieldElevation::getElevation( const Agents::BaseAgent * agent ) const {
		return _field->getHeightAt( agent->_pos._x, agent->_pos._y );
	}

	////////////////////////////////////////////////////////////////

	Vector2 HeightFieldElevation::getGradient( const Agents::BaseAgent * agent ) const {
		Vector3 norm = _field->getNormalAt( agent->_pos._x, agent->_pos._y );
		return Vector2( norm._x, norm._z );
	}

	////////////////////////////////////////////////////////////////

	Vector2 HeightFieldElevation::getGradient( const Vector2 & point ) const {
		Vector3 norm = _field->getNormalAt( point._x, point._y );
		return Vector2( norm._x, norm._z );
	}

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of HeightFieldElevationFactory
	/////////////////////////////////////////////////////////////////////

	HeightFieldElevationFactory::HeightFieldElevationFactory() : Agents::ElevationFactory() {
		_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
	}

	/////////////////////////////////////////////////////////////////////

	bool HeightFieldElevationFactory::setFromXML( Agents::Elevation * e, TiXmlElement * node, const std::string & specFldr ) const {
		HeightFieldElevation * hfe = dynamic_cast< HeightFieldElevation * >( e );
		assert( hfe != 0x0 && "Trying to set attributes of a height field elevation component on an incompatible object" );

		if ( ! ElevationFactory::setFromXML( hfe, node, specFldr ) ) return false;

		// get the file name
		std::string fName;
		std::string path = os::path::join( 2, specFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
		os::path::absPath( path, fName );
		// nav mesh
		HeightFieldPtr hfPtr;
		try {
			hfPtr = loadHeightField( fName );
		} catch ( ResourceException ) {
			logger << Logger::ERR_MSG << "Couldn't instantiate the height field elevation referenced on line " << node->Row() << ".";
			return false;
		}
		hfe->setHeightField( hfPtr );

		return true;
	}
	
}	// namespace Terrain

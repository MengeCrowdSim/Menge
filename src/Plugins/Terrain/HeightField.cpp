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

#include "HeightField.h"
#include "image.h"
#include "tinyxml.h"
#include "os.h"
#include <cmath>

#ifdef _WIN32
#define MIN min
#else
#define MIN std::min
#endif

namespace Terrain {

	///////////////////////////////////////////////////////////////////////////
	//				IMPLEMENTATION FOR HeightField
	///////////////////////////////////////////////////////////////////////////

	const std::string HeightField::LABEL("height_field");

	///////////////////////////////////////////////////////////////////////////

	HeightField::HeightField( const std::string & fileName ) : Resource(fileName), _cellSize(1.f), _W(0), _H(0), _heightMap(0x0), _normalMap(0x0), _xpos(0.f), _ypos(0.f) {
	}

	///////////////////////////////////////////////////////////////////////////

	HeightField::~HeightField()
	{
		for (int i=0; i<_W; i++)
		{
			delete[] _heightMap[i];
			delete[] _normalMap[i];
		}
		delete[] _heightMap;
		delete[] _normalMap;
	}

	///////////////////////////////////////////////////////////////////////////

	bool HeightField::initialize(const std::string & imgName, float cellSize, float vertScale, float xpos, float zpos, float smoothParam ) {
		_cellSize = cellSize;
		_xpos = xpos;
		_ypos = zpos;
		
		// TODO: Catch errors and exceptions
		Image *img = loadImage( imgName );
		if ( img == 0x0 ) {
			logger << Logger::ERR_MSG << "Unable to load height field image " << imgName << ".";
			return false;
		}

		_W = img->data()->getWidth();
		_H = img->data()->getHeight();

		// TODO: 
		//	Do this in one block of contiguous memory
		_heightMap = new float*[ _W ];
		_normalMap = new Vector3*[ _W ];

		for ( int x = 0; x < _W; x++ ) {
			_heightMap[ x ] = new float[ _H ];
			_normalMap[ x ] = new Vector3[ _H ];
		}
		
		const float VSCALE = vertScale / 255.f;
		for (int x = 0; x < _W; x++ ) {
			for (int z = 0; z < _H; z++ ) {
				_heightMap[ x ][ z ] = img->data()->getPixelAt( x, z ) * VSCALE;
			}
		}

		if ( smoothParam > 0.f ) {
			smoothElevation( smoothParam );
		}

		computeNormals();

		return true;
	}

	///////////////////////////////////////////////////////////////////////////

	Resource * HeightField::load( const std::string & fileName ) {
		// Open the XML
		TiXmlDocument xml( fileName );
		bool loadOkay = xml.LoadFile();

		if ( !loadOkay ) {	// load xml file
			logger << Logger::ERR_MSG << "Could not load height field configuration xml (" << fileName << ") due to xml syntax errors.\n";
			logger << "\t" << xml.ErrorDesc();
			return false;
		}

		TiXmlElement* rootNode = xml.RootElement();	
		if( ! rootNode ) {
			logger << Logger::ERR_MSG << "Height field configuration (" << fileName << ") does not contain a root element.";
			return false;
		}

		if( rootNode->ValueStr () != "HeightField" ) {
			logger << Logger::ERR_MSG << "Height field configuration (" << fileName << ")'s root element is not \"HeightField\".";
			return false;
		}

		std::string absPath;
		os::path::absPath( fileName, absPath );
		std::string junk;
		std::string sceneFldr;
		os::path::split( absPath, sceneFldr, junk );	

		std::string imgName;
		double d;
		float cellSize, vertScale, xPos, yPos, smooth;
		bool valid = true;

		const char * nameCStr = rootNode->Attribute( "file_name" );
		if ( nameCStr == 0x0 ) {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"file_name\" attribute.";
			valid = false;
		}
		imgName = nameCStr;
		
		if ( rootNode->Attribute( "cell_size", &d ) ) {
			cellSize = (float)d;
		} else {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"cell_size\" attribute.";
			valid = false;
		}
		
		if ( rootNode->Attribute( "vert_scale", &d ) ) {
			vertScale = (float)d;
		} else {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"vert_scale\" attribute.";
			valid = false;
		}
		
		if ( rootNode->Attribute( "x", &d ) ) {
			xPos = (float)d;
		} else {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"x\" attribute.";
			valid = false;
		}
		
		if ( rootNode->Attribute( "y", &d ) ) {
			yPos = (float)d;
		} else {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"y\" attribute.";
			valid = false;
		}
		
		if ( rootNode->Attribute( "kernel", &d ) ) {
			smooth = (float)d;
		} else {
			logger << Logger::ERR_MSG << "The HeightField definition " << fileName << " is missing the required \"kernel\" attribute.";
			valid = false;
		}

		if ( valid ) {
			HeightField * hf = new HeightField( fileName );
			if ( ! hf->initialize( os::path::join( 2, sceneFldr.c_str(), imgName.c_str() ), cellSize, vertScale, xPos, yPos, smooth ) ) {
			//if ( ! hf->initialize( imgName, cellSize, vertScale, xPos, yPos, smooth ) ) {
				hf->destroy();
				return 0x0;
			}
			return hf;
		} else {
			logger << Logger::ERR_MSG << "No height field instantiated from " << fileName << ".";
			return 0x0;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	void HeightField::computeNormals() {
		const float DELTA = 2 * _cellSize;

		for ( int x = 0; x < _W; x++ ) {
			for ( int y = 0; y < _H; y++ ) {
				Vector3 Nx;
				if ( x == 0 ) {
					float dh = _heightMap[ x + 1 ][ y ] - _heightMap[ x ][ y ];
					Nx.set( _cellSize, -dh, 0.f );
				} else if ( x == _W - 1 ) {
					float dh = _heightMap[ x ][ y ] - _heightMap[ x - 1 ][ y ];
					Nx.set( _cellSize, -dh, 0.f );
				} else {
					float dh = _heightMap[ x + 1 ][ y ] - _heightMap[ x - 1 ][ y ];
					Nx.set( DELTA, -dh, 0.f );
				}
				
				Vector3 Ny;
				if ( y == 0 ) {
					float dh = _heightMap[ x ][ y + 1 ] - _heightMap[ x ][ y ];
					Ny.set( 0.f, -dh , _cellSize);
				} else if ( y == _H - 1 ) {
					float dh = _heightMap[ x ][ y ] - _heightMap[ x ][ y - 1 ];
					Ny.set( 0.f, -dh , _cellSize);
				} else {
					float dh = _heightMap[ x ][ y + 1 ] - _heightMap[ x ][ y - 1 ];
					Ny.set( 0.f, -dh , DELTA);
				}
				Vector3 norm( Ny.cross( Nx ) );
				norm.normalize();
				//Nx.normalize();
				//Ny.normalize();
				_normalMap[ x ][ y ].set( Vector3( -norm._x, norm._y, -norm._z ) );
				//_normalMap[ x ][ y ].set( norm );
				//_normalMap[ x ][ y ].set( Ny.cross( Nx ) );
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////

	float HeightField::getHeightAtCell( int x, int y ) const {
		return _heightMap[x][y];
	}

	///////////////////////////////////////////////////////////////////////////

	Vector3 HeightField::getNormalAtCell( int x, int y ) const {
		return _normalMap[x][y];
	}

	///////////////////////////////////////////////////////////////////////////

	float HeightField::getHeightAt( float x, float y ) const {
		x = (x - _xpos) / _cellSize;
		y = (y - _ypos) / _cellSize;
		
		if ( ( x < 0 ) || ( y < 0 ) || ( x > _W-1 ) || ( y > _H-1 ) ) { return 0.f; }

		int x1 = (int)floor(x);
		int y1 = (int)floor(y);
		int x2 = MIN(_W-1, x1+1);
		int y2 = MIN(_H-1, y1+1);

		float f11 = _heightMap[x1][y1];
		float f12 = _heightMap[x1][y2];
		float f21 = _heightMap[x2][y1];
		float f22 = _heightMap[x2][y2];

		x -= x1;
		y -= y1;

		float res = f11*(1-x)*(1-y) + f21*x*(1-y) + f12*(1-x)*y + f22*x*y;
		return res;
	}

	///////////////////////////////////////////////////////////////////////////

	Vector3 HeightField::getNormalAt( float x, float y ) const {
		x -= _xpos;
		y -= _ypos;
		int X = (int)( x / _cellSize );
		int Y = (int)( y / _cellSize );
		if ( X < 0 ) X = 0;
		else if ( X >= _W ) X = _W - 1;
		if ( Y < 0 ) Y = 0;
		else if ( Y >= _H ) Y = _H - 1;
		return _normalMap[ X ][ Y ];
	}

	///////////////////////////////////////////////////////////////////////////

	void HeightField::smoothElevation( float smooth ) {
		// Compute the kernel size
		int cellCount = (int)(( 6.f * smooth ) / _cellSize + 0.5f );
		if ( cellCount % 2 == 0 ) ++cellCount;
		// compute the kernel
		float * kernel = new float[ cellCount ];
		int halfCount = cellCount / 2;
		int cell = -halfCount;
		float sum = 0.f;
		float denom = 1.f / ( 2 * smooth * smooth );
		for ( int i = 0; i < cellCount; ++i, ++cell ) {
			float x = cell * _cellSize;
			kernel[ i ] = exp( -x * x * denom );
			sum += kernel[ i ];
		}
		// normalize
		denom = 1.f / sum;
		for ( int i = 0; i < cellCount; ++i ) {
			kernel[ i ] *= denom;
		}

		// Normalization factors for one-sided kernels
		//	normFactor[ i ] is what I should multiply the convolved value when the kernel
		//	is centered either on index i, or END_VALUE - (i + 1).
		float * normFactors = new float[ halfCount ];
		for ( int i = 0; i < halfCount; ++i ) {
			float sum = 0.f;
			for ( int j = halfCount - i; j < cellCount; ++j ) {
				sum += kernel[ j ];
			}
			normFactors[i] = 1.f / sum;
		}

		// Prepare to convolve 
		float * workSpace = new float[ _W > _H ? _W : _H ];
		// iterate along width axis
		int SIZE = _W;
		for ( int strip = 0; strip < _H; ++strip ) {
			for ( int center = 0; center < SIZE; ++center ) {
				// convolve data
				float sum = 0.f;
				if ( center >= halfCount && center < SIZE - halfCount ) {
					int i = 0;
					for ( int k = center - halfCount; k < center + halfCount + 1; ++k, ++i ) {
						sum += _heightMap[ k ][ strip ] * kernel[ i ];
					}
				} else if ( center < halfCount ) {
					// Truncated kernel on the left
					int i = halfCount - center;
					for ( int k = 0; k <= center + halfCount; ++k, ++i ) {
						sum += _heightMap[ k ][ strip ] * kernel[ i ];
					}
					sum *= normFactors[ center ];
				} else {
					// Truncated kernel on the right
					int i = 0;
					for ( int k = center - halfCount; k < SIZE; ++k, ++i ) {
						sum += _heightMap[ k ][ strip ] * kernel[ i ];
					}
					sum *= normFactors[ SIZE - 1 - center ];
				}
				workSpace[ center ] = sum;
			}
				// recopy data
			for ( int center = 0; center < SIZE; ++center ) {
				_heightMap[ center ][ strip ] = workSpace[ center ];
			}
		}

		// iterate along the other axis

		SIZE = _H;
		for ( int strip = 0; strip < _H; ++strip ) {
			for ( int center = 0; center < SIZE; ++center ) {
				// convolve data
				float sum = 0.f;
				if ( center >= halfCount && center < SIZE - halfCount ) {
					int i = 0;
					for ( int k = center - halfCount; k < center + halfCount + 1; ++k, ++i ) {
						sum += _heightMap[ strip ][ k ] * kernel[ i ];
					}
				} else if ( center < halfCount ) {
					// Truncated kernel on the left
					int i = halfCount - center;
					for ( int k = 0; k <= center + halfCount; ++k, ++i ) {
						sum += _heightMap[ strip ][ k ] * kernel[ i ];
					}
					sum *= normFactors[ center ];
				} else {
					// Truncated kernel on the right
					int i = 0;
					for ( int k = center - halfCount; k < SIZE; ++k, ++i ) {
						sum += _heightMap[ strip ][ k ] * kernel[ i ];
					}
					sum *= normFactors[ SIZE - 1 - center ];
				}
				workSpace[ center ] = sum;
			}
				// recopy data
			for ( int center = 0; center < SIZE; ++center ) {
				_heightMap[ strip ][ center ] = workSpace[ center ];
			}
		}

		delete [] kernel;
		delete [] normFactors;
		delete [] workSpace;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	HeightFieldPtr loadHeightField( const std::string & fileName ) throw ( ResourceException ) {
		Resource * rsrc = ResourceManager::getResource( fileName, &HeightField::load, HeightField::LABEL );
		if ( rsrc == 0x0 ) {
			logger << Logger::ERR_MSG << "No height field resource available.";
			throw ResourceException();
		}
		HeightField * hf = dynamic_cast< HeightField * >( rsrc );
		if ( hf == 0x0 ) {
			logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a height field.";
			throw ResourceException();
		}
		return HeightFieldPtr( hf );
	}

}	// namespace Terrain

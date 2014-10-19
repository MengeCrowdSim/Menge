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

#include "RandGenerator.h"
#include "SimRandom.h"
#include "tinyxml/tinyxml.h"
#include <ctime>
#include <limits>

#ifdef _WIN32
#define NOMINMAX	// prevent windows.h from stomping on "max" - used by limits
#include "windows.h"
#else	// _WIN32
#include <ctime>
#include "include/macros.h"
#endif	// _WIN32

namespace Menge {

	namespace Math {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of Default Seed Generator
		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The global seed for the number generators.
		 *
		 *	If the seed is zero, the number generators will get a
		 *	unique seed based on the system clock.  If it is non-zero
		 *	they will get this seed (unless they locally override it
		 *	explicitly).
		 */
		int	GLOBAL_SEED = 0;

		/*!
		 *	@brief		The number of generators that have been created.
		 *				This is used to create a constant, fixed variety.
		 *				If every generator gets exactly the same seed, then
		 *				the results are strongly correlated.  By incrementing
		 *				the seed by the number of times the getSeed is called
		 *				it guarantees uncorrelated pseudo-random sequences.
		 */
		int SEED_REQUESTS = 0;

		/////////////////////////////////////////////////////////////////////

		void setDefaultGeneratorSeed( int seed ) {
			GLOBAL_SEED = seed;
		}

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Based on the default seed state, return a random
		 *				generator seed.
		 *
		 *	@returns	A seed for a number generator.
		 */
		int getDefaultSeed() {
			if ( GLOBAL_SEED ) {
				return GLOBAL_SEED + ++SEED_REQUESTS;
			} else {
	#ifdef _WIN32
				__int64 t;
				::QueryPerformanceCounter( (LARGE_INTEGER*) &t );
				return static_cast< int >( t );
	#else
				struct timespec t;
				clock_gettime( CLOCK_REALTIME, &t );
				return static_cast< int >( t.tv_nsec );
	#endif
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstFloatGenerator
		/////////////////////////////////////////////////////////////////////

		void ConstFloatGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		FloatGenerator * ConstFloatGenerator::copy() const {
			return new ConstFloatGenerator( _value );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const ConstFloatGenerator & gen ) {
			out << "Const float: value( " << gen._value << " )";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NormalFloatGenerator
		/////////////////////////////////////////////////////////////////////

		NormalFloatGenerator::NormalFloatGenerator( float mean, float stddev, float minVal, float maxVal, int seed ):FloatGenerator(), _mean(mean),_std(stddev),_min(minVal),_max(maxVal),_second(0.f),_calls(0) {
			if ( seed == 0 ) {
				_seed = getDefaultSeed();
			} else {
				_seed = seed;
			}
		}

		/////////////////////////////////////////////////////////////////////

		void NormalFloatGenerator::set( float mean, float stddev, float minVal, float maxVal ) {
			_mean = mean;
			_std = stddev;
			_min = minVal;
			_max = maxVal;
			_calls = 0;
			_seed = getDefaultSeed();
		}

		/////////////////////////////////////////////////////////////////////

		float NormalFloatGenerator::getValue() const {
			float val;
			if ( _calls % 2 == 0 ) {	// Generate new values			
				r4_normalR( _mean, _std, val, _second, &_seed );
			} else {					// simply return second value			
				val = _second;
			}
			++_calls;
				
			if ( val < _min ) val = _min;
			else if ( val > _max ) val = _max;

			return val;
		}

		/////////////////////////////////////////////////////////////////////

		float NormalFloatGenerator::getValueConcurrent() const {
			_lock.lock();
			float value = getValue();
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		void NormalFloatGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		FloatGenerator * NormalFloatGenerator::copy() const {
			return new NormalFloatGenerator( _mean, _std, _min, _max, _seed + 1 );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const NormalFloatGenerator & gen ) {
			out << "Normal float: mean( " << gen._mean << " ), std( " << gen._std << " ) in the range [ " << gen._min << ", " << gen._max << " ]";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of UniformFloatGenerator
		/////////////////////////////////////////////////////////////////////

		UniformFloatGenerator::UniformFloatGenerator( float minVal, float maxVal, int seed ):FloatGenerator(), _min(minVal),_size(maxVal-minVal) {
			if ( seed == 0 ) {
				_seed = getDefaultSeed();
			} else {
				_seed = seed;
			}
		}

		/////////////////////////////////////////////////////////////////////

		UniformFloatGenerator::UniformFloatGenerator( const UniformFloatGenerator & gen ):FloatGenerator(gen), _min(gen._min), _size(gen._size), _seed(gen._seed+1) {
		}

		/////////////////////////////////////////////////////////////////////

		float UniformFloatGenerator::getValue() const {
			return _min + r4_uniform_01( &_seed ) * _size;
		}

		/////////////////////////////////////////////////////////////////////

		float UniformFloatGenerator::getValueConcurrent() const {
			_lock.lock();
			float value = getValue();
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		void UniformFloatGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		FloatGenerator * UniformFloatGenerator::copy() const {
			return new UniformFloatGenerator( _min, _min + _size, _seed + 1 );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const UniformFloatGenerator & gen ) {
			out << "Uniform float: range[ " << gen._min << " , " << (gen._min + gen._size ) << "  ]";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstIntGenerator
		/////////////////////////////////////////////////////////////////////

		void ConstIntGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		IntGenerator * ConstIntGenerator::copy() const {
			return new ConstIntGenerator( _value );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const ConstIntGenerator & gen ) {
			out << "Const int: value( " << gen._value << " )";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of UniformIntGenerator
		/////////////////////////////////////////////////////////////////////

		UniformIntGenerator::UniformIntGenerator( int minVal, int maxVal, int seed ):IntGenerator(), _min(minVal),_size(maxVal-minVal+1),_seed(seed) {
			if ( seed == 0 ) {
				_seed = getDefaultSeed();
			} else {
				_seed = seed;
			}
		}

		/////////////////////////////////////////////////////////////////////

		int UniformIntGenerator::getValue() const {
			int randVal = static_cast< int >( r4_uniform_01( &_seed ) * std::numeric_limits<int>::max() );
			int val = randVal % _size;
			return _min + val;
		}

		/////////////////////////////////////////////////////////////////////

		int UniformIntGenerator::getValueConcurrent() const {
			_lock.lock();
			int value = getValue();
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		void UniformIntGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		IntGenerator * UniformIntGenerator::copy() const {
			return new UniformIntGenerator( _min, _min + _size, _seed + 1 );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const UniformIntGenerator & gen ) {
			out << "Uniform int: range[ " << gen._min << " , " << (gen._min + gen._size - 1) << "  ]";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of Zero2DGenerator
		/////////////////////////////////////////////////////////////////////

		void Zero2DGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const Zero2DGenerator & gen ) {
			out << "ZERO 2D Generator";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of Const2DGenerator
		/////////////////////////////////////////////////////////////////////

		void Const2DGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const Const2DGenerator & gen ) {
			out << "Const 2D Generator: value = " << gen._value;
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBUniformPosGenerator
		/////////////////////////////////////////////////////////////////////

		AABBUniformPosGenerator::AABBUniformPosGenerator( const Vector2 & minPt, const Vector2 & maxPt, int seed ):Vec2DGenerator(), _xRand(minPt.x(),maxPt.x(),seed==0?getDefaultSeed():seed), _yRand(minPt.y(), maxPt.y(),(seed==0?getDefaultSeed():seed)+5) {
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 AABBUniformPosGenerator::getValue() const {
			return Vector2( _xRand.getValue(), _yRand.getValue() );
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 AABBUniformPosGenerator::getValueConcurrent() const {
			_lock.lock();
			Vector2 value( _xRand.getValue(), _yRand.getValue() );
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		AABBUniformPosGenerator::AABBUniformPosGenerator( const AABBUniformPosGenerator & aabbGen ): _xRand(aabbGen._xRand), _yRand(aabbGen._yRand) {
		}

		/////////////////////////////////////////////////////////////////////

		Vec2DGenerator * AABBUniformPosGenerator::copy() const {
			return new AABBUniformPosGenerator( *this );
		}

		/////////////////////////////////////////////////////////////////////

		void AABBUniformPosGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const AABBUniformPosGenerator & gen ) {
			out << "Uniform in AABB: ";
			out << "x: " << gen._xRand << ", y: " << gen._yRand;
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBUniformPosGenerator
		/////////////////////////////////////////////////////////////////////

		OBBUniformPosGenerator::OBBUniformPosGenerator( const Vector2 & minPt, const Vector2 & size, float theta, int seed ):Vec2DGenerator(), _xRand(0.f, size.x(),seed==0?getDefaultSeed():seed), _yRand(0.f, size.y(),(seed==0?getDefaultSeed():seed)+5), _minPt(minPt), _cosTheta(cos(theta)), _sinTheta(sin(theta)) {
		}

		/////////////////////////////////////////////////////////////////////

		OBBUniformPosGenerator::OBBUniformPosGenerator( const OBBUniformPosGenerator & obbGen ): _xRand(obbGen._xRand), _yRand(obbGen._yRand), _minPt(obbGen._minPt), _cosTheta(obbGen._cosTheta), _sinTheta(obbGen._sinTheta) {
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 OBBUniformPosGenerator::getValue() const {
			Vector2 inRect( _xRand.getValue(), _yRand.getValue() );
			// rotate
			const float x = inRect.x() * _cosTheta - inRect.y() * _sinTheta + _minPt.x();
			const float y = inRect.y() * _cosTheta + inRect.x() * _sinTheta + _minPt.y();
			return Vector2( x, y );
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 OBBUniformPosGenerator::getValueConcurrent() const {
			_lock.lock();
			Vector2 value( getValue() );
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		Vec2DGenerator * OBBUniformPosGenerator::copy() const {
			return new OBBUniformPosGenerator( *this );
		}

		/////////////////////////////////////////////////////////////////////

		void OBBUniformPosGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const OBBUniformPosGenerator & gen ) {
			out << "Uniform in OBB:";
			out << " x: " << gen._xRand << ", y: " << gen._yRand;
			out << ", minPt: " << gen._minPt;
			out << ", theta: " << atan2( gen._sinTheta, gen._cosTheta );
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of WeightedInt
		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const WeightedInt & wInt ) {
			out << wInt._val << "( " << wInt._wt << " )";
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of WeightedIntGenerator
		/////////////////////////////////////////////////////////////////////

		WeightedIntGenerator::WeightedIntGenerator():IntGenerator(), _dice(0.f,1.f,getDefaultSeed()), _pairs() {
		}

		/////////////////////////////////////////////////////////////////////

		WeightedIntGenerator::WeightedIntGenerator( const WeightedIntGenerator & gen):IntGenerator(), _dice(gen._dice) {
			_pairs.insert( _pairs.begin(), gen._pairs.begin(), gen._pairs.end() );
		}

		/////////////////////////////////////////////////////////////////////

		int WeightedIntGenerator::getValue() const {
			size_t pCount = _pairs.size();
			float t = _dice.getValue();
			float start = 0.f;
			for ( size_t i = 0; i < pCount; ++i ) {
				float end = _pairs[i]._wt;
				if ( t >= start && t < end ) {
					return _pairs[i]._val;
				}
			}
			return _pairs[ pCount - 1 ]._val;
		}

		/////////////////////////////////////////////////////////////////////

		int WeightedIntGenerator::getValueConcurrent() const {
			_lock.lock();
			int value = getValue();
			_lock.release();
			return value;
		}

		/////////////////////////////////////////////////////////////////////

		void WeightedIntGenerator::addValue(int value, float weight) {
			_pairs.push_back( WeightedInt( value, weight ) );
		}

		/////////////////////////////////////////////////////////////////////

		void WeightedIntGenerator::finalize() {
			size_t pCount = _pairs.size();
			float sum = 0.f;
			for ( size_t i = 0; i < pCount; ++i ) {
				sum += _pairs[i]._wt;
			}
			sum = 1.f / sum;
			for ( size_t i = 0; i < pCount; ++i ) {
				_pairs[i]._wt *= sum;
				if ( i > 0 ) {
					_pairs[i]._wt += _pairs[i-1]._wt;
				}
			}
		}

		/////////////////////////////////////////////////////////////////////

		void WeightedIntGenerator::print( Logger & out ) const {
			out << (*this);
		}

		/////////////////////////////////////////////////////////////////////

		IntGenerator * WeightedIntGenerator::copy() const {
			return new WeightedIntGenerator( *this );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const WeightedIntGenerator & gen ) {
			out << "Weighted int generator:";
			std::vector< WeightedInt >::const_iterator itr = gen._pairs.begin();
			for ( ; itr != gen._pairs.end(); ++itr ) {
				out << ", " << (*itr);
			}
			return out;
		}

		/////////////////////////////////////////////////////////////////////

		// Given an xml node which SHOULD have the definition of a 2D generator, this
		//	instantiates one
		Vec2DGenerator * create2DGenerator( TiXmlElement * node, float scale ) {
			int seed = getDefaultSeed();
			Vec2DGenerator * gen = 0x0;
			const char * genCStr = node->Attribute( "dist" );
			if ( genCStr == 0x0 ) {
				logger << Logger::WARN_MSG << "Missing the \"dist\" attribute on line " << node->Row() << "\n";
				return 0x0;
			}
			std::string genType( genCStr );
			if ( genType == "n" ) {
				logger << Logger::ERR_MSG << "Normal 2D generators not supported yet!\n";
			} else if ( genType == "u" ) {
				double minX, maxX, minY, maxY;
				if ( ! node->Attribute( "min_x", &minX ) ) {
					logger << Logger::ERR_MSG << "Uniform 2D distributon requires \"min_x\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( "max_x", &maxX ) ) {
					logger << Logger::ERR_MSG << "Uniform 2D distributon requires \"max_x\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( "min_y", &minY ) ) {
					logger << Logger::ERR_MSG << "Uniform 2D distributon requires \"min_y\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( "max_y", &maxY ) ) {
					logger << Logger::ERR_MSG << "Uniform 2D distributon requires \"max_y\" attribute.\n";
					return gen;
				}
				node->Attribute( "seed", &seed );
				gen = new AABBUniformPosGenerator( Vector2( (float)minX * scale, (float)minY * scale ), Vector2( (float)maxX * scale, (float)maxY * scale ), seed );
			} else if ( genType == "c" ) {
				double x, y;
				if ( ! node->Attribute( "x_value", &x ) ) {
					logger << Logger::ERR_MSG << "Constant 2D distributon requires \"x_value\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( "y_value", &y ) ) {
					logger << Logger::ERR_MSG << "Constant 2D distributon requires \"y_value\" attribute.\n";
					return gen;
				}
				gen = new Const2DGenerator( Vector2( (float)x * scale, (float)y * scale ) );
			} else if ( genType == "" ) {
				logger << Logger::ERR_MSG << "No 2D number generation type specified.  Should be u or c.\n";
			} else {
				logger << Logger::ERR_MSG << "Unrecognized 2D number generation type: " << genType << "\n";
			}
			return gen;
		}

		/////////////////////////////////////////////////////////////////////

		FloatGenerator * createFloatGenerator( TiXmlElement * node, float scale, const std::string & prefix ) {
			FloatGenerator * gen = 0x0;
			std::string distS = prefix + "dist";
			const char * genCStr = node->Attribute( distS.c_str() );
			if ( genCStr == 0x0 ) {
				logger << Logger::WARN_MSG << "Missing the \"dist\" attribute on line " << node->Row() << "\n";
				return 0x0;
			}
			std::string genType( genCStr );
			
			if ( genType == "n" ) {
				double mean, std;
				if ( ! node->Attribute( prefix + "mean", &mean ) ) {
					logger << Logger::ERR_MSG << "Normal distribution requires \"mean\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( prefix + "stddev", &std ) ) {
					logger << Logger::ERR_MSG << "Normal distribution requires \"stddev\" attribute.\n";
					return gen;
				}
				int seed;
				if ( !node->Attribute( prefix + "seed", &seed ) ) {
					seed = getDefaultSeed();
				}
				mean *= scale;
				std *= scale;
				double minVal = mean - 3.f * std;
				double maxVal = mean + 3.f * std;
				gen = new NormalFloatGenerator( (float)mean, (float)std, (float)minVal, (float)maxVal, seed );
			} else if ( genType == "u" ) {
				double minVal, maxVal;
				if ( ! node->Attribute( prefix + "min", &minVal ) ) {
					logger << Logger::ERR_MSG << "Uniform distribution requires \"min\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( prefix + "max", &maxVal ) ) {
					logger << Logger::ERR_MSG << "Uniform distribution requires \"max\" attribute.\n";
					return gen;
				}
				int seed;
				if ( !node->Attribute( prefix + "seed", &seed ) ) {
					seed = getDefaultSeed();
				}
				minVal *= scale;
				maxVal *= scale;
				gen = new UniformFloatGenerator( (float)minVal, (float)maxVal, seed );
			} else if ( genType == "c" ) {
				double val;
				if ( ! node->Attribute( prefix + "value", &val ) ) {
					logger << Logger::ERR_MSG << "Constant distribution requires \"value\" attribute.\n";
					return gen;
				}
				gen = new ConstFloatGenerator( (float)val * scale );
			} else if ( genType == "" ) {
				logger << Logger::ERR_MSG << "Float generation requires a type: n, c or u.\n";
			} else {
				logger << Logger::ERR_MSG << "Unrecognized float generation type: " << genType << "\n";
			}
			return gen;
		}

		/////////////////////////////////////////////////////////////////////

		IntGenerator * createIntGenerator( TiXmlElement * node, const std::string & prefix ) {
			int seed = getDefaultSeed();
				
			IntGenerator * gen = 0x0;
			std::string distS = prefix + "dist";
			const char * genCStr = node->Attribute( distS.c_str() );
			if ( genCStr == 0x0 ) {
				logger << Logger::WARN_MSG << "Missing the \"dist\" attribute on line " << node->Row() << "\n";
				return 0x0;
			}
			std::string genType( genCStr );
			if ( genType == "u" ) {
				int minVal, maxVal;
				if ( ! node->Attribute( prefix + "min", &minVal ) ) {
					logger << Logger::ERR_MSG << "Uniform distribution requires \"min\" attribute.\n";
					return gen;
				}
				if ( ! node->Attribute( prefix + "max", &maxVal ) ) {
					logger << Logger::ERR_MSG << "Uniform distribution requires \"max\" attribute.\n";
					return gen;
				}
				node->Attribute( "seed", &seed );
				gen = new UniformIntGenerator( minVal, maxVal, seed );
			} else if ( genType == "c" ) {
				int val;
				if ( ! node->Attribute( prefix + "value", &val ) ) {
					logger << Logger::ERR_MSG << "Constant distribution requires \"value\" attribute.\n";
					return gen;
				}
				gen = new ConstIntGenerator( val );
			} else if ( genType == "" ) {
				logger << Logger::ERR_MSG << "Int generation requires a distribution type: c or u.\n";
			} else {
				logger << Logger::ERR_MSG << "Unrecognized int generation type: " << genType << ". Must be c or u.\n";
			}
			return gen;
		}
	}	// namespace Math
}	// namespace Menge
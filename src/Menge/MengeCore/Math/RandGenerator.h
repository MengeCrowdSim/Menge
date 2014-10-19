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
 *	@file		RandGenerator.h
 *	@brief		Utility for generating number distributions.
 *
 *	Number generators simple purpose is to produce a value, scalar or vector,
 *	float or int.  These values may be constant or random with either normal
 *	or uniform distributions.  The number generator provides an interface
 *	so that the various entities do not need to know anything about the
 *	value type or distribution.
 *
 *	When pointers to number generators are passed around, "ownership" never
 *	changes.  I.e., the caller is still responsible for the eventually freeing
 *	up the memory.  If the callee needs to save an instance of the generator
 *	(rather than simply just using it to obtain a value) it is responsible
 *	for calling the copy method to obtain its own unique copy.  It is then
 *	responsible for freeing up that copy.
 */

#ifndef __RAND_GENERATOR_H__
#define __RAND_GENERATOR_H__

#include "CoreConfig.h"
#include "vector.h"
#include <iostream>
#include <vector>
#include "SimpleLock.h"

// Forward Declarations
class	TiXmlElement;

namespace Menge {

	namespace Math {
		/*!
		 *	@brief		Allows the global random number seed value to be set.
		 *
		 *	This allows a bit more control over the random number generator.  Setting
		 *	the global seed value to zero will cause the default seed value to be 
		 *	"randomly" generated from the system clock.  This will cause repeated
		 *	executions of the program to vary.
		 *
		 *	Setting the seed to a non-zero constant will still allow for pseudo-random
		 *	distribution of values, but the pattern of distributions will be the same.
		 *	Two different non-zero values will lead to two different, but repeatable,
		 *	distributions.
		 *
		 *	@param		seed		The desired seed.
		 */
		MENGE_API void setDefaultGeneratorSeed( int seed );

		/*!
		 *	@brief		Retrieves a seed based on the global settings
		 *
		 *	@returns	A random seed according to global parameters.
		 */
		MENGE_API int getDefaultSeed();

		/*!
		 *	@brief		Generic *abstract* class which generates a scalar float value
		 */
		class MENGE_API FloatGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			FloatGenerator(){}

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~FloatGenerator(){}

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	This *must* be overridden by each derived class to provide a string format
			 *	of the float generator.
			 *
			 *	This method is used instead of the traditional method of writing a 
			 *	friend overloaded operator (>>).  The reason for this is that using pointers to
			 *	this base class to refer to derived classes would need to be derefenced to pass
			 *	(as objects) to the output stream and dereferencing them precludes the use of 
			 *	polymorphism in the format.
			 *
			 *	This could be overcome by writing the streaming operator to operate on 
			 *	a pointer to the base class, which in turn calls a virtual function.  But this
			 *	approach removes one level of indirection, and preserves the ability to output
			 *	pointer addresses for the sake of debugging.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const = 0;

			/*!
			 *	@brief		Return a value based on the float generation rules.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A float value.
			 */
			virtual float getValue() const = 0;
			
			/*!
			 *	@brief		Return a value based on the float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A float value.
			 */
			virtual float getValueConcurrent() const = 0;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *	This is not an exact copy.  The copy should not have the same seed as the
			 *	original.  All other parameters should be identical.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual FloatGenerator * copy() const = 0;	
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A FloatGenerator which returns a constant value.
		 */
		class MENGE_API ConstFloatGenerator : public FloatGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		value		The constant value this generator returns.
			 */
			ConstFloatGenerator( float value ):FloatGenerator(), _value(value){}

			/*!
			 *	@brief		Return a value based on the float generation rules.
			 *
			 *	@return		A constant float value.
			 */
			virtual float getValue() const { return _value; }
			
			/*!
			 *	@brief		Return a value based on the float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A float value.
			 */
			virtual float getValueConcurrent() const { return _value; }

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual FloatGenerator * copy() const;	

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const ConstFloatGenerator & gen );

		protected:
			/*!
			 *	@brief		The generator's constant value.
			 */
			float _value;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A FloatGenerator which returns a normally distributed value.
		 *
		 *	Furthermore, the normally distributed value is clamped to a range [min, max] that
		 *	prevents the otherwise theoretically possible (though highly improbable) values that
		 *	can lie outside of practical ranges.
		 *
		 *	The min and max values should span a range which includes the interval:
		 *	[ mean - 3 * std. dev.,  mean + 3 * std. dev.]
		 *	Clamping the range smaller will lead to abnormal accretions of samples at the clamping
		 *	boundaries.  By having the range span at least six standard deviations, centered at the
		 *	mean, most of the normal distribution will be unaffected by clamping.
		 */
		class MENGE_API NormalFloatGenerator : public FloatGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	The value is normally distributed around mean, with a standard deviation of stddev
			 *	but clamped to lie within the range [minVal, maxVal].
			 *
			 *	@param		mean		The mean value of the distribution.
			 *	@param		stddev		The standard deviation of the value
			 *	@param		minVal		The lower clamped value, such that all values returned will be
			 *							greater than or equal to minVal.  For largely "pure" normal
			 *							distribution, minVal <= mean - 3 * stddev.
			 *	@param		maxVal		The upper clamped value, such that all values returned will be
			 *							less than or equal to maxVal.  For largely "pure" normal
			 *							distribution, maxVal >= mean + 3 * stddev.
			 *	@param		seed		If the seed is zero, the global seed will be used otherwise
			 *							the particular seed will be used.
			 */
			NormalFloatGenerator( float mean, float stddev, float minVal, float maxVal, int seed=0 );

			/*!
			 *	@brief		Sets the distribution parameters.
			 *
			 *	@param		mean		The mean value of the distribution.
			 *	@param		stddev		The standard deviation of the value
			 *	@param		minVal		The lower clamped value, such that all values returned will be
			 *							greater than or equal to minVal.  For largely "pure" normal
			 *							distribution, minVal <= mean - 3 * stddev.
			 *	@param		maxVal		The upper clamped value, such that all values returned will be
			 *							less than or equal to maxVal.  For largely "pure" normal
			 *							distribution, maxVal >= mean + 3 * stddev.
			 */
			void set( float mean, float stddev, float minVal, float maxVal );
			
			/*!
			 *	@brief		Return a value based on the float generation rules.
			 *
			 *	@return		A clamped, normally-distributed float value.
			 */
			virtual float getValue() const;
			
			/*!
			 *	@brief		Return a value based on the float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A float value.
			 */
			virtual float getValueConcurrent() const;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual FloatGenerator * copy() const;	

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const NormalFloatGenerator & gen );

		protected:
			/*!
			 *	@brief		The mean value of the distribution.
			 */
			float _mean;

			/*!
			 *	@brief		The standard deviation of the value
			 */
			float _std;

			/*!
			 *	@brief		The lower clamped value, such that all values returned will be
			 *				greater than or equal to minVal.  For largely "pure" normal
			 *				distribution, minVal <= mean - 3 * stddev.
			 */
			float _min;
			/*!
			 *	@brief		The upper clamped value, such that all values returned will be
			 *				less than or equal to maxVal.  For largely "pure" normal
			 *				distribution, maxVal >= mean + 3 * stddev.
			 */
			float _max;

			/*!
			 *	@brief		The second random number generated (see Math::r4_normalR).
			 */
			mutable float	_second;

			/*!
			 *	@brief		The number of calls to the generator.  Every second call requires
			 *				a new call to the random number generator.
			 */
			mutable unsigned int _calls;

			/*!
			 *	@brief		A seed for the random number generator.
			 */
			mutable int	  _seed;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A FloatGenerator which returns a uniformly distributed value within
		 *				a defined range.
		 */
		class MENGE_API UniformFloatGenerator : public FloatGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	The value will lie within the range [minVal, maxVal].  All values in
			 *	that range have equal probability of being selected.
			 *
			 *	@param		minVal		The lower end of the valid range.
			 *	@param		maxVal		The upper end of the valid range.
			 *	@param		seed		If the seed is zero, the global seed will be used otherwise
			 *							the particular seed will be used.
			 */
			UniformFloatGenerator( float minVal, float maxVal, int seed=0 );

			/*!
			 *	@brief		Copy constructor
			 *
			 *	The copy is not a perfect copy - the seed value in the new
			 *	float generator is NOT the same as the source.
			 *  @param		gen		The generator to copy
			 */
			UniformFloatGenerator( const UniformFloatGenerator & gen );

			/*!
			 *	@brief		Return a value based on the float generation rules.
			 *
			 *	@return		A clamped, uniformly-distributed float value.
			 */
			virtual float getValue() const;
			
			/*!
			 *	@brief		Return a value based on the float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A float value.
			 */
			virtual float getValueConcurrent() const;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual FloatGenerator * copy() const;	

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const UniformFloatGenerator & gen );

			/*!
			 *	@brief		Reports the lower end of the valid range.
			 *
			 *	@returns	The lower end of the valid range.
			 */
			float getMin() const { return _min; }
			
			/*!
			 *	@brief		Reports the upper end of the valid range.
			 *
			 *	@returns	The upper end of the valid range.
			 */
			float   getMax() const { return _min + _size; }

			/*!
			 *	@brief		Reports the size of the interval.
			 *
			 *	@returns	The size of the interval.
			 */
			float getSize() const { return _size; }

		protected:
			/*!
			 *	@brief		The lower end of the valid range.
			 */
			float _min;

			/*!
			 *	@brief		The size of the valid range.
			 */
			float _size;

			/*!
			 *	@brief		A seed for the random number generator.
			 */
			mutable int	  _seed;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Generic *abstract* class which generates a scalar integer value
		 */
		class MENGE_API IntGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			IntGenerator() {}

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~IntGenerator() {}

			/*!
			 *	@brief		Return a value based on the integer generation rules.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		An integer value.
			 */
			virtual int getValue() const = 0;
			
			/*!
			 *	@brief		Return a value based on the integer generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A integer value.
			 */
			virtual int getValueConcurrent() const = 0;
			
			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	This *must* be overridden by each derived class to provide a string format
			 *	of the float generator.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const = 0;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual IntGenerator * copy() const = 0;	
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		An IntGenerator which returns a constant value.
		 */
		class MENGE_API ConstIntGenerator : public IntGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		value		The constant value this generator returns.
			 */
			ConstIntGenerator( int value ): IntGenerator(),_value(value) {}

			/*!
			 *	@brief		Return a value based on the integer generation rules.
			 *
			 *	@return		A constant integer value.
			 */
			virtual int getValue() const { return _value; }
			
			/*!
			 *	@brief		Return a value based on the integer generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A integer value.
			 */
			virtual int getValueConcurrent() const { return _value; }
			
			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual IntGenerator * copy() const;

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const ConstIntGenerator & gen );

			
		protected:
			/*!
			 *	@brief		The generator's constant value.
			 */
			int	_value;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		An IntGenerator which returns a uniformly distributed value within
		 *				a defined range.
		 */
		class MENGE_API UniformIntGenerator : public IntGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	The value will lie within the range [minVal, maxVal].  All values in
			 *	that range have equal probability of being selected.
			 *
			 *	@param		minVal		The lower end of the valid range.
			 *	@param		maxVal		The upper end of the valid range.
			 *	@param		seed		If the seed is zero, the global seed will be used otherwise
			 *							the particular seed will be used.
			 */
			UniformIntGenerator( int minVal, int maxVal, int seed=0 );

			/*!
			 *	@brief		Set the selection range.
			 *
			 *	@param		minVal		The lower end of the valid range.
			 *	@param		maxVal		The upper end of the valid range.
			 */
			void setRange( int minVal, int maxVal ) { _min = minVal; _size = maxVal - minVal + 1; }

			/*!
			 *	@brief		Return a value based on the integer generation rules.
			 *
			 *	@return		A uniformly distributed integer value.
			 */
			virtual int getValue() const;
			
			/*!
			 *	@brief		Return a value based on the integer generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A integer value.
			 */
			virtual int getValueConcurrent() const;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual IntGenerator * copy() const;

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const UniformIntGenerator & gen );

		protected:
			/*!
			 *	@brief		The lower end of the valid range.
			 */
			int _min;
			
			/*!
			 *	@brief		The size of the valid range.
			 */
			int _size;

			/*!
			 *	@brief		A seed for the random number generator.
			 */
			mutable int _seed;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};


		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Generic *abstract* class which generates a 2D vector float values
		 */
		class MENGE_API Vec2DGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Vec2DGenerator() {}

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~Vec2DGenerator(){}

			/*!
			 *	@brief		Return a value based on the 2D float generation rules.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValue() const = 0;
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValueConcurrent() const = 0;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	This *must* be overridden by each derived class to provide a string format
			 *	of the float generator.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const = 0;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual Vec2DGenerator * copy() const = 0;	
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A 2D float generator which always returns a zero.
		 */
		class MENGE_API Zero2DGenerator : public Vec2DGenerator {
		public:
			/*!
			 *	@brief		Return a value based on the 2D float generation rules.
			 *
			 *	@return		A 2D zero vector.
			 */
			virtual Vector2 getValue() const { return Vector2(0.f, 0.f); }
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValueConcurrent() const { return Vector2(0.f, 0.f); }
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual Vec2DGenerator * copy() const { return new Zero2DGenerator(); }

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const Zero2DGenerator & gen );
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A 2D float generator which always returns a constant 2D vector.
		 */
		class MENGE_API Const2DGenerator : public Vec2DGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		val		The value to be returned.
			 */
			Const2DGenerator( const Vector2 & val ): Vec2DGenerator(), _value(val) {}
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules.
			 *
			 *	@return		A constat 2D value.
			 */
			virtual Vector2 getValue() const { return _value; }
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValueConcurrent() const { return _value; }
			
			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual Vec2DGenerator * copy() const { return new Const2DGenerator( _value ); }

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const Const2DGenerator & gen );

		protected:
			/*!
			 *	@brief		The constant value to return.
			 */
			Vector2	_value;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Generates a 2D float value uniformly distributed in an axis-aligned box (AAB).
		 */
		class MENGE_API AABBUniformPosGenerator : public Vec2DGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		minPt		The minimum extent of the AAB.
			 *	@param		maxPt		The maximum extent of the AAB.
			 *	@param		seed		If the seed is zero, the global seed will be used otherwise
			 *							the particular seed will be used.
			 */
			AABBUniformPosGenerator( const Vector2 & minPt, const Vector2 & maxPt, int seed=0 );
			
			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		aabbGen		The generator to copy its values from.
			 */
			AABBUniformPosGenerator( const AABBUniformPosGenerator & aabbGen );

			/*!
			 *	@brief		Return a value based on the 2D float generation rules.
			 *
			 *	@return		A constat position uniformly samples from an AAB.
			 */
			virtual Vector2 getValue() const;
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValueConcurrent() const;

			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual Vec2DGenerator * copy() const;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const AABBUniformPosGenerator & gen );
		protected:
			/*!
			 *	@brief		The random selector for the x-position of the return value.
			 */
			UniformFloatGenerator	_xRand;
			
			/*!
			 *	@brief		The random selector for the y-position of the return value.
			 */
			UniformFloatGenerator	_yRand;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Generates a 2D float value uniformly distributed in an oriented box (OB).
		 */
		class MENGE_API OBBUniformPosGenerator : public Vec2DGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		minPt		The anchor point of the OB.
			 *	@param		size		The size of the OB.
			 *	@param		theta		The rotation around the achor point (in degrees) of the OB.
			 *	@param		seed		If the seed is zero, the global seed will be used otherwise
			 *							the particular seed will be used.
			 */
			OBBUniformPosGenerator( const Vector2 & minPt, const Vector2 & size, float theta, int seed=0 );
			
			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		obbGen		The generator to copy its values from.
			 */
			OBBUniformPosGenerator( const OBBUniformPosGenerator & obbGen );

			/*!
			 *	@brief		Return a value based on the 2D float generation rules.
			 *
			 *	@return		A constat position uniformly samples from an OB.
			 */
			virtual Vector2 getValue() const;
			
			/*!
			 *	@brief		Return a value based on the 2D float generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A 2D float value.
			 */
			virtual Vector2 getValueConcurrent() const;

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;

			/*! 
			 *	@brief		Create a copy of itself
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual Vec2DGenerator * copy() const;

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const OBBUniformPosGenerator & gen );

		protected:
			/*!
			 *	@brief		The random selector for the position of the return value along the *width* of the OB.
			 */
			UniformFloatGenerator	_xRand;

			/*!
			 *	@brief		The random selector for the position of the return value along the *height* of the OB.
			 */
			UniformFloatGenerator	_yRand;

			/*!
			 *	@brief		The anchor point of the OB.
			 */
			Vector2	_minPt;
			
			/*!
			 *	@brief		The cosine of the OB's rotation.
			 */
			float	_cosTheta;
			
			/*!
			 *	@brief		The sine of the OB's rotation.
			 */
			float	_sinTheta;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A weighted integer value.  Used with WeightedIntGenerator.
		 */
		class MENGE_API WeightedInt {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		value		The integer value.
			 *	@param		weight		The relative weight associated with the value.
			 */
			WeightedInt( int value, float weight): _val(value), _wt( weight ){}

			/*!
			 *	@brief		The value of the entry.
			 */
			int _val;

			/*!
			 *	@brief		The weight of the value.
			 */
			float _wt;

			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		wInt	An instance of the weighted int to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const WeightedInt & wInt );
		};

		///////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A number generator based on a weighted probability of a discrete value set.
		 *
		 *	Each value in the set is associated with a weight.  The relative probability of any
		 *	given value is that value's weight, divided by the summed weight of all values.
		 */
		class MENGE_API WeightedIntGenerator : public IntGenerator {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			WeightedIntGenerator();

			/*!
			 *	@brief		Copy constructor.
			 *
			 *	The constructed copy should not share the same seed as the original.
			 *
			 *	@param		gen		The generator to copy.
			 */
			WeightedIntGenerator( const WeightedIntGenerator & gen);

			/*!
			 *	@brief		Return a value based on the integer generation rules.
			 *
			 *	@return		An integer value drawn from a set with weighted probabilities.
			 */
			virtual int getValue() const;
			
			/*!
			 *	@brief		Return a value based on the integer generation rules - performed
			 *				in a thread-safe manner.
			 *
			 *	This is the basic functionality that must be overwridden by derived classes.
			 *
			 *	@return		A integer value.
			 */
			virtual int getValueConcurrent() const;

			/*!
			 *	@brief		Add a value to the set.
			 *
			 *	@param		value		The value to add to the set.
			 *	@param		weight		The weight of the corresponding value.
			 */
			void addValue( int value, float weight );

			/*!
			 *	@brief		Function for converting the generator to a string on a output stream.
			 *
			 *	@param		out		The output stream to write the string representation to.
			 */
			virtual void print( Logger & out ) const;
			
			/*! 
			 *	@brief		Create a copy of itself.
			 *
			 *	This should only be called after the generator has been finalized.
			 *	Otherwise, the copy will need to be finalized as well.
			 *
			 *	@returns	A pointer to a new generator which is a copy of this one.
			 *				The caller of this function is responsible for freeing up the
			 *				memory for the copy.
			 */
			virtual IntGenerator * copy() const;	
			
			/*!
			 *	@brief		Friend function for writing string representation to an output stream
			 *
			 *	@param		out		The output stream.
			 *	@param		gen		An instance of the generator to represent as a string.
			 *	@returns	Reference to the input output stream.
			 */
			friend Logger & operator<<( Logger & out, const WeightedIntGenerator & gen );
			
			/*!
			 *	@brief		Finalizes the generator so that it can generate values.
			 *
			 *	When finished adding points, call this function to post-process them.
			 *	Calling WeightedIntGenerator::getValue before calling this will lead to
			 *	unpredictable results.  Furthermore, calling WeightedIntGenerator::addValue
			 *	after calling finalize will likewise lead to unpredictable results.
			 */
			void finalize();
		protected:

			/*!
			 *	@brief		A uniform float generator for "rolling the dice".
			 */
			UniformFloatGenerator _dice;

			/*!
			 *	@brief		The weighted values to select from.
			 */
			std::vector< WeightedInt >	_pairs;

			/*!
			 *	@brief		The lock for guaranteeing threadsafe random number generation.
			 */
			mutable SimpleLock	_lock;
		};

		/*!
		 *	@brief		Creates a float generator from an xml node.
		 *
		 *	This function does not maintain responsibility for freeing up the object.
		 *	The calling function is responsible for the memory.
		 *
		 *	@param		node		The tiny XML node containing the definition of a scalar float generator.
		 *	@param		scale		Optional argument for changing the units of the input value.
		 *							(e.g., specification commonly describes angles as degrees, 
		 *							but internal representation is in radians).
		 *	@param		prefix		An optional prefix which may be affixed to the float generator keywords.
		 *	@returns	A pointer to a float generator, consistent with the XML definition.
		 *				If there is an error in the definition, NULL is returned.
		 */
		MENGE_API FloatGenerator * createFloatGenerator( TiXmlElement * node, float scale=1.f, const std::string & prefix="" );

		/*!
		 *	@brief		Creates an int generator from an xml node.
		 *
		 *	This function does not maintain responsibility for freeing up the object.
		 *	The calling function is responsible for the memory.
		 *
		 *	@param		node		The tiny XML node containing the definition of a scalar float generator.
		 *	@param		prefix		An optional prefix which may be affixed to the float generator keywords.
		 *	@returns	A pointer to an int generator, consistent with the XML definition.
		 *				If there is an error in the definition, NULL is returned.
		 */
		MENGE_API IntGenerator * createIntGenerator( TiXmlElement * node, const std::string & prefix="" );

		/*!
		 *	@brief		Creates a 2D vector generator from an xml node
		 *
		 *	This function does not maintain responsibility for freeing up the object.
		 *	The calling function is responsible for the memory.
		 *
		 *	@param		node		The tiny XML node containing the definition of a vector float generator.
		 *	@param		scale		Optional argument for changing the units of the input value.
		 *							(e.g., specification commonly describes angles as degrees, 
		 *							but internal representation is in radians).
		 *	@returns	A pointer to a vector generator, consistent with the XML definition.
		 *				If there is an error in the definition, NULL is returned.
		 */
		MENGE_API Vec2DGenerator * create2DGenerator( TiXmlElement * node, float scale=1.f );

		
	}	// namespace Math
}	// namespace Menge
#endif	// __RAND_GENERATOR_H__

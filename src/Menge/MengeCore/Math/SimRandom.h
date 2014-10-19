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
 *	@file		SimRandom.h
 *	@brief		Functions for calculating normally distributed values.
 *
 *	This code taken from John Burkardt (see normal.cpp).
 */

#ifndef __SIM_RANDOM_H__
#define __SIM_RANDOM_H__

namespace Menge {

	namespace Math {
		/*!
		 *	@brief		Generates a quasi-thread-safe, normally distributed random number.
		 *
		 *	This algorithm generates two normally distributed values at a time. 
		 *	To be thread-safe, it could simply dismiss every second value.  Alternatively,
		 *	we can provide both values an place the burden on the caller to handle both.
		 *
		 *	@param		a		The mean of the probability distribution field (pdf).
		 *	@param		b		The standard deviation of the pdf.
		 *	@param		val1	The first normally distributed value generated.
		 *	@param		val2	The second normally distributed value generated.
		 *	@param		seed	A seed for the random number generator.
		 *						The value in this integer changes with every call.
		 */
		void r4_normalR ( float a, float b, float & val1, float & val2, int *seed );	

		/*!
		 *	@brief		The Box-Muller method for computing two normally distributed
		 *				values at the same time (with mean of 0 and standard deviation of 1.
		 *
		 *	@param		val1		The first normally distributed value generated.
		 *	@param		val2		The second normally distributed value generated.
		 *	@param		seed		A seed for the random number generator.
		 *	@returns	The normally distributed number.
		 */
		void r4_normal_01R ( float & val1, float & val2, int *seed );

		/*!
		 *	@brief		A uniformly generated random number in the range [0, 1]
		 *
		 *	@param		seed		A seed for the random number generator.
		 *	@returns	The uniformly distributed number.
		 */
		float r4_uniform_01 ( int *seed );

	}	// namespace Math
}	// namespace Menge
#endif	//__SIM_RANDOM_H__

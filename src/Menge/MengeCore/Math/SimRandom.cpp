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

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "Logger.h"
#include "SimRandom.h"
#include "consts.h"

namespace Menge {

	namespace Math {

		/*
		//	Functions r4_normalR and r4_normal_01R are derived from John Burkardt's
		//	code (with the given licensing information).  They have been modified
		//	to remove the static members so that they could be used in a thread-safe
		//	re-entrant manner (by placing the burden on the caller to account for
		//	the fact that the normal generator produces two values at a time.
		//
		//	Modified: 05 October 2013, Sean Curtis
		//
		//	Original credit
		//  Licensing:
		//    This code is distributed under the GNU LGPL license.
		//
		//  Modified:
		//    29 June 2006
		//
		//  Author:
		//    John Burkardt
		//
		*/
		void r4_normalR ( float a, float b, float & val1, float & val2, int *seed ) {
			r4_normal_01R( val1, val2, seed );

			val1 = a + b * val1;
			val2 = a + b * val2;
		}

		//****************************************************************************80

		void r4_normal_01R( float & val1, float & val2, int *seed ) {
			float r1 = r4_uniform_01( seed );

			if ( r1 == 0.0 ) {
				logger << Logger::ERR_MSG << "\n";
				logger << Logger::ERR_MSG << "R4_NORMAL_01 - Fatal error!\n";
				logger << Logger::ERR_MSG << "  R4_UNIFORM_01 returned a value of 0.\n";
				exit ( 1 );
			}

			float r2 = r4_uniform_01 ( seed );

			val1 = sqrt ( - 2.0f * log ( r1 ) ) * cos ( 2.0f * PI * r2 );
			val2 = sqrt ( - 2.0f * log ( r1 ) ) * sin ( 2.0f * PI * r2 );
		}

		//****************************************************************************80

		float r4_uniform_01 ( int *seed )

		//****************************************************************************80
		//
		//  Purpose:
		//
		//    R4_UNIFORM_01 returns a unit pseudorandom R4.
		//
		//  Discussion:
		//
		//    This routine implements the recursion
		//
		//      seed = 16807 * seed mod ( 2**31 - 1 )
		//      r4_uniform_01 = seed / ( 2**31 - 1 )
		//
		//    The integer arithmetic never requires more than 32 bits,
		//    including a sign bit.
		//
		//    If the initial seed is 12345, then the first three computations are
		//
		//      Input     Output      R4_UNIFORM_01
		//      SEED      SEED
		//
		//         12345   207482415  0.096616
		//     207482415  1790989824  0.833995
		//    1790989824  2035175616  0.947702
		//
		//  Licensing:
		//
		//    This code is distributed under the GNU LGPL license.
		//
		//  Modified:
		//
		//    16 November 2004
		//
		//  Author:
		//
		//    John Burkardt
		//
		//  Reference:
		//
		//    Paul Bratley, Bennett Fox, Linus Schrage,
		//    A Guide to Simulation,
		//    Springer Verlag, pages 201-202, 1983.
		//
		//    Pierre L'Ecuyer,
		//    Random Number Generation,
		//    in Handbook of Simulation
		//    edited by Jerry Banks,
		//    Wiley Interscience, page 95, 1998.
		//
		//    Bennett Fox,
		//    Algorithm 647:
		//    Implementation and Relative Efficiency of Quasirandom
		//    Sequence Generators,
		//    ACM Transactions on Mathematical Software,
		//    Volume 12, Number 4, pages 362-376, 1986.
		//
		//    Peter Lewis, Allen Goodman, James Miller,
		//    A Pseudo-Random Number Generator for the System/360,
		//    IBM Systems Journal,
		//    Volume 8, pages 136-143, 1969.
		//
		//  Parameters:
		//
		//    Input/output, int *SEED, the "seed" value.  Normally, this
		//    value should not be 0.  On output, SEED has been updated.
		//
		//    Output, float R4_UNIFORM_01, a new pseudorandom variate, strictly between
		//    0 and 1.
		//
		{
		  int k;
		  float r;

		  k = *seed / 127773;

		  *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

		  if ( *seed < 0 )
		  {
			*seed = *seed + 2147483647;
		  }
		//
		//  Although SEED can be represented exactly as a 32 bit integer,
		//  it generally cannot be represented exactly as a 32 bit real number!
		//
		  r = ( float ) ( *seed ) * 4.656612875E-10f;

		  return r;
		}
	}	 // namespace Math
}	// namespace Menge
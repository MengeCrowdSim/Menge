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
 *	@file		CoreConfig.h
 *	@brief		Sets up the proper compiler directives for 
 *				platform and dll export/import
 */

#ifndef __CORE_CONFIG_H__
#define	__CORE_CONFIG_H__

#if defined(_MSC_VER)
    // Exposing templated instances in a dll requires special handling
	// There is no problem as long as all code is built on the same machine.
	// Until this is finally corrected, we will simply suppress the warning.
	//
	// TODO: Correct the problem and remove the warning suppression.
    #pragma warning(disable:4251)
#endif

//// First determine compiling environment
#if defined( WIN32 ) || defined( _WIN32 )
  /*!
   * @brief defines the build environment as windows
   */
  #define PLUGIN_WIN32 1
#else
  /*!
   * @brief defines the build environment as linux
   */
  #define PLUGIN_LINUX 1
#endif

// Now set up compiler-dependent export/import symbols

#if defined(_MSC_VER)
  #if defined( MENGE_STATICLIB )
    #define MENGE_API
	#define MATHEXTERN 
  #else
    #if defined( MENGECORE_EXPORTS )
      // We are building the DLL, export the symbols tagged like this
      #define MENGE_API __declspec(dllexport)
	  #define MATHEXTERN 
    #else
      // If we are consuming the DLL, import the symbols tagged like this
      #define MENGE_API __declspec(dllimport)
	  #define MATHEXTERN extern
    #endif
  #endif

#elif defined(__GNUC__)
 
  #define MATHEXTERN 
  #if defined( MENGE_STATICLIB )
    #define MENGE_API
  #else
    #if defined( MENGECORE_EXPORTS )
      #define MENGE_API __attribute__ ((visibility ("default")))
    #else
      // If you use -fvisibility=hidden in GCC, exception handling and RTTI
      // would break if visibility wasn't set during export _and_ import
      // because GCC would immediately forget all type infos encountered.
      // See http://gcc.gnu.org/wiki/Visibility
      #define MENGE_API __attribute__ ((visibility ("default")))
    #endif
  #endif

#else

  #error Unknown compiler, please implement shared library macros

#endif	// Compiler-detection

#endif	// __CORE_CONFIG_H__

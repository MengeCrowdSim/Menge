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
 *	@file		Profiler.h
 *	@brief		Functionality for timing and profiling the program
 */

#ifndef __PROFILER_H__
#define __PROFILER_H__

#define TIME_CROWD
#include <string>

#ifdef _WIN32
#include "windows.h"
#else	// _WIN32
#include <ctime>
#include "include/macros.h"
#endif	// _WIN32

namespace Menge {

	namespace Vis {
		/*!
		 *	@brief		Basic timer.
		 */
		class Timer {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			Timer();

			/*! 
			 *	@brief	Starts the timer running.
			 */
			void start();

			/*!
			 *	@brief		Reports the time elapsed between this call and the last start.
			 *
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The elapsed time of the timer's last call (scaled by the given amount).
			 */
			float elapsed( float scale );

		protected:
		#ifdef _WIN32
			/*!
			 *	@brief		The computer's clock frequency - used for converting cycles to seconds
			 */
			static __int64 FREQ;

			/*!
			 *	@brief		The time (in clock cycles) at which the timer was started.
			 */
			__int64 _start;

		#else	// _WIN32
			/*!
			 *	@brief		The time (in clock cycles) at which the timer was started.
			 */
			struct timespec _start;

		#endif	// _WIN32
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Lap timer.  A timer which supports "laps"
		 *				i.e., single calls which measure from the last "tick"
		 *				to this tick.
		 */
		class LapTimer: public Timer  {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			LapTimer();

			/*!
			 *	@brief		Reports the time elapsed from the previous call to lap() or start() to
			 *				this call.  The clock is still "running" and the next lap starts.
			 *
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The time elapsed (for units see "scale").
			 */
			float lap( float scale=1.f );

			/*!
			 *	@brief		Reports the average lap time across all recorded laps.
			 *
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The time elapsed (for units see "scale").
			 */
			float average( float scale=1.f );

			/*!
			 *	@brief		Reports the number of calls to laps.
			 */
			inline int	laps() const { return _lapCount; }

		protected:
		#ifdef _WIN32
			/*!
			 *	@brief		The total accrued time of timed intervals (in cycles).
			 */
			__int64	_total;
		#else	// _WIN32
			/*!
			 *	@brief		The total accrued time of timed intervals (in seconds).
			 */
			double	_total;
		#endif	// _WIN32

			/*!
			 *	@brief		The total number of calls to lap()
			 */
			int _lapCount;
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A timer which uses a cache of values to only update its values
		 *				every N calls to lap/elapsed.  Useful for displaying frame rate.
		 */
		class SampleTimer: public Timer {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		sampleCount		The number of repeated calls to lap to cause the
			 *								timer to report a new, average value.
			 */
			SampleTimer( int sampleCount );

			/*!
			 *	@brief		Reports the average elapsed time of the last N calls to lap
			 *
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The time elapsed (for units see "scale").
			 */
			float lap( float scale=1.f );

		protected:
			/*!
			 *	@brief		The number of samples to compute the average over
			 */
			int	_totalSamples;

			/*! 
			 *	@brief		The curren total number of calls to lap.
			 */
			int _currSample;

		#ifdef _WIN32
			/*! 
			 *	@brief		The current accrual of time for the current cache (in cycles).
			 */
			__int64	_total;
		#else	// _WIN32
			/*!
			*	@brief		The current accrual of time for the current cache (in seconds).
			 */
			float	_total;
		#endif	// _WIN32

			/*!
			 *	@brief		The most recently defined elapsed lap time.
			 */
			float _cached;
		};

	}	// namespace Vis

	#ifdef TIME_CROWD

	/*!
	 *	@brief		Creates a lap timer which uses the given label for display.
	 *	
	 *	@param		displayString		The string to display when reporting the profiling
	 *									results.
	 *	@returns	The identifier for the created timer.
	 */	
	size_t addTimer( const std::string & displayString );

	/*!
	 *	@brief		Starts the timer with the given identifier.
	 *
	 *	@param		index		The timer identifier supplied by addTimer
	 */
	void startTimer( size_t index );

	/*!
	 *	@brief		Stops the timer with the given identifier.
	 *
	 *	@param		index		The timer identifier supplied by addTimer
	 */
	void stopTimer( size_t index );

	/*!
	 *	@brief		Lap the ith timer
	 *
	 *	@param		index		The timer identifier supplied by addTimer
	 */
	void lapTimer( size_t index );

	/*!
	 *	@brief		Reports the average time of the ith timer
	 *
	 *	@param		index		The timer identifier supplied by addTimer
	 *	@returns	The average time of all laps.
	 */
	float averageTime( size_t index );

	/*!
	 *	@brief		Prints the average times for all timers, displayed with the
	 *				accompanying messages and set units.
	 */
	void printAverages();

	/*!
	 *	@brief		Sets the internal units of the profiler.
	 *
	 *	@param		scale		The scale of the units to report the elapsed time in.
	 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
	 *	@param		unitString	The units which accompany the given scale -- for printing purposes.
	 */	 
	void setUnits( float scale, const std::string & unitString );

	#else		// not defined TIME_CROWD

	#define addTimer( displayString ) (0)
	#define startTimer( index )
	#define stopTimer( index )
	#define lapTimer( index )
	#define averageTime( index )
	#define printAverages()
	#define setUnits( scale, unitString )

	#endif		// TIME_CROWD

}	// namespace Menge

#endif		// __PROFILER_H__

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

#include "Profiler.h"

#include <cassert>
#include <vector>
#include <iostream>

namespace Menge {

	namespace Vis {

		////////////////////////////////////////////////////////////////////////////
		//                IMPLEMENTATION for Timer
		////////////////////////////////////////////////////////////////////////////

		#ifdef _WIN32

		__int64 Timer::FREQ = 0;

		////////////////////////////////////////////////////////////////////////////

		Timer::Timer(): _start(0) {
			if ( FREQ == 0 ) {
				::QueryPerformanceFrequency( (LARGE_INTEGER*)&FREQ );
			}
		}

		////////////////////////////////////////////////////////////////////////////

		void Timer::start() {
			::QueryPerformanceCounter( (LARGE_INTEGER*) &_start );
		}

		////////////////////////////////////////////////////////////////////////////

		float Timer::elapsed( float scale ) {
			__int64 t;
			::QueryPerformanceCounter( (LARGE_INTEGER*) &t );
			__int64 e = t - _start;
			_start = t;
			return (float)( e ) / (float)(FREQ) * scale;
		}

		////////////////////////////////////////////////////////////////////////////

		#else

		Timer::Timer() {
		}

		////////////////////////////////////////////////////////////////////////////

		void Timer::start() {
			clock_gettime( CLOCK_REALTIME, &_start );
		}

		////////////////////////////////////////////////////////////////////////////

		float Timer::elapsed( float scale ) {
			struct timespec t;
			clock_gettime( CLOCK_REALTIME, &t );
			return ( ( t.tv_sec - _start.tv_sec ) + ( t.tv_nsec - _start.tv_nsec ) * 1e-9f ) * scale;
		}

		#endif

		////////////////////////////////////////////////////////////////////////////
		//                IMPLEMENTATION for LapTimer
		////////////////////////////////////////////////////////////////////////////

		LapTimer::LapTimer(): Timer(), _total(0), _lapCount(0) {
		}

		////////////////////////////////////////////////////////////////////////////

		#ifdef _WIN32

		float LapTimer::lap( float scale ) {
			__int64 t;
			::QueryPerformanceCounter( (LARGE_INTEGER*) &t );
			__int64 e = t - _start;
			_start = t;
			_total += e;
			++_lapCount;
			return (float)( e ) / (float)(FREQ) * scale;
		}

		////////////////////////////////////////////////////////////////////////////

		float LapTimer::average( float scale ) {
			return (float)_total / (float)_lapCount / (float)(FREQ) * scale ;
		}

		////////////////////////////////////////////////////////////////////////////

		#else

		////////////////////////////////////////////////////////////////////////////
			
		float LapTimer::lap( float scale ) {
			struct timespec t;
			clock_gettime( CLOCK_REALTIME, &t );
			float e = ( t.tv_sec - _start.tv_sec ) + ( t.tv_nsec - _start.tv_nsec ) * 1e-9f;
			_start = t;
			_total += e;
			++_lapCount;
			return e * scale;
		}
		////////////////////////////////////////////////////////////////////////////

		float LapTimer::average( float scale ) {
			return (float)_total / (float)_lapCount * scale ;
		}

		////////////////////////////////////////////////////////////////////////////

		#endif

		////////////////////////////////////////////////////////////////////////////
		//                IMPLEMENTATION for SampleTimer
		////////////////////////////////////////////////////////////////////////////

		SampleTimer::SampleTimer( int sampleCount ): Timer(), _totalSamples(sampleCount), _currSample(0),
		_total(0), _cached(0.f) {}

		////////////////////////////////////////////////////////////////////////////

		#ifdef _WIN32

		float SampleTimer::lap( float scale ) {
			__int64 t;
			::QueryPerformanceCounter( (LARGE_INTEGER*) &t );
			_total += t - _start;
			++_currSample;
			if ( _currSample == _totalSamples ) {
				_cached = (float)_total / ( (float)_totalSamples * (float)FREQ );
				_currSample = 0;
				_total = 0;
			}
			_start = t;
			return _cached * scale;
		}

		#else

		float SampleTimer::lap( float scale ) {
			struct timespec t;
			clock_gettime( CLOCK_REALTIME, &t );
			_total += ( t.tv_sec - _start.tv_sec ) + ( t.tv_nsec - _start.tv_nsec ) * 1e-9f;
			++_currSample;
			if ( _currSample == _totalSamples ) {
				_cached = _total / (float)_totalSamples;
				_currSample = 0;
				_total = 0;
			}
			_start = t;
			return _cached * scale;
		}

		#endif

		////////////////////////////////////////////////////////////////////////////

		#ifdef TIME_CROWD

		////////////////////////////////////////////////////////////////////////////
		//                IMPLEMENTATION for Profiler
		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Class for storing sets of timers for profiling aspects of
		 *				the simulation.
		 */
		class Profiler {
			/*!
			 *	@brief		Pointer for singleton instance.
			 */
			static Profiler * PROFILER;

			/*!
			 *	@brief		Private constructor.
			 */
			Profiler() {}
			
			////////////////////////////////////////////////////////////////////////////
		public:
			/*!
			 *	@brief		Returns a pointer to the singleton instance, creating it as
			 *				necessary.
			 *
			 *	@returns	A pointer to the single instance of profiler
			 */
			static Profiler * getInstance() {
				if ( PROFILER == 0x0 ) {
					PROFILER = new Profiler();
				}
				return PROFILER;
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Creates a lap timer which uses the given label for display.
			 *	
			 *	@param		label		The string to display when reporting the profiling
			 *							results.
			 *	@returns	The identifier for the created timer.
			 */
			size_t addTimer( const ::std::string & label ) {
				assert( _timers.size() == _dispStrings.size() && "Mis-match in timer and display strings" );
				size_t id = _timers.size();
				_timers.push_back( LapTimer() );
				_dispStrings.push_back( label );
				return id;
			}

			////////////////////////////////////////////////////////////////////////////
			
			/*!
			 *	@brief		Reports the number of timers
			 */
			size_t	timerCount() const { return _timers.size(); }

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Starts the ith timer.
			 *
			 *	@param		i			The identifier for the timer.  Only checked in debug mode.
			 */
			void start( size_t i ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::start()" ); 
				_timers[i].start();
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Reports the time elapsed between this call and the last start for the
			 *				ith timer.
			 *
			 *	@param		i			The identifier for the timer.  Only checked in debug mode.
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The elapsed time of the ith timer's last call (scaled by the given amount).
			 */
			float elapsed( size_t i, float scale ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::elapsed()" ); 
				return _timers[i].elapsed( scale );
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Reports the time elapsed from the previous call to lap() or start() to
			 *				this call for the ith timer.  The clock is still "running" and the next 
			 *				lap starts.
			 *
			 *	@param		i			The identifier for the timer.  Only checked in debug mode.
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The time elapsed (for units see "scale").
			 */
			float lap( size_t i, float scale ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::lap()" ); 
				return _timers[i].lap( scale );
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Reports the average lap time across all recorded laps for the ith timer.
			 *
			 *	@param		i			The identifier for the timer.  Only checked in debug mode.
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@returns	The time elapsed (for units see "scale").
			 */
			float average( size_t i, float scale ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::average()" ); 
				return _timers[i].average( scale );
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Reports the number of laps the ith counter has had.
			 *
			 *	@param		i			The identifier for the timer.  Only checked in debug mode.
			 *	@returns	The number of laps.
			 */
			int laps( size_t i ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::laps()" ); 
				return _timers[i].laps();
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Reports the average lap time across all recorded laps for the first "count" timers.
			 *
			 *	@param		count		The first count timers' average times will be reported.  
			 *							Only checked in debug mode.
			 *	@param		scale		The scale of the units to report the elapsed time in.
			 *							e.g., 1.0 --> seconds, 0.001 -->, 1e-6 --> microseconds.
			 *	@param		averages	A pointer to an array of floats sufficiently large to hold
			 *							count values.
			 */
			void averages( size_t count, float scale, float * averages ) {
				assert( count <= _timers.size() && "Timer index outside of valid range in Profiler::averages()" ); 
				for ( size_t i = 0; i < count; ++i ) {
					averages[ i ] = _timers[i].average( scale );
				}
			}

			////////////////////////////////////////////////////////////////////////////

			/*!
			 *	@brief		Returns the display string for the given LapTimer.
			 *
			 *	@param		i		The index of the desired timer -- only validated in debug mode.
			 *	@returns	The display string for the indicated LapTimer.
			 */
			const ::std::string & displayString( size_t i ) {
				assert( i < _timers.size() && "Timer index outside of valid range in Profiler::displayString()" ); 
				return _dispStrings[ i ];
			}

		private:
			/*!
			 *	@brief		A vector of timers used in profiling.
			 */
			::std::vector< LapTimer >	_timers;

			/*!
			 *	@brief		A vector of display strings for the timers.
			 */
			::std::vector< ::std::string > _dispStrings;
		};

		Profiler * Profiler::PROFILER = 0x0;
	}	// namespace Vis

	////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	float SCALE = 1000.0f;
	std::string SCALE_LABEL = " ms";

#endif

	////////////////////////////////////////////////////////////////////////////

	size_t addTimer( const std::string & displayString ) {
		return Vis::Profiler::getInstance()->addTimer( displayString );
	}

	////////////////////////////////////////////////////////////////////////////

	void startTimer( size_t index ) {
		Vis::Profiler::getInstance()->start( index );
	}

	////////////////////////////////////////////////////////////////////////////

	void stopTimer( size_t index ) {
		Vis::Profiler::getInstance()->lap( index, SCALE );
	}

	////////////////////////////////////////////////////////////////////////////

	void lapTimer( size_t index ) {
		Vis::Profiler::getInstance()->lap( index, SCALE );
	}

	////////////////////////////////////////////////////////////////////////////

	float averageTime( size_t index ) {
		Vis::Profiler * pro = Vis::Profiler::getInstance();
		return pro->average( index, SCALE );
	}

	////////////////////////////////////////////////////////////////////////////

	void printAverages() {
		Vis::Profiler* profiler = Vis::Profiler::getInstance();
		const size_t count = profiler->timerCount();
		for ( size_t i = 0; i < count; ++i ) {
			float time = profiler->average( i, SCALE );
			int laps = profiler->laps( i );
			::std::cout << profiler->displayString( i ) << "   (avg): " << time << SCALE_LABEL << " in " << laps << " laps\n";
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void setUnits( float scale, const std::string & unitString ) {
		SCALE = scale;
		SCALE_LABEL = unitString;
	}

	#endif // TIME_CROWD
}	// namespace Menge
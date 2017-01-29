#ifndef __VIS_ELEMENT_EXCEPTION_H__
#define __VIS_ELEMENT_EXCEPTION_H__

#include "MengeCore/MengeException.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {
	namespace Runtime {
		/*!
		 *	@brief		Exception base class for vis elements.
		 */
		class MENGEVIS_API VisElementException : public virtual Menge::MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VisElementException() : MengeException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VisElementException( const std::string & s ) : Menge::MengeException( s ) {}
		};

		/*!
		 *	@brief		The fatal vis element exception.
		 */
		class MENGEVIS_API VisElementFatalException : public VisElementException,
			public Menge::MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VisElementFatalException() : VisElementException(), Menge::MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VisElementFatalException( const std::string & s ) : VisElementException(s),
				Menge::MengeFatalException() {}
		};

	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __VIS_ELEMENT_EXCEPTION_H__

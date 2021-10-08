#ifndef LIB_RATSS_DEBUG_H
#define LIB_RATSS_DEBUG_H
#include <libratss/constants.h>
#include <mpreal/mpreal.h>
#include <gmpxx.h>

#if defined(LIB_RATSS_WITH_CGAL)
	#include <CGAL/CORE/BigRat.h>
	#include <CGAL/CORE/Expr.h>
#endif

#if defined(LIB_RATSS_WITH_CORE_TWO)
	#include <CORE_TWO.h>
#endif


#if defined(__clang__)
	#define NO_OPTIMIZE __attribute__((optnone))
#elif defined(__GNUC__) || defined(__GNUG__)
	#define NO_OPTIMIZE __attribute__((optimize(0)))
#elif defined(_MSC_VER)
	//what should be here?
	#define NO_OPTIMIZE __attribute__((optimize(0)))
#else
	//bailout?
	#define NO_OPTIMIZE __attribute__((optimize(0)))
#endif

#define NO_INLINE __attribute__ ((noinline))

#define FORCE_SYMBOL_INCLUSION __attribute__((used))

extern "C" {
	void ratss_debug_print_mpreal(const mpfr::mpreal & v) FORCE_SYMBOL_INCLUSION;
	void ratss_debug_print_mpq_class(const mpq_class & v) FORCE_SYMBOL_INCLUSION;
	void ratss_debug_print_mpz_class(const mpz_class & v) FORCE_SYMBOL_INCLUSION;
	void ratss_debug_print_mpf_class(const mpf_class & v) FORCE_SYMBOL_INCLUSION;
#if defined(LIB_RATSS_WITH_CGAL)
	void ratss_debug_print_CORE_BigFloat(CORE::BigFloat const & v) FORCE_SYMBOL_INCLUSION;
	void ratss_debug_print_CORE_Expr(CORE::Expr const & v) FORCE_SYMBOL_INCLUSION;
#endif
#if defined(LIB_RATSS_WITH_CORE_TWO)
	void ratss_debug_print_CORE_TWO_BigFloat(CORE_TWO::BigFloat const & v) FORCE_SYMBOL_INCLUSION;
	void ratss_debug_print_CORE_TWO_Expr(CORE_TWO::Expr const & v) FORCE_SYMBOL_INCLUSION;
#endif
}

namespace LIB_RATSS_NAMESPACE {
	void init_interactive_debuging();
}

#endif

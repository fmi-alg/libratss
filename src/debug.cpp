#include <libratss/debug.h>
#include <libratss/Conversion.h>
#include <iostream>

extern "C" {
	
void ratss_debug_print_mpreal(const mpfr::mpreal & v) {
	mpfr_exp_t exp;
	char * binstr = ::mpfr_get_str(0, &exp, 2, 0, v.mpfr_ptr(), MPFR_RNDZ);
	std::cerr << "value=" << v << " precision=" << v.get_prec() << " exponent=" << v.get_exp() << " binary=" <<  binstr << "E" << exp << std::endl;
	mpfr_free_str(binstr);
}

void ratss_debug_print_mpq_class(const mpq_class & v) {
	std::cerr << "value=" << v << "~" << v.get_d();
	std::cerr << " binary=" << v.get_str(2);
	std::cerr << " bits=" << mpz_sizeinbase(v.get_num().get_mpz_t(), 2) << "/"
							<< mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	std::cerr << std::endl;
}

void ratss_debug_print_mpz_class(const mpz_class & v) {
	std::cerr << "value=" << v << " bits=" << mpz_sizeinbase(v.get_mpz_t(), 2) << std::endl;
}

void ratss_debug_print_mpf_class(const mpf_class & v) {
	std::cerr << "value=" << v << " precision=" << v.get_prec() << " exp=" << v.get_ui() << std::endl;
}

#if defined(LIB_RATSS_WITH_CGAL)
void ratss_debug_print_CORE_BigFloat(CORE::BigFloat const & v) {
	std::cerr << "value=" << v << " m=" << mpz_class(v.m().get_mp()) << " exp=" << v.exp() << std::endl;
}

void ratss_debug_print_CORE_Expr(CORE::Expr const & v) {
	std::cerr << "value=" << v << std::endl;
}
#endif


#if defined(LIB_RATSS_WITH_CORE_TWO)
void ratss_debug_print_CORE_TWO_BigFloat(CORE_TWO::BigFloat const & v) {
	ratss_debug_print_mpreal(LIB_RATSS_NAMESPACE::convert<mpfr::mpreal>(v));
}

void ratss_debug_print_CORE_TWO_Expr(CORE_TWO::Expr const & v) {
	std::cerr << "value=" << v << std::endl;
}
#endif

}

namespace LIB_RATSS_NAMESPACE {
	
#define PRINT_FN_ADDR(__F) std::cout << #__F ": " << std::size_t(&__F) << std::endl;
void init_interactive_debuging() {
	std::cout << "Initializing interactive debugging functionality of ratss:\n";
	PRINT_FN_ADDR(ratss_debug_print_mpreal)
	PRINT_FN_ADDR(ratss_debug_print_mpq_class)
	PRINT_FN_ADDR(ratss_debug_print_mpz_class)
	PRINT_FN_ADDR(ratss_debug_print_mpf_class)
#if defined(LIB_RATSS_WITH_CGAL)
	PRINT_FN_ADDR(ratss_debug_print_CORE_BigFloat)
	PRINT_FN_ADDR(ratss_debug_print_CORE_Expr)
#endif
#if defined(LIB_RATSS_WITH_CORE_TWO)
	PRINT_FN_ADDR(ratss_debug_print_CORE_TWO_BigFloat)
	PRINT_FN_ADDR(ratss_debug_print_CORE_TWO_Expr)
#endif
	std::cout << std::endl;
}
#undef PRINT_FN_ADDR

}

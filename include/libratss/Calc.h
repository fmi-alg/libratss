#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/enum.h>
#include <libratss/Conversion.h>
#include <libratss/SimApxBruteForce.h>

#ifdef LIB_RATSS_WITH_FPLLL
	#include <libratss/SimApxLLL.h>
#endif

namespace LIB_RATSS_NAMESPACE {

class Calc {
public:
	using SnapType = LIB_RATSS_NAMESPACE::SnapType;
public:
	template<typename T_FT>
	inline T_FT add(const T_FT & a, const T_FT & b) const { return a+b; }

	template<typename T_FT>
	inline T_FT sub(const T_FT & a, const T_FT & b) const { return a-b; }

	template<typename T_FT>
	inline T_FT mult(const T_FT & a, const T_FT & b) const { return a*b; }
	
	template<typename T_FT>
	inline T_FT div(const T_FT & a, const T_FT & b) const { return a/b; }
public:
	mpfr::mpreal add(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal sub(const mpfr::mpreal & a, const mpfr::mpreal & b)  const;
	mpfr::mpreal mult(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal div(const mpfr::mpreal& a, const mpfr::mpreal& b) const;
public:
	mpfr::mpreal sin(const mpfr::mpreal & v) const;
	mpfr::mpreal asin(const mpfr::mpreal & v) const;

	mpfr::mpreal cos(const mpfr::mpreal & v) const;
	mpfr::mpreal acos(const mpfr::mpreal & v) const;
	
	mpfr::mpreal tan(const mpfr::mpreal & v) const;
	mpfr::mpreal atan(const mpfr::mpreal & v) const;
	
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b) const;
	mpfr::mpreal squaredDistance(const mpfr::mpreal & a, const mpfr::mpreal & b, const mpfr::mpreal & c) const;
	mpfr::mpreal sq(const mpfr::mpreal & v) const;
	mpfr::mpreal sqrt(const mpfr::mpreal & v) const;
public:
	int msb(const mpz_class & v) const;
public:
	///create a fixpoint number with abs(p) < 1
	///hence abs(v) < 1
	///everthing above is clipped to infinity
	mpfr::mpreal toFixpoint(const mpfr::mpreal & v, int significands = -1) const;
#if defined(LIB_RATSS_WITH_CGAL)
	///create a fixpoint number with abs(p) < 1
	///hence abs(v) < 1
	///everything above is an error!
	CORE::BigFloat toFixpoint(CORE::BigFloat const & v, int significands = -1) const;
#endif
#if defined(LIB_RATSS_WITH_CORE_TWO)
	CORE_TWO::BigFloat toFixpoint(CORE_TWO::BigFloat const & v, int significands = -1) const;
#endif
public:
	template<typename T_INPUT_ITERATOR>
	mpfr::mpreal squaredLength(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end) const;
	///input and output may point to the same storage
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void normalize(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out) const;
public:
	template<typename T_ITERATOR>
	std::size_t summedDenomSize(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR>
	std::size_t maxDenom(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR>
	std::size_t limbCount(T_ITERATOR begin, const T_ITERATOR& end) const;
	template<typename T_ITERATOR_1, typename T_ITERATOR_2>
	mpq_class squaredDistance(T_ITERATOR_1 begin1, const T_ITERATOR_1 & end1, T_ITERATOR_2 begin2) const;
	template<typename T_ITERATOR_1, typename T_ITERATOR_2>
	mpq_class maxNorm(T_ITERATOR_1 begin1, const T_ITERATOR_1 & end1, T_ITERATOR_2 begin2) const;
public:
	///@return r a number satisfying the following conditions:
	/// r is a fraction with the smallest denominator such that lower <= r <= upper
	mpq_class within(const mpq_class& lower, const mpq_class& upper) const;
	
	mpq_class contFrac(const mpq_class& value, int significands, int mode) const;
	
	void jacobiPerron2D(const mpq_class& input1, const mpq_class& input2, mpq_class& output1, mpq_class& output2, int significands, int snapTypeGuarantee) const;
	
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void apply_common_denominator(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const mpz_class & common_denom) const;
	///this will first set common_denom and the write all numerators to out
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const;
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, mpq_class epsilon) const;
	
	///In the following v_real is the real unkown value of v and v is an approimation thereof
	///For ST_CF this computes a snapped rational r with |r - v_real| <= 3/4*2^-significands and r.den <= 2^(significands+2)
	///For ST_FX|ST_FL this computes a snapped rational r with |r - v_real| <= 2^-significands
	///ST_FX also guarantees r.den <= 2^significands
	mpq_class snap(const mpfr::mpreal & v, int st, int significands = -1) const;
	mpq_class snap(const mpq_class & v, int st, int significands = -1) const;
	mpq_class snap(const mpq_class & v, int st, const mpq_class & eps) const;
public:
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
public:
	std::size_t maxBitCount(const mpq_class &v) const;
	std::size_t numBits(const mpz_class &v) const;
};

}//end namespace LIB_RATSS_NAMESPACE

//definitions

namespace LIB_RATSS_NAMESPACE {
	

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::apply_common_denominator(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const mpz_class & common_denom) const {
	assert(common_denom > 0);
	using InputIterator = T_INPUT_ITERATOR;
	using input_type = typename std::iterator_traits<InputIterator>::value_type;
	for(auto it(begin); it != end; ++it, ++out) {
		auto x = Conversion<input_type>::toMpq(*it);
		mpz_class num = (x.get_num() * common_denom) / x.get_den();
		mpz_class num1 = num+(x >= 0 ? 1 : -1);
		//check which one is better
		mpq_class pq(num, common_denom);
		mpq_class pq1(num1, common_denom);
		pq.canonicalize();
		pq1.canonicalize();
		if (abs(x - pq) < abs(x - pq1)) {
			*out = num;
		}
		else {
			*out = num1;
		}
	}
}

// #define LIBRATSS_DEBUG_LLL_VERBOSE

///See Siam Journal on Computing: THE COMPUTATIONAL COMPLEXITY OF SIMULTANEOUS DIOPHANTINE APPROXIMATION PROBLEMS by J. C. LAGARIAS
///A short description can be found in Gathen, Joachim & Gerhard, Jürgen. (2003). Modern computer algebra (2. ed.) Chapter 17.3 Simultaneous Diophantine approximation
///We choose our Q such that the algorithm guarantees the target epsilon
///
#ifdef LIB_RATSS_WITH_FPLLL
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const {
	SimApxLLL<T_INPUT_ITERATOR> worker(begin, end, significands);
	common_denom = worker.run(true);
	for(auto it(worker.numerators_begin()); it != worker.numerators_end(); ++it, ++out) {
		*out = std::move(*it);
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, mpq_class eps) const {
	SimApxLLL<T_INPUT_ITERATOR> worker(begin, end, eps);
	common_denom = worker.run(true);
	for(auto it(worker.numerators_begin()); it != worker.numerators_end(); ++it, ++out) {
		*out = std::move(*it);
	}
}
#else
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR, T_INPUT_ITERATOR, T_OUTPUT_ITERATOR, mpz_class &, int) const {
	throw std::runtime_error("libratss was compiled without snapping using the lll algorithm");
}
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR, T_INPUT_ITERATOR, T_OUTPUT_ITERATOR, mpz_class &, mpq_class) const {
	throw std::runtime_error("libratss was compiled without snapping using the lll algorithm");
}
#endif


template<typename T_INPUT_ITERATOR>
mpfr::mpreal Calc::squaredLength(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end) const {
	mpfr::mpreal tmp(0);
	for(; begin != end; ++begin) {
		tmp = add(sq(*begin), tmp);
	}
	return tmp;
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::normalize(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out) const {
	mpfr::mpreal tmp = sqrt( squaredLength(begin, end) );
	for(; begin != end; ++begin, ++out) {
		*out = div(*begin, tmp);
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands) const {
	using input_ft = typename std::iterator_traits<T_INPUT_ITERATOR>::value_type;
	if (snapType & ST_JP) {
		using std::distance;
		if (distance(begin, end) != 2) {
			throw std::domain_error("ratss::Calc::toRational: Snapping with jacobiPerron only supports dimension 2");
		}
		mpq_class input1( convert<mpq_class>(*begin) );
		++begin;
		mpq_class input2( convert<mpq_class>(*begin) );
		mpq_class output1, output2;
		jacobiPerron2D(input1, input2, output1, output2, significands+1, snapType & ST_GUARANTEE_MASK);
		*out = output1;
		++out;
		*out = output2;
		++out;
	}
	else if (snapType & (ST_FPLLL_MASK)) {
		#if defined(LIB_RATSS_WITH_FPLLL)
			using std::distance;
			std::vector<mpq_class> tmp;
			
			for(auto it(begin); it != end; ++it) {
				tmp.emplace_back( convert<mpq_class>(*it) );
			}
			
			SimApxLLL<std::vector<mpq_class>::const_iterator> sapx(tmp.cbegin(), tmp.cend(), significands+1);
			sapx.run(SnapType(snapType & ST_SNAP_TYPES_MASK));
			
			for(auto it(sapx.numerators_begin()); it != sapx.numerators_end(); ++it) {
				mpq_class pq(*it, sapx.denominator());
				pq.canonicalize();
				*out = pq;
				++out;
			}
		#else
			throw std::runtime_error("libratss::Calc::toRational: libratss was built without support for fplll");
		#endif
	}
	else if (snapType & (ST_BRUTE_FORCE)) {
			using std::distance;
			std::vector<mpq_class> tmp;
			
			for(auto it(begin); it != end; ++it) {
				tmp.emplace_back( snap(*it, ST_FX, significands+2) );
			}
			
			SimApxBruteForce<mpq_class,0> sapx(tmp.cbegin(), tmp.cend());
			sapx.run(significands+1);
			
			for(auto it(sapx.numerators_begin()); it != sapx.numerators_end(); ++it, ++out) {
				*out = *it/sapx.denominator();
			}
	}
	else {
		std::transform(
			begin,
			end,
			out,
			[this, snapType, significands](const input_ft & v) {
				return snap(v, snapType, significands);
			}
		);
	}
}


template<typename T_ITERATOR>
std::size_t Calc::summedDenomSize(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result += mpz_sizeinbase(begin->get_den().get_mpz_t(), 2);
	}
	return result;
}

template<typename T_ITERATOR>
std::size_t Calc::maxDenom(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result = std::max<std::size_t>(result, mpz_sizeinbase(begin->get_den().get_mpz_t(), 2));
	}
	return result;
}

template<typename T_ITERATOR>
std::size_t Calc::limbCount(T_ITERATOR begin, const T_ITERATOR & end) const {
	std::size_t result = 0;
	for(; begin != end; ++begin) {
		result += __GMPXX_BITS_TO_LIMBS(mpz_sizeinbase(begin->get_num().get_mpz_t(), 2));
		result += __GMPXX_BITS_TO_LIMBS(mpz_sizeinbase(begin->get_den().get_mpz_t(), 2));
	}
	return result;
}


template<typename T_ITERATOR_1, typename T_ITERATOR_2>
mpq_class Calc::squaredDistance(T_ITERATOR_1 begin1, const T_ITERATOR_1 & end1, T_ITERATOR_2 begin2) const {
	using value_type1 = typename std::iterator_traits<T_ITERATOR_1>::value_type;
	using value_type2 = typename std::iterator_traits<T_ITERATOR_2>::value_type;
	mpq_class sq = 0;
	for(; begin1 != end1; ++begin1, ++begin2) {
		mpq_class tmp = Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2);
		sq += tmp*tmp;
	}
	return sq;
}

template<typename T_ITERATOR_1, typename T_ITERATOR_2>
mpq_class Calc::maxNorm(T_ITERATOR_1 begin1, const T_ITERATOR_1 & end1, T_ITERATOR_2 begin2) const {
	using value_type1 = typename std::iterator_traits<T_ITERATOR_1>::value_type;
	using value_type2 = typename std::iterator_traits<T_ITERATOR_2>::value_type;
	using std::abs;
	using std::max;
	using std::distance;
	
	if (distance(begin1, end1) == 0) {
		return mpq_class(0);
	}
	
	mpq_class result = abs(Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2));
	for(++begin1, ++begin2; begin1 != end1; ++begin1, ++begin2) {
		mpq_class tmp = abs(Conversion<value_type1>::toMpq(*begin1) - Conversion<value_type2>::toMpq(*begin2));
		result = max(result, tmp);
	}
	return result;
}

}//end namespace LIB_RATSS_NAMESPACE

#endif

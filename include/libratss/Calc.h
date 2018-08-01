#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

#ifdef LIB_RATSS_WITH_FPLLL
	#include <fplll.h>
#endif

namespace LIB_RATSS_NAMESPACE {

class Calc {
public:
	///Values are compatible with the ones defined in ProjectSN::SnapType
	typedef enum { ST_NONE=0x0, ST_CF=0x4, ST_FX=0x8, ST_FL=0x10, ST_JP=0x20, ST_} SnapType;
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
	mpfr::mpreal toFixpoint(const mpfr::mpreal & v, int significands = -1) const;
	void makeFixpoint(mpfr::mpreal& v, int significands = -1) const;
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
	
	mpq_class contFrac(const mpq_class& value, int significands) const;
	
	void jacobiPerron2D(const mpq_class& input1, const mpq_class& input2, mpq_class& output1, mpq_class& output2, int significands) const;
	
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const;
	
	mpq_class snap(const mpfr::mpreal & v, int st, int eps = -1) const;
public:
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	typename std::enable_if<
		std::is_same<
			typename std::decay<
				typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
			>::type,
			mpq_class
		>::value,
		void
	>::type
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
	
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	typename std::enable_if<
		!std::is_same<
			typename std::decay<
				typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
			>::type,
			mpq_class
		>::value,
		void
	>::type
	toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int eps = -1) const;
public:
	std::size_t maxBitCount(const mpq_class &v) const;
};

}//end namespace LIB_RATSS_NAMESPACE

//definitions

namespace LIB_RATSS_NAMESPACE {
	
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const {
#ifdef LIB_RATSS_WITH_FPLLL
	using InputIterator = T_INPUT_ITERATOR;
	using input_type = typename std::iterator_traits<InputIterator>::value_type;
	using std::distance;
	
	using Matrix = fplll::IntMatrix;
	
	auto dim = distance(begin, end);
	
	if (dim < 2) {
		throw std::runtime_error("Calc::lll: dimension has to be larger than 1");
	}
	
	Matrix mtx(dim+1, dim+1);
	{
		auto it(begin);
		for(uint32_t i(0); i < dim; ++i, ++it) {
			auto x = Conversion<input_type>::toMpq(*it);
			mtx(0, i+1) = x.get_num_mpz_t(); //numerator
			mtx(i+1, i+1) = x.get_den_mpz_t(); //denominator
		}
	}
	int status = fplll::lll_reduction(mtx);
	
	if (status != 0) {
		throw std::runtime_error("Could not approximate input with LLL");
	}
	
	common_denom = mpz_class( mtx(0, 0) );
	
	//common_denom now holds the correct denominator, calculate for each entry the best numerator ceil/floor
	for(auto it(begin); it != end; ++it, ++out) { 
		auto x = Conversion<input_type>::toMpq(*it);
		mpz_class num = (x.get_num() * common_denom) / x.get_den();
		mpz_class num1 = num+(x >= 0 ? 1 : -1);
		//check which one is better
		if (abs(x - mpq_class(num, common_denom)) < abs(x - mpq_class(num1, common_denom))) {
			*out = num;
		}
		else {
			*out = num1;
		}
	}
#else
	throw std::runtime_error("libratss was no compiled with support snapping using the lll algorithm");
#endif
}


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
typename std::enable_if<
	std::is_same<
		typename std::decay<
			typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
		>::type,
		mpq_class
	>::value,
	void
>::type
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int /*snapType*/, int eps) const {
	if (eps < 0) {
		for( ;begin != end; ++begin, ++out) {
			*out = *begin;
		}
	}
	else {
		throw std::runtime_error("Unsupported options: toRational with rational and eps");
	}
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
typename std::enable_if<
	!std::is_same<
		typename std::decay<
			typename std::iterator_traits<T_INPUT_ITERATOR>::value_type
		>::type,
		mpq_class
	>::value,
	void
>::type
Calc::toRational(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, int snapType, int significands) const {
	using input_ft = typename std::iterator_traits<T_INPUT_ITERATOR>::value_type;
	if (snapType & ST_JP) {
		using std::distance;
		if (distance(begin, end) != 2) {
			throw std::domain_error("Calc::toRational: Snapping with jacobiPerron only supports dimension 2");
		}
		mpq_class input1( Conversion<input_ft>::toMpq(*begin) );
		++begin;
		mpq_class input2( Conversion<input_ft>::toMpq(*begin) );
		mpq_class output1, output2;
		jacobiPerron2D(input1, input2, output1, output2, significands);
		*out = output1;
		++out;
		*out = output2;
		++out;
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

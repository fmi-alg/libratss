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
	typedef enum { ST_NONE=0x0, 
		ST_CF=0x8, //snap by continous fraction
		ST_FX=ST_CF*2, //snap by fix point
		ST_FL=ST_FX*2, //snap by floating point
		ST_JP=ST_FL*2, // jacobi perron
		ST_FPLLL=ST_JP*2,
	} SnapType;
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
	void apply_common_denominator(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const mpz_class & common_denom) const;
	///this will first set common_denom and the write all numerators to out
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const;
	
	mpq_class snap(const mpfr::mpreal & v, int st, int eps = -1) const;
	mpq_class snap(const mpq_class & v, int st, int eps = -1) const;
	mpq_class snap(const mpq_class & v, int st, const mpq_class & eps) const;
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
void Calc::apply_common_denominator(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, const mpz_class & common_denom) const {
	using InputIterator = T_INPUT_ITERATOR;
	using input_type = typename std::iterator_traits<InputIterator>::value_type;
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
}

///See Siam Journal on Computing: THE COMPUTATIONAL COMPLEXITY OF SIMULTANEOUS DIOPHANTINE APPROXIMATION PROBLEMS by J. C. LAGARIAS
///How we set N:
///Theorem Dirichlet:
///for x, N exists q s.t. abs(x - p/q) <= 1/(q * sqrt(N))
///
#ifdef LIB_RATSS_WITH_FPLLL
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, int significands) const {
	using std::distance;
	
	using Matrix = fplll::IntMatrix;
	
	auto dim = distance(begin, end);
	
	if (dim < 2) {
		throw std::domain_error("Calc::lll: dimension has to be larger than 1");
	}
	
	mpq_class eps = mpq_class(mpz_class(1), mpz_class(1) << significands); //input eps
	eps = (eps / dim) * mpq_class(mpz_class(1), mpz_class(1) << (dim/2+3)); //our eps
	
	mpz_class N = (eps.get_den() / eps.get_num()) + 1; // 1/eps
	mpz_class B; //either product or the common denominator for the input if available
	mpz_class NB;
	
	mpq_class best_eta;
	mpq_class best_common_denom;
	
	mpq_class current_eta;
	mpz_class current_common_denom;
	
	//init B, first check if all denominators of the input are equal
	{
		auto it(begin);
		B = it->get_den();
		bool input_has_common_denom = true;
		for(++it; it != end; ++it) {
			if (it->get_den() != B) {
				input_has_common_denom = false;
				break;
			}
		}
		if (!input_has_common_denom) {
			B = 1;
			for(it = begin; it != end; ++it) {
				B *= *it;
			}
		}
	}
	
	NB = N*B;
	
	for(int j(0), s(dim+msb(NB)+1); j < s; ++j) {
		Matrix mtx(dim+1, dim+1);
		{
			auto it(begin);
			for(uint32_t i(0); i < dim; ++i, ++it) {
				mpq_class x = NB * (*it);
				::mpz_set(mtx(i+1, i+1).get_data(), x.get_num_mpz_t());
				::mpz_set(mtx(0, i+1).get_data(), NB.get_mpz_t());
			}
		}
		int status = fplll::lll_reduction(mtx);
		if (!status) {
			throw std::runtime_error("LLL reduction failed");
		}
		current_common_denom = mpz_class( mtx(0, 0).get_data() );
		current_eta = abs( mpz_class(mtx(0, 1).get_data()) );
		for(int i(1); i < dim; ++i) {
			mpz_class xi( abs(mpz_class(mtx(0, i+1).get_data()) ) );
			if (xi > current_eta) {
				current_eta = xi;
			}
		}
		if (j == 0 || current_eta < best_eta) {
			best_eta = std::move(current_eta);
			best_common_denom = std::move(current_common_denom);
		}
	}
	
	common_denom = std::move(best_common_denom);
	apply_common_denominator(begin, end, out, common_denom);
}
#else
template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR, T_INPUT_ITERATOR, T_OUTPUT_ITERATOR, mpz_class &, int) const {
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
	else if (snapType & ST_FPLLL) {
		using std::distance;
		std::vector<mpq_class> tmp;
		std::vector<mpz_class> numerators(distance(begin, end));
		mpz_class common_denom;
		
		tmp.reserve(numerators.size());
		for(; begin != end; ++begin) {
			tmp.emplace_back(Conversion<input_ft>::toMpq(*begin));
		}
		
		lll(tmp.begin(), tmp.end(), numerators.begin(), common_denom, significands);
		for(const mpz_class & x : numerators) {
			*out = mpq_class(x, common_denom);
			++out;
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

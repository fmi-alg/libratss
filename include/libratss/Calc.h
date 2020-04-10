#ifndef LIB_RATSS_CALC_H
#define LIB_RATSS_CALC_H
#pragma once

#include <libratss/constants.h>
#include <libratss/enum.h>
#include <libratss/Conversion.h>

#ifdef LIB_RATSS_WITH_FPLLL
	#include <fplll.h>
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
	template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
	void lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, mpq_class epsilon) const;
	
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
	mpq_class eps = mpq_class(mpz_class(1), mpz_class(1) << significands); //input eps
	lll(
		std::forward<T_INPUT_ITERATOR>(begin),
		std::forward<T_INPUT_ITERATOR>(end),
		std::forward<T_OUTPUT_ITERATOR>(out),
		common_denom,
		eps
	);
}

template<typename T_INPUT_ITERATOR, typename T_OUTPUT_ITERATOR>
void Calc::lll(T_INPUT_ITERATOR begin, T_INPUT_ITERATOR end, T_OUTPUT_ITERATOR out, mpz_class & common_denom, mpq_class eps) const {
	using std::distance;
	
	using Matrix = fplll::ZZ_mat<mpz_t>;
	
	auto dim = distance(begin, end);
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "LLL with " << dim << " dimensions" << std::endl;
		std::cerr << "Input=";
		for(auto it(begin); it != end; ++it) {
			mpq_class tmp(*it);
			tmp.canonicalize();
			std::cerr << tmp << " ";
		}
		std::cerr << std::endl;
	#endif
	
	if (dim < 2) {
		throw std::domain_error("ratss::Calc::lll: dimension has to be larger than 1");
	}
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "Input eps: " << eps << std::endl;
	#endif
	
	//let d=dim
	//Let b_i=*(it+i)=u_i/v_i and hence b=(b_0,...b_d)
	//Let {{bq}} = min_{p_i in Z} max_{b_i in b} abs{u_i/v_i - p_i/q}
	//where p_i is computed using the apply_common_denominator function which takes a p_i such that abs{u_i/v_i - p_i/q} is minimal
	//For N=eps^(-d) and B=prod v_i the algorithm produces
	//1 <= q <= 2^(d/2) N * B with
	//{{bq}} <= \sqrt(5*d) * 2^((d-1)/2) * eps
	//
	//In order to guarantee our target epsilon we use our own epsilon to choose N:
	//eps = \sqrt(5*d) * 2^((d-1)/2) * eps_o
	//eps_o = eps/(sqrt(5*d) * 2^((d-1)/2))
	//N = 1/eps_o^d
	//N = (5d)^(d/2) * 2^((d^2-d)/2) * 1/eps^d
	
	mpz_class N;
	mpz_class B; //either product or the common denominator for the input if available
	mpz_class NB;
	
	mpq_class best_eta;
	mpq_class best_common_denom;
	
	mpq_class current_eta;
	mpz_class current_common_denom;
	
	{
		//Let a = (5d)^(d/2), b = 2^((d-1)*d/2), c = eps^d
		//Then N = a*b/c
		mpz_class a, b;
		mpq_class c;
		//Compute a
		if (dim % 2 == 1) {
			mpz_class tmp;
			mpz_ui_pow_ui(tmp.get_mpz_t(), 5*dim, dim);
			int exact = mpz_root(a.get_mpz_t(), tmp.get_mpz_t(), 2);
			a += int(exact != 0);
		}
		else {
			mpz_ui_pow_ui(a.get_mpz_t(), 5*dim, dim/2);
		}
		//Compute b:
		//If d=2k even then b=(2k-1)*2k/2=(2k-1)*k
		//If d=2k+1 odd then b=(2k+1-1)*(2k+1)/2=2k*(2k+1)/2=k*(2k+1)
		mpz_ui_pow_ui(b.get_mpz_t(), 2, ((dim-1)*dim)/2);
		
		//Compute c:
		{
			mpz_class tmp_num = eps.get_num();
			mpz_class tmp_den = eps.get_den();
			mpz_class c_num, c_den;
			mpz_pow_ui(c_num.get_mpz_t(), tmp_num.get_mpz_t(), dim);
			mpz_pow_ui(c_den.get_mpz_t(), tmp_den.get_mpz_t(), dim);
			c = mpq_class(c_num, c_den);
			c.canonicalize();
		}
		mpq_class N_r = (a*b)/c;
		
		N = (N_r.get_den() / N_r.get_num()) + int( N_r.get_den() % N_r.get_num() != 0 );
	}
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "N=" << N << std::endl;
	#endif
	
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
				B *= it->get_den();
			}
		}
		#ifdef LIBRATSS_DEBUG_VERBOSE
			if (input_has_common_denom) {
				std::cerr << "Input has common denominator" << std::endl;
			}
			else {
				std::cerr << "Input does not have a common denominator" << std::endl;
			}
		#endif
	}
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "B=" << B << std::endl;
	#endif
	
	NB = N*B;
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "NB=" << NB << std::endl;
	#endif
	
	for(int j(0), s(dim+msb(NB)+1); j < s; ++j) {
		#ifdef LIBRATSS_DEBUG_VERBOSE
			std::cerr << "Checking matrix for j=" << j << std::endl;
		#endif
		mpz_class w;
		if (j == 0) {
			w = 1;
		}
		else {
			w = mpz_class(2) << j;
		}
		
		Matrix mtx(dim+1, dim+1);
		mtx.gen_zero(dim+1, dim+1);
		
		::mpz_set(mtx(0, 0).get_data(), w.get_mpz_t());
		
		{
			auto it(begin);
			for(uint32_t i(0); i < dim; ++i, ++it) {
				mpq_class x = NB * (*it);
				::mpz_set(mtx(0, i+1).get_data(), x.get_num_mpz_t());
				::mpz_set(mtx(i+1, i+1).get_data(), NB.get_mpz_t());
			}
		}
		#ifdef LIBRATSS_DEBUG_VERBOSE
			std::cerr << mtx << std::endl;
		#endif
		int status = fplll::lll_reduction(mtx);
		if (status != fplll::RedStatus::RED_SUCCESS) {
			throw std::runtime_error("ratss::Calc::lll: LLL reduction failed with status=" + std::to_string(status));
		}
		
		#ifdef LIBRATSS_DEBUG_VERBOSE
			std::cerr << "=>" << std::endl;
			std::cerr << mtx << std::endl;
		#endif
		current_common_denom = abs( mpz_class( mtx(0, 0).get_data() ) );
		current_eta = abs( mpz_class(mtx(0, 1).get_data()) );
		for(int i(1); i < dim; ++i) {
			mpz_class xi( abs(mpz_class(mtx(0, i+1).get_data()) ) );
			if (xi > current_eta) {
				current_eta = xi;
			}
		}
		#ifdef LIBRATSS_DEBUG_VERBOSE
			std::cerr << "Current eta: " << current_eta << std::endl;
			std::cerr << "Best eta: " << best_eta << std::endl;
			std::cerr << "Current common denom: " << current_common_denom << std::endl;
			std::cerr << "Best common denom: " << best_common_denom << std::endl;
			std::cerr << std::endl;
		#endif
		if (j == 0 || current_eta < best_eta || (current_eta == best_eta && current_common_denom < best_common_denom)) {
			best_eta = std::move(current_eta);
			best_common_denom = std::move(current_common_denom);
		}
	}
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "Finished reductions" << std::endl;
		std::cerr << "Best eta: " << best_eta << std::endl;
		std::cerr << "Best common denom: " << best_common_denom << std::endl;
	#endif
	
	common_denom = std::move(best_common_denom);
	if (common_denom == 0) { //this means the approximation should be 0 for all points
		for(; begin != end; ++begin, ++out) {
			*out = mpq_class(0);
		}
	}
	else {
		apply_common_denominator(begin, end, out, common_denom);
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
			mpq_class pq(x, common_denom);
			pq.canonicalize();
			*out = pq;
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

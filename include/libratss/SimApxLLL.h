#ifndef LIBRATSS_SIM_APX_LLL_H
#define LIBRATSS_SIM_APX_LLL_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

#include <fplll.h>

namespace LIB_RATSS_NAMESPACE {
	

#define CLS_TMPL_DECL template<typename T_MPQ_CLASS_CONVERTIBLE_ITERATOR>
#define CLS_TMPL_NAME SimApxLLL<T_MPQ_CLASS_CONVERTIBLE_ITERATOR>
	
CLS_TMPL_DECL
class SimApxLLL final {
public:
	using input_iterator = T_MPQ_CLASS_CONVERTIBLE_ITERATOR;
	using result_iterator = std::vector<mpz_class>::iterator;
public:
	SimApxLLL(input_iterator begin, input_iterator end, int significands);
	SimApxLLL(input_iterator begin, input_iterator end, mpq_class eps);
	~SimApxLLL();
public:
	mpz_class run(bool greedy);
public:
	inline mpz_class const & denominator() const { return best_common_denom; }
	inline result_iterator numerators_begin() { return numerators.begin(); }
	inline result_iterator numerators_end() { return numerators.end(); }
private:
	using Matrix = fplll::ZZ_mat<mpz_t>;
private:
	void initB();
	void initN(mpq_class eps);
	mpz_class run_single();
private:
	mpq_class pow(mpq_class const & x, unsigned long int n);
	//max distance between input and result
	mpq_class apxEps();
	void set_numerators();
private:
	input_iterator begin;
	input_iterator end;
	mpq_class target_eps;
	int dim;
	std::vector<mpz_class> numerators;
	//State variables
	mpz_class N;
	mpz_class B; //either product or the common denominator for the input if available
	mpz_class NB;
	
	mpz_class best_eta;
	mpz_class best_common_denom;
	
	mpz_class current_eta;
	mpz_class current_common_denom;
};
	
} //end namespace LIB_RATSS_NAMESPACE

//Definitions
namespace LIB_RATSS_NAMESPACE {
	

CLS_TMPL_DECL
CLS_TMPL_NAME::SimApxLLL(input_iterator begin, input_iterator end, int significands) : 
SimApxLLL(begin, end, mpq_class(mpz_class(1), mpz_class(1) << significands))
{}

CLS_TMPL_DECL
CLS_TMPL_NAME::SimApxLLL(input_iterator begin, input_iterator end, mpq_class eps) :
begin(begin),
end(end),
target_eps(eps)
{
	using std::distance;
	dim = distance(begin, end);
	
	if (dim < 2) {
		throw std::domain_error("ratss::SimaApxLLL: dimension has to be larger than 1");
	}
	
	numerators.resize(dim);
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "ratss::SimaApxLLL with " << dim << " dimensions" << std::endl;
		std::cerr << "Input=";
		for(auto it(begin); it != end; ++it) {
			std::cerr << *it << " ";
		}
		std::cerr << std::endl;
	#endif
		
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "Input eps: " << eps << std::endl;
	#endif
}

CLS_TMPL_DECL
CLS_TMPL_NAME::~SimApxLLL() {
#ifdef LIBRATSS_DEBUG_VERBOSE
	std::cerr << "ratss::SimaApxLLL: Finished all calculations" << std::endl;
#endif
}

CLS_TMPL_DECL
void CLS_TMPL_NAME::initB() {
	//init B, first check if all denominators of the input are equal
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

CLS_TMPL_DECL
void CLS_TMPL_NAME::initN(mpq_class eps) {
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
	c = pow(eps, dim);
	mpq_class N_r = (a*b)/c;
	
	N = (N_r.get_den() / N_r.get_num()) + int( N_r.get_den() % N_r.get_num() != 0 );
}


CLS_TMPL_DECL
mpq_class CLS_TMPL_NAME::pow(mpq_class const & x, unsigned long int n) {
	mpz_class tmp_num = x.get_num();
	mpz_class tmp_den = x.get_den();
	mpz_class r_num, r_den;
	mpz_pow_ui(r_num.get_mpz_t(), tmp_num.get_mpz_t(), n);
	mpz_pow_ui(r_den.get_mpz_t(), tmp_den.get_mpz_t(), n);
	mpq_class r(r_num, r_den);
	r.canonicalize();
	return r;
}

CLS_TMPL_DECL
mpq_class CLS_TMPL_NAME::apxEps() {
	using std::abs;
	using std::max;
	auto it = begin;
	auto nit = numerators_begin();
	int i = 0;
	
	mpq_class tmp(*nit, denominator());
	tmp.canonicalize();
	mpq_class dist = abs(*it-tmp);
	
	for(++i, ++it, ++nit; i < dim; ++i, ++it, ++nit) {
		tmp = mpq_class(*nit, denominator());
		tmp.canonicalize();
		dist = max(dist, mpq_class(abs(*it-tmp)));
	}
	return dist;
}

CLS_TMPL_DECL
mpz_class CLS_TMPL_NAME::run(bool greedy) {
	
	initB();
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "B=" << B << std::endl;
	#endif
		
	if (!greedy) {
		initN(target_eps);
		
		#ifdef LIBRATSS_DEBUG_VERBOSE
			std::cerr << "N=" << N << std::endl;
		#endif
		
		return run_single();
	}
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "Using greedy strategy" << std::endl;
	#endif
	
	N = pow(1/target_eps, dim);
	
	while (true) {
		run_single();
		//check the distance
		if (apxEps() <= target_eps) {
			break;
		}
		else {
			N *= 2;
		}
		#ifdef LIBRATSS_DEBUG_VERBOSE
		{
			mpq_class dist = apxEps();
			std::cerr << "Current distance=" << dist << "~" << dist.get_d() << std::endl;
			std::cerr << "Increasing N to " << N << std::endl;
		}
		#endif
	}
	return best_common_denom;
}

CLS_TMPL_DECL
void CLS_TMPL_NAME::set_numerators() {
	auto out  = numerators.begin();
	if (best_common_denom == 0) {
		for(; begin != end; ++begin, ++out) {
			*out = mpz_class(0);
		}
	}
	else {
		for(auto it(begin); it != end; ++it, ++out) {
			mpq_class x = *it;
			mpz_class num = (x.get_num() * best_common_denom) / x.get_den();
			mpz_class num1 = num+(x >= 0 ? 1 : -1);
			//check which one is better
			mpq_class pq(num, best_common_denom);
			mpq_class pq1(num1, best_common_denom);
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
}
		
CLS_TMPL_DECL
mpz_class CLS_TMPL_NAME::run_single() {
	
	NB = N*B;
	
	#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "NB=" << NB << std::endl;
	#endif
	
	for(int j(0), s(dim+::mpz_sizeinbase(NB.get_mpz_t(), 2)); j < s; ++j) {
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
			for(int i(0); i < dim; ++i, ++it) {
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
	
	set_numerators();
	
	return best_common_denom;
}
	
} //end namespace LIB_RATSS_NAMESPACE

#undef CLS_TMPL_DECL
#undef CLS_TMPL_NAME


#endif

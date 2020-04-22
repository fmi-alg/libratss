#ifndef LIBRATSS_SIM_APX_BRUTE_FORCE_H
#define LIBRATSS_SIM_APX_BRUTE_FORCE_H
#pragma once

#include <libratss/constants.h>
#include <libratss/Conversion.h>

#include <array>
#include <numeric>

namespace LIB_RATSS_NAMESPACE {
	

#define CLS_TMPL_DECL template<typename T_INTEGER_BASE, std::size_t T_DIMENSIONS>
#define CLS_TMPL_NAME SimApxBruteForce<T_INTEGER_BASE, T_DIMENSIONS>
	
CLS_TMPL_DECL
class SimApxBruteForce {
public:
	static constexpr std::size_t dimensions = T_DIMENSIONS;
	static constexpr int default_number_of_significands = 31;
	
	using integer_base = T_INTEGER_BASE;
	using numerators_const_iterator = typename std::array<integer_base, dimensions>::const_iterator;
	
public:
	template<typename T_ITERATOR>
	SimApxBruteForce(T_ITERATOR begin, T_ITERATOR end);
	virtual  ~SimApxBruteForce() {}
public:
	void run(int significands);
public:
	inline integer_base const & denominator() const { return m_den; }
	inline numerators_const_iterator numerators_begin() { return m_num.begin(); }
	inline numerators_const_iterator numerators_end() { return m_num.end(); }
protected:
	std::array<integer_base, dimensions> m_sign;
	std::array<integer_base, dimensions> m_inum;
	std::array<integer_base, dimensions> m_iden;
	std::array<integer_base, dimensions> m_num;
	integer_base m_den;
};


template<std::size_t T_DIMENSIONS>
class SimApxBruteForce<mpq_class, T_DIMENSIONS> {
public:
	static constexpr int default_number_of_significands = 31;
	
	using integer_base = mpq_class;
	using numerators_const_iterator = typename std::vector<integer_base>::const_iterator;
	
public:
	template<typename T_ITERATOR>
	SimApxBruteForce(T_ITERATOR begin, T_ITERATOR end);
	virtual  ~SimApxBruteForce() {}
public:
	void run(int significands);
public:
	inline std::size_t dimensions() const { return m_dims; }
	inline integer_base const & denominator() const { return m_den; }
	inline numerators_const_iterator numerators_begin() { return m_num.begin(); }
	inline numerators_const_iterator numerators_end() { return m_num.end(); }
protected:
	std::vector<bool> m_sign;
	std::vector<integer_base> m_input;
	std::vector<integer_base> m_num;
	integer_base m_den;
	std::size_t m_dims;
};

} //end namespace LIB_RATSS_NAMESPACE

namespace LIB_RATSS_NAMESPACE {

CLS_TMPL_DECL
template<typename T_ITERATOR>
CLS_TMPL_NAME::SimApxBruteForce(T_ITERATOR begin, T_ITERATOR end) {
	for(auto it(begin); it != end; ++it) {
		mpq_class tmp = convert<mpq_class>(*it);
	}
}

CLS_TMPL_DECL
void SimApxBruteForce<T_INTEGER_BASE, T_DIMENSIONS>::run(int significands) {
	integer_base max_den = integer_base(1) << significands;
	std::array<integer_base, dimensions> num_f;
	std::array<integer_base, dimensions> num_c;
	
	for(integer_base den(2); den < max_den; ++den) {
		for(std::size_t i(0); i < dimensions; ++i) {
			integer_base num_f[i] = (m_inum[i]*den)/m_iden[i];
			integer_base num_c[i] = num_f[i]+1;
		}
		for(std::size_t i(0); i < dimensions; ++i) {
			
		}
	}
}


template<std::size_t T_DIMENSIONS>
template<typename T_ITERATOR>
SimApxBruteForce<mpq_class, T_DIMENSIONS>::SimApxBruteForce(T_ITERATOR begin, T_ITERATOR end) {
	for(; begin != end; ++begin) {
		m_input.push_back( convert<mpq_class>(*begin) );
		m_sign.push_back(m_input.back() < 0);
		if (m_sign.back()) {
			m_input.back() *= -1;
		}
	}
	m_dims = m_sign.size();
}

template<std::size_t T_DIMENSIONS>
void SimApxBruteForce<mpq_class, T_DIMENSIONS>::run(int significands) {
	std::size_t max_den = std::size_t(1) << significands;
	mpq_class bden = 2;
	mpq_class bdist(100);

	mpq_class newDist;
	bool haveNewDist;
	
	mpq_class num_f, num_c, dist;
	
	std::size_t i(max_den);
	mpz_class den(i);
	for(; i > 1; --i, --den) {
		//bdist = min(bdist, max(dist({...})))
		haveNewDist = true;
		newDist = 0;
		for(std::size_t j(0), s(dimensions()); j < s; ++j) {
			using std::floor;
			using std::abs;
			using std::min;
			num_f = (m_input[j].get_num()*den)/m_input[j].get_den();
			num_c = num_f+1;
			dist = min( abs(num_f/den - m_input[j]), abs(num_c/den - m_input[j]) );
			if (dist < bdist) {
				if (newDist < dist) {
					newDist = std::move(dist);
				}
			}
			else {
				haveNewDist = false;
				break;
			}
		}
		if (haveNewDist) {
			bden = den;
			bdist = newDist;
		}
	}
	m_den = bden;
	m_num.resize(dimensions());
	for(std::size_t i(0); i < dimensions(); ++i) {
		mpq_class num_f = (m_input[i].get_num()*m_den.get_num())/m_input[i].get_den();
		mpq_class num_c = num_f+1;
		if (abs(num_f/m_den - m_input[i]) <= abs(num_c/m_den - m_input[i])) {
			m_num[i] = num_f;
		}
		else {
			m_num[i] = num_c;
		}
		if (m_sign[i]) {
			m_num[i] *= -1;
		}
	}
}


#undef CLS_TMPL_DECL
#undef CLS_TMPL_NAME


	
} //end namespace LIB_RATSS_NAMESPACE

#endif

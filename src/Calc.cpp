#include <libratss/Calc.h>
#include <assert.h>
#include <cmath>

namespace LIB_RATSS_NAMESPACE {

mpfr::mpreal Calc::sin(const mpfr::mpreal& v) const {
	return mpfr::sin(v);
}

mpfr::mpreal Calc::asin(const mpfr::mpreal& v) const {
	return mpfr::asin(v);
}

mpfr::mpreal Calc::cos(const mpfr::mpreal& v) const {
	return mpfr::cos(v);
}

mpfr::mpreal Calc::acos(const mpfr::mpreal& v) const {
	return mpfr::acos(v);
}


mpfr::mpreal Calc::tan(const mpfr::mpreal & v) const {
	return mpfr::tan(v);
}

mpfr::mpreal Calc::atan(const mpfr::mpreal & v) const {
	return mpfr::atan(v);
}


mpfr::mpreal Calc::squaredDistance(const mpfr::mpreal& a, const mpfr::mpreal& b) const {
	return add(sq(a), sq(b));
}

mpfr::mpreal Calc::squaredDistance(const mpfr::mpreal& a, const mpfr::mpreal& b, const mpfr::mpreal& c) const {
	return add(add(sq(a), sq(b)), sq(c));
}

mpfr::mpreal Calc::mult(const mpfr::mpreal & a, const mpfr::mpreal & b) const {
	//double precision to remove rounding errors
	int prec = a.getPrecision() + b.getPrecision();
	mpfr::mpreal tmp1(a), tmp2(b);
	tmp1.setPrecision(prec);
	tmp2.setPrecision(prec);
	return tmp1*tmp2;
}

mpfr::mpreal Calc::div(mpfr::mpreal a, const mpfr::mpreal &b) const {
	int prec = std::max<int>(a.getPrecision(), b.getPrecision());
	a.setPrecision(prec);
	a /= b;
	return a;
}

mpfr::mpreal Calc::sq(const mpfr::mpreal& v) const {
	return mult(v, v);
}

mpfr::mpreal Calc::sqrt(const mpfr::mpreal& v) const {
	return mpfr::sqrt(v);
}

mpfr::mpreal Calc::add(const mpfr::mpreal & a, const mpfr::mpreal & b) const {
	//double precision to remove rounding
	int prec = std::max<int>(a.getPrecision(), b.getPrecision()) + 1;
	mpfr::mpreal tmp1(a), tmp2(b);
	tmp1.setPrecision(prec);
	tmp2.setPrecision(prec);
	return tmp1+tmp2;
}

mpfr::mpreal Calc::sub(const mpfr::mpreal & a, const mpfr::mpreal & b) const {
	//double precision to remove rounding errors
	int prec = std::max<int>(a.getPrecision(), b.getPrecision()) + 1;
	mpfr::mpreal tmp1(a), tmp2(b);
	tmp1.setPrecision(prec);
	tmp2.setPrecision(prec);
	return tmp1-tmp2;
}

/*
mpfr has the following constraints:
0.5 <= m < 1
m[0] = 1
e = signed integer
we want to emulate a fixed-point number, thus:
m * 2^exp = m_fixed
The issue here are not the large number since they should usually be between -1 and 1
The problem are points that are close to 0 that are representable by floating points but to a lesser degree by a fixed point type
*/
void Calc::makeFixpoint(mpfr::mpreal& v, int significands) const {
	if (significands < 0) {
		mp_exp_t exp = v.get_exp();
		int prec = v.get_prec();
		
		if (exp < -prec) { //exponent does more right-shifts than we have bits in our mantissa
			using std::signbit;
			v.setZero((signbit(v) ? -1 : 1));
		}
		else if (exp > prec) { //exponent does more left shifts than we have bits in our mantissa
			using std::signbit;
			v.setInf((signbit(v) ? -1 : 1));
		}
		else if (exp < 0) {
			//exponent does right shifts,
			//this means that there are leading zeros,
			//thus we need to cut off as many bits at the end as we have leading zeros
			int new_prec = prec + exp;
			v.setPrecision(new_prec, MPFR_RNDZ);
		}
		else if (exp > 0) {
			//exponent does left shifts,
			//this means that the decimal point changes
			//this is ok as long as it does not go beyond the last digit
			//this is captured in the second case
			//so there is nothing left todo here
			;
		}
	}
	else if (v.getPrecision() < significands) {
		throw std::domain_error(
			"Calc::makeFixpoint: Number of signifcands is " +
			std::to_string(significands) +
			" which is smaller than input precision which is " +
			std::to_string(v.getPrecision())
		);
	}
	else {
		v.setPrecision(significands);
		makeFixpoint(v, -1);
	}
}

mpfr::mpreal Calc::toFixpoint(const mpfr::mpreal& v, int significands) const {
	mpfr::mpreal c(v);
	makeFixpoint(c, significands);
	return c;
}

mpq_class Calc::within(const mpq_class & lower, const mpq_class & upper) const {
// 	if (lower == upper) {
// 		return lower;
// 	}
	
	if (lower > upper) {
		return within(upper, lower);
	}
	if (lower < 0 && upper > 0) {
		return mpq_class(0);
	}
	if (lower == 0 || upper == 0) {
		return mpq_class(0);
	}
	if (lower < 0) { //this also means that upper is < 0
		return - within(-upper, -lower);
	}
	
	
	//now calculate continous fractions for lower and upper up to the point where they differ
	std::vector<mpz_class> cf;
	mpz_class ldiv, udiv;
	mpq_class ltmp(lower), utmp(upper);
	while (true) {
// 		std::cout << "lower=" << ltmp << std::endl;
// 		std::cout << "upper=" << utmp << std::endl;
	
		ldiv = ltmp.get_num() / ltmp.get_den();
		udiv = utmp.get_num() / utmp.get_den();
		
// 		std::cout << "ldiv=" << ldiv << std::endl;
// 		std::cout << "udiv=" << udiv << std::endl;

		//prepare for next iteration
		ltmp -= mpq_class(ldiv);
		utmp -= mpq_class(udiv);

		//one is the prefix of the other
		
		if (ltmp == 0 && utmp == 0) {
			cf.push_back(ldiv);
// 			std::cout << "push " << ldiv << " and break" << '\n';
			break;
		}
		else if (ltmp == 0 || utmp == 0) {
			using std::min;
			if (ltmp == 0) {
				cf.push_back(ldiv);
			}
			else {
				cf.push_back(udiv);
			}
// 			cf.push_back(ldiv);
// 			cf.push_back( min(ldiv, udiv)+mpz_class(1) );
// 			std::cout << "Break since ltmp/utmp == 0" << std::endl;
// 			std::cout << "push " << min(ldiv, udiv)+mpz_class(1) << '\n';
			break;
		}
		
		if ( ldiv != udiv ) {
			using std::min;
			cf.push_back( min(ldiv, udiv)+mpz_class(1) );
// 			std::cout << "push " << min(ldiv, udiv)+mpz_class(1) << " and break" << '\n';
			break;
		}
		else {
			cf.push_back(ldiv);
// 			std::cout << "push " << ldiv << '\n';
		}
		
		ltmp = 1  / ltmp;
		utmp = 1 / utmp;
	}
	if (cf.size() == 1) {
		assert(cf.back() >= lower);
		assert(cf.back() <= upper);
		return mpq_class(cf.back());
	}
	
	//we now have a (regular) continous fraction in cf, let's reduce it
	//the form is a_0 + ( 1 / (a_1 + 1 / (a_2 + 1 / (a_3 + ...))))
	mpq_class result_naive(cf.back());
	result_naive.canonicalize();
	for(auto it(cf.rbegin()+1), end(cf.rend()); it != end; ++it) {
// 		std::cout << "result=" << result << std::endl;
// 		result is the denominator of the fraction
		result_naive = 1 / result_naive;
		result_naive += mpq_class( *it );
	}
	result_naive.canonicalize();
	
	mpz_class pn(cf.front()), pn1(1), pn2(0);
	mpz_class qn(1), qn1(0), qn2(1);
// 	mpz_class ptmp, qtmp;
	for(auto it(cf.begin()+1), end(cf.end()); it != end; ++it) {
		const mpz_class & a_i = *it;
		pn = a_i * pn1 + pn2;
		pn2 = pn1;
		pn1 = pn;
		
		qn = a_i * qn1 + qn2;
		qn2 = qn1;
		qn1 = qn;
	}
	mpq_class result(qn, pn);
	result.canonicalize();
	
	if (result != result_naive) {
		std::cout << "result=" << result << '\n';
		std::cout << "result_naive=" << result_naive << '\n';
		using std::abs;
		std::cout.precision(20);
		std::cout << "diff=" << Conversion<mpq_class>::toMpreal(abs(result-result_naive), 64) << std::endl;
	}
	assert(result_naive == result);
// 	if (result <= lower || result >= upper) {
// 		std::cout << "result=" << result << std::endl;
// 	}
// 	std::cout << "result=" << result << std::endl;
	if (result > upper) {
		std::cout << "result=" << result << '\n';
		std::cout << "upper=" << upper << '\n';
		std::cout << "lower=" << lower << '\n';
	}
	assert(result <= 1 || result >= -1);
	assert(result >= lower);
	assert(result <= upper);
	if (result.get_den() > upper.get_den() || result.get_den() > lower.get_den()) {
		std::cout << "result=" << result << '\n';
		std::cout << "upper=" << upper << '\n';
		std::cout << "lower=" << lower << '\n';
	}
	assert(result.get_den() <= lower.get_den());
	assert(result.get_den() <= upper.get_den());
	return result;
}

mpq_class Calc::snap(const mpfr::mpreal& v, int st, int significands) const {
	if (st & ST_CF) {
		if (significands < 0) {
			mpq_class rat = Conversion<mpfr::mpreal>::toMpq(v);
			mpz_class tmp(1);
			tmp <<= v.getPrecision();
			mpq_class eps = mpq_class(mpz_class(1), tmp)/2;
			mpq_class lower = rat - eps;
			mpq_class upper = rat + eps;
			assert(within(rat, rat) == rat);
			return within(lower, upper);
		}
		else if (v.getPrecision() < significands) {
			throw std::domain_error(
				"Calc::makeFixpoint: Number of signifcands is " +
				std::to_string(significands) +
				" which is smaller than input precision which is " +
				std::to_string(v.getPrecision())
			);
		}
		else {
			mpq_class rat = Conversion<mpfr::mpreal>::toMpq(v);
			mpq_class precEps = 0; //mpq_class(mpz_class(1), rat.get_den());
			mpz_class tmp(1);
			tmp <<= significands;
			mpq_class eps = mpq_class(mpz_class(1), tmp);
// 			std::cerr << eps << std::endl;
			mpq_class lower = rat - eps + precEps;
			mpq_class upper = rat + eps - precEps;
			return within(lower, upper);
		}
	}
	else if (st & ST_FX) {
		return Conversion<mpfr::mpreal>::toMpq( toFixpoint(v, significands) );
	}
	else if (st & ST_FL) {
		if (significands > 0 && significands != v.getPrecision()) {
			mpfr::mpreal tmp(v);
			tmp.setPrecision(significands);
			return Conversion<mpfr::mpreal>::toMpq(tmp);
		}
		else {
			return Conversion<mpfr::mpreal>::toMpq( v );
		}
	}
	else {
		throw std::runtime_error("ratss::Calc::snap: Unsupported snap type");
	}
}

std::size_t Calc::maxBitCount(const mpq_class &v) const {
	std::size_t sizeNum = mpz_sizeinbase(v.get_num().get_mpz_t(), 2);
	std::size_t sizeDenom = mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	return std::max<std::size_t>(sizeNum, sizeDenom);
}

}//end namespace
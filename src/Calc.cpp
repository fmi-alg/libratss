#include <libratss/Calc.h>
#include <assert.h>

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

mpfr::mpreal Calc::div(const mpfr::mpreal &a, const mpfr::mpreal &b) const {
	return a / b;
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

void Calc::makeFixpoint(mpfr::mpreal& v) {

}

mpfr::mpreal Calc::toFixpoint(mpfr::mpreal& v) const {

}


mpq_class Calc::within(const mpq_class & lower, const mpq_class & upper) {
	if (lower == upper) {
		return lower;
	}
	if (lower > upper) {
		return within(upper, lower);
	}
	if (lower < 0 && upper > 0) {
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

		if ( ldiv != udiv ) {
			using std::min;
			cf.push_back( min(ldiv, udiv)+1 );
			break;
		}
		else {
			cf.push_back(ldiv);
		}
		
		//prepare for next iteration
		ltmp -= ldiv;
		utmp -= udiv;
		
		//one is the prefix of the other
		if (ltmp == 0 || utmp == 0) {
			break;
		}
		
		ltmp = 1  / ltmp;
		utmp = 1 / utmp;
	}
	if (cf.size() == 1) {
		return mpq_class(cf.back());
	}
	
	//we now have a (regular) continous fraction in cf, let's reduce it
	//the form is a_0 + ( 1 / (a_1 + 1 / (a_2 + 1 / (a_3 + ...))))
	mpq_class result(cf.back());
	result.canonicalize();
	for(auto it(cf.rbegin()+1), end(cf.rend()); it != end; ++it) {
// 		std::cout << "result=" << result << std::endl;
		// result is the denominator of the fraction
		result = 1 / result;
		result += *it;
	}
	result.canonicalize();
// 	std::cout << "result=" << result << std::endl;
	assert(result >= lower);
	assert(result <= upper);
	return result;
}

mpq_class Calc::snap(const mpfr::mpreal & v) {
	mpq_class rat = Conversion<mpfr::mpreal>::toMpq(v);
	mpq_class eps = mpq_class(mpz_class(1), rat.get_den())/2;
	mpq_class lower = rat - eps;
	mpq_class upper = rat + eps;
	return within(lower, upper);
}

std::size_t Calc::maxBitCount(const mpq_class &v) const {
	std::size_t sizeNum = mpz_sizeinbase(v.get_num().get_mpz_t(), 2);
	std::size_t sizeDenom = mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	return std::max<std::size_t>(sizeNum, sizeDenom);
}

}//end namespace
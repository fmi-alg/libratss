#include <libratss/Calc.h>

#include <assert.h>
#include <cmath>

#include <libratss/internal/Matrix.h>

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
// 	int prec = a.getPrecision() + b.getPrecision();
// 	mpfr::mpreal tmp1(a), tmp2(b);
// 	tmp1.setPrecision(prec);
// 	tmp2.setPrecision(prec);
// 	return tmp1*tmp2;
	return a*b;
}

mpfr::mpreal Calc::div(const mpfr::mpreal & a, const mpfr::mpreal &b) const {
// 	int prec = std::max<int>(a.getPrecision(), b.getPrecision());
// 	a.setPrecision(prec);
// 	a /= b;
// 	return a;
	return a / b;
}

mpfr::mpreal Calc::sq(const mpfr::mpreal& v) const {
	return mult(v, v);
}

mpfr::mpreal Calc::sqrt(const mpfr::mpreal& v) const {
	return mpfr::sqrt(v);
}

mpq_class Calc::closestInteger(mpq_class v) const {
	if (v < 0) {
		return  - closestInteger(-v);
	}
	else {
		mpz_class q, r;
		::mpz_fdiv_qr(q.get_mpz_t(), r.get_mpz_t(), v.get_num_mpz_t(), v.get_den_mpz_t());
		if (r*2 > v.get_den()) { //ceil is closer
			q += 1;
			assert(abs(q-v) < abs((q-1)-v));
		}
		return q;
	}
}

mpfr::mpreal Calc::add(const mpfr::mpreal & a, const mpfr::mpreal & b) const {
	//double precision to remove rounding
// 	int prec = std::max<int>(a.getPrecision(), b.getPrecision()) + 1;
// 	mpfr::mpreal tmp1(a), tmp2(b);
// 	tmp1.setPrecision(prec);
// 	tmp2.setPrecision(prec);
// 	return tmp1+tmp2;
	return a+b;
}

mpfr::mpreal Calc::sub(const mpfr::mpreal & a, const mpfr::mpreal & b) const {
	//double precision to remove rounding errors
// 	int prec = std::max<int>(a.getPrecision(), b.getPrecision()) + 1;
// 	mpfr::mpreal tmp1(a), tmp2(b);
// 	tmp1.setPrecision(prec);
// 	tmp2.setPrecision(prec);
// 	return tmp1-tmp2;
	return a-b;
}


int Calc::msb(const mpz_class & v) const {
	return ::mpz_sizeinbase(v.get_mpz_t(), 2)-1;
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
mpfr::mpreal Calc::toFixpoint(mpfr::mpreal const & v, int significands) const {
	assert(significands > 0);
	assert(!isnan(v));
	assert(isfinite(v));
	using std::abs;
	
	int sign = v < 0 ? -1 : 1;
	
	if (v*sign >= 1) {
		return mpfr::const_infinity(sign, significands);
	}

	mpfr_exp_t exp = v.get_exp();
	mpfr::mpreal result(0, significands);
	
	assert(exp <= 0);
	if (-exp > significands) {
		result.setZero(sign);
	}
	else {
		//this will get us the mantissa in base 2 without a sign character in front
		result = v*sign;
		char * binstr = ::mpfr_get_str(0, &exp, 2, significands, result.mpfr_ptr(), MPFR_RNDZ);
		std::string strval(binstr);
		::mpfr_free_str(binstr);
		
		//our number has abs(exp) many 0 bits at the front
		//In total we want signifcands many bits
		//thus only signifcands-abs(exp) many bits remain after adding the zeros in front
		strval.resize(significands-abs(exp));
		
		result.setPrecision(std::max<int>(2, significands-abs(exp))); //a precision < 2 is not allowed
		::mpfr_set_str(result.mpfr_ptr(), strval.data(), 2, MPFR_RNDZ);
		::mpfr_set_exp(result.mpfr_ptr(), exp);
		result *= sign;
	}
	return result;
}

#if defined(LIB_RATSS_WITH_CGAL)
CORE::BigFloat Calc::toFixpoint(CORE::BigFloat const & v, int significands) const {
	using std::abs;
	assert(significands > 0);
	
	int sign = v < 0 ? -1 : 1;
	
	if (v*sign >= 1) {
		throw std::runtime_error("Calc::toFixpoint: Conversion would result in infinite value, but BigFloat does not support it.");
	}
	else if (v == 0) {
		return v;
	}
	//CORE1 BigFloat is different from mpfr
	//Here the floating point is at the end of the mantissa m
	//Hence in our case abs(exp) >= bits(m)
	mpz_class m(v.m().get_mp());
	m *= sign;
	
	//Exponents in BigFloat are a bit complicated
	//The function BigFloat::exp() returns the number of chunk shifts
	//So the real number is m()*B^exp() with B either 2^14 or 2^30
	//However when setting a BigFloat then we have m*2^exp
	long int mbits = this->numBits(m);
	long int exp = v.getRep().bits(v.exp());
	
	assert(exp <= 0);
	assert(-exp >= mbits);
	if (abs(exp)-mbits >= significands) { //check if we have more zeros in front than we have significant bits
		return CORE::BigFloat(double(0)*sign);
	}
	
	long int leadingZeros = abs(exp)-mbits;
	long int targetMantissaBits = significands-leadingZeros;
	assert(targetMantissaBits > 0);
	long int shiftBits = mbits-targetMantissaBits;
	m >>= shiftBits;
	exp += shiftBits;
	
	m *= sign;
	
	CORE::BigInt mc(m.get_mpz_t());
	return CORE::BigFloat(mc, exp);
}
#endif

#if defined(LIB_RATSS_WITH_CORE_TWO)
CORE_TWO::BigFloat Calc::toFixpoint(CORE_TWO::BigFloat const & v, int significands) const {
	return CORE_TWO::BigFloat(toFixpoint(mpfr::mpreal(v.mp()), significands).mpfr_srcptr());
}
#endif

mpq_class fromRegContFrac(const std::vector<mpz_class> & cf) {
	if (cf.size() == 1) {
		return mpq_class(cf.back());
	}

	//we have a regular continous fraction in cf, let's reduce it
	//the form is a_0 + ( 1 / (a_1 + 1 / (a_2 + 1 / (a_3 + ...))))
	//we use the fast non-division form to reconstruct our fraction
	mpz_class pn(cf.front()), pn1(1), pn2(0);
	mpz_class qn(1), qn1(0), qn2(1);
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
	return result;
}

mpq_class Calc::within(const mpq_class & lower, const mpq_class & upper) const {
	if (lower == upper) {
		return lower;
	}
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
	
		ldiv = ltmp.get_num() / ltmp.get_den();
		udiv = utmp.get_num() / utmp.get_den();
		
		//prepare for next iteration
		ltmp -= mpq_class(ldiv);
		utmp -= mpq_class(udiv);

		//one is the prefix of the other
		
		if (ltmp == 0 && utmp == 0) {
			cf.push_back(ldiv);
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
			break;
		}
		
		if ( ldiv != udiv ) {
			using std::min;
			cf.push_back( min(ldiv, udiv)+mpz_class(1) );
			break;
		}
		else {
			cf.push_back(ldiv);
		}
		
		ltmp = 1  / ltmp;
		utmp = 1 / utmp;
	}
	mpq_class result = fromRegContFrac(cf);
	
	//make sure that we always return the smallest possible denominator
	if (result == lower || result == upper) {
		if (lower.get_den() <= upper.get_den()) {
			result = lower;
		}
		else {
			result = upper;
		}
	}
	assert(result <= 1 || result >= -1);
	assert(result >= lower);
	assert(result <= upper);
	assert(result.get_den() <= lower.get_den());
	assert(result.get_den() <= upper.get_den());
	return result;
}

mpq_class Calc::contFrac(const mpq_class& value, int significands, int mode) const {
	
	if (value < 0) {
		return -contFrac(-value, significands, mode);
	}
	if (value == 0) {
		return mpq_class(0);
	}
	mpz_class epsDenom(1);
	epsDenom <<= significands;
	mpq_class eps{mpz_class(1), epsDenom};
	
	if (value >= 1) {
		mpz_class intPart = value.get_num() / value.get_den();
		mpq_class remainder = value - intPart;
		if ((mode & ST_GUARANTEE_DISTANCE) && remainder < eps) {
			return mpq_class(intPart);
		}
		return intPart + contFrac(remainder, significands, mode);
	}
	//we now know that 0 < value < 1
	
#ifndef NDEBUG
	std::vector<mpz_class> cf;
#else
	mpz_class cfBack;
#endif
	mpz_class distUpperBoundDenom;
	mpz_class intPart;
	mpq_class tmp(value);

	//take care of the first iteration
	intPart = tmp.get_num() / tmp.get_den();
	tmp -= intPart;
	
	if (mode & ST_GUARANTEE_DISTANCE) {
		if (tmp < eps) { //distance to real value is smaller than eps
			if (intPart == 0) {
				return mpq_class(0);
			}
			else {
				return mpq_class(mpz_class(1), mpz_class(intPart));
			}
		}
	}
	else if (mode & ST_GUARANTEE_SIZE) {
		 //den of first iterator is larger than allowed value
		if (mpz_sizeinbase(tmp.get_den().get_mpz_t(), 2) > size_t(significands)) {
			return 0;
		}
	}
	else {
		throw std::runtime_error("ratss::Calc::contFrac: unsupported mode:" + std::to_string(mode & ST_GUARANTEE_MASK));
	}
	
#ifndef NDEBUG
	cf.emplace_back( std::move(intPart) );
#else
	cfBack = std::move(intPart);
#endif
	//a_1 is now in cf, calculate a_2...
	//eps is of the form 1/number, our bound is
	//abs(value-p_n/q_n) < 1/(a_(n+1) * q_n**2 )
	
	//we have to calculate our continuous fraction on the fly in order to obey our significand as good as possible
	#ifndef NDEBUG
	mpz_class pn(cf.back());
	#else
	mpz_class pn(cfBack);
	#endif
	mpz_class pn1(1), pn2(0);
	mpz_class qn(1), qn1(0), qn2(1);
	
	mpq_class result;
	while(tmp > 0) {
		tmp = 1 / tmp;
		intPart = tmp.get_num() / tmp.get_den();
		#ifndef NDEBUG
		mpz_class qsq = cf.back()*cf.back();
		#else
		mpz_class qsq = cfBack*cfBack;
		#endif
		distUpperBoundDenom = (intPart) * qsq;
		
		if (mode & ST_GUARANTEE_DISTANCE) { 
			if (distUpperBoundDenom > epsDenom) {
				break;
			}
		}
		
		tmp -= intPart;
		#ifndef NDEBUG
		cf.emplace_back( std::move(intPart) );
		const mpz_class & a_i = cf.back();
		#else
		cfBack = std::move(intPart);
		const mpz_class & a_i = cfBack;
		#endif
		pn = a_i * pn1 + pn2;
		pn2 = pn1;
		pn1 = pn;
		
		qn = a_i * qn1 + qn2;
		qn2 = qn1;
		qn1 = qn;
		mpq_class nextResult(qn, pn);
		nextResult.canonicalize();
		assert(fromRegContFrac(cf) == nextResult);
		if (mode & ST_GUARANTEE_DISTANCE) {
			if (abs(nextResult - value) < eps) {
				result = nextResult;
				break;
			}
		}
		else if (mode & ST_GUARANTEE_SIZE) {
			if (mpz_sizeinbase(nextResult.get_den().get_mpz_t(), 2) > size_t(significands)) {
				break;
			}
		}
		result = nextResult;
	}
	return result;
}

void Calc::jacobiPerron2D(const mpq_class& input1, const mpq_class& input2, mpq_class& output1, mpq_class & output2, int significands, int mode) const {
	using Matrix = internal::Matrix<mpz_class>;
	using std::abs;
	
	if (significands < 2) {
		throw std::underflow_error("ratss::Calc::jacobiPerron2D: significands is too small.");
	}
	
	if (input1 < 0) {
		jacobiPerron2D(-input1, input2, output1, output2, significands, mode);
		output1 *= -1;
		return;
	}
	if (input2 < 0) {
		jacobiPerron2D(input1, -input2, output1, output2, significands, mode);
		output2 *= -1;
		return;
	}
	
	//input1, input2 >= 0
	
	if (input1 > 1) {
		mpz_class tmp1 = input1.get_num() / input1.get_den();
		jacobiPerron2D(input1-tmp1, input2, output1, output2, significands, mode);
		output1 += tmp1;
		return;
	}
	if (input2 > 1) {
		mpz_class tmp2 = input2.get_num() / input2.get_den();
		jacobiPerron2D(input1, input2-tmp2, output1, output2, significands, mode);
		output2 += tmp2;
		return;
	}
	
	assert(input1 >= 0 && input1 <= 1);
	assert(input2 >= 0 && input2 <= 1);
	
	mpq_class eps = mpq_class(mpz_class(1), mpz_class(1) << significands);
	
	if (mode & ST_GUARANTEE_DISTANCE) {
		if (abs(input1) < eps && abs(input2) < eps) {
			output1 = 0;
			output2 = 0;
			return;
		}
		else if (abs(input1) < eps) {
			output1 = 0;
			output2 =  within(input2 - eps, input2 + eps); //TODO:use contFrac here
			return;
		}
		else if (abs(input2) < eps) {
			output2 = 0;
			output1 = within(input1 - eps, input1 + eps); //TODO:use contFrac here
			return;
		}
	}
	
	Matrix result( Matrix::identity(3) );
	Matrix mtxStep(3);
	mtxStep(0, 2) = 1;
	mtxStep(1, 0) = 1;
	mtxStep(2, 1) = 1;
	
	mpz_class an, bn;
	
	mpq_class alpha(input1), beta(input2);
	
	mpq_class tmp1, tmp2;
	
	while(alpha != 0) {
		tmp1 = 1 / alpha;
		an = closestInteger(tmp1);
		
		tmp2 = beta / alpha;
		bn = closestInteger(tmp2);
		
		alpha = tmp2 - bn;
		beta = tmp1 - an;
		
		assert(alpha >= 0 && beta >= 0);
		
		mtxStep(0,0) = an;
		mtxStep(2,0) = bn;
		
		result = result * mtxStep;
		
		if (mode & ST_GUARANTEE_DISTANCE) {
			output1 = mpq_class( result(1, 0), result(0, 0) );
			output2 = mpq_class( result(2, 0), result(0, 0) );
			
			output1.canonicalize();
			output2.canonicalize();
			
			const mpq_class diff1 = abs(output1-input1);
			const mpq_class diff2 = abs(output2-input2);
			
			if ( diff1 <= eps && diff2 <= eps) {
				break;
			}
		}
		else if (mode & ST_GUARANTEE_SIZE) {
			if (result(0,0) <= eps.get_den()) {
				output1 = mpq_class( result(1, 0), result(0, 0) );
				output2 = mpq_class( result(2, 0), result(0, 0) );
				
				output1.canonicalize();
				output2.canonicalize();
			}
			else {
				break;
			}
		}
	}
	//TODO: if alpha = 0, but beta not good enough?
	
	if (alpha == 0) {
		#ifdef LIBRATSS_DEBUG_VERBOSE
		std::cerr << "ratss::Calc::jacobiPerron2D: simultaneous approximation failed. Using continued fractions." << std::endl;
		#endif
		if (mode & ST_GUARANTEE_DISTANCE) {
			if (abs(output1-input1) > eps) {
				output1 = within(input1-eps, input1+eps);
			}
			if (abs(output2-input2) > eps) {
				output2 = within(input2-eps, input2+eps);
			}
		}
		else if (mode & ST_GUARANTEE_SIZE) {
			if (output1.get_den() > eps.get_den()) {
				output1 = contFrac(input1, significands, mode);
			}
			if (output2.get_den() > eps.get_den()) {
				output2 = contFrac(input2, significands, mode);
			}
		}
	}
	
	if (mode & ST_GUARANTEE_DISTANCE) {
		assert(abs(output1-input1) <= eps);
		assert(abs(output2-input2) <= eps);
	}
	else if (mode & ST_GUARANTEE_SIZE) {
		assert(output1.get_den() <= eps.get_den());
		assert(output2.get_den() <= eps.get_den());
	}
}

mpq_class Calc::snap(const mpfr::mpreal& v, int st, int significands) const {
	if (!isfinite(v)) {
		throw std::runtime_error("Calc::snap: v=" + v.toString() + " is not finite");
	}
	if (abs(v) >= 1) {
		mpfr::mpreal integerPart;
		mpfr::mpreal fractionalPart = modf(abs(v), integerPart);
		return (v < 0 ? -1 : 1) * (snap(fractionalPart, st, significands) + Conversion<mpfr::mpreal>::toMpq(integerPart));
	}
	if (st & ST_CF) {
		if (v.getPrecision() < significands+2) {
			throw std::domain_error(
				"Calc::makeFixpoint: Number of signifcands is " +
				std::to_string(significands) +
				" which is smaller than input precision which is " +
				std::to_string(v.getPrecision())
			);
		}
		else {
			return contFrac(snap(v, ST_FX, significands+2), significands+1, st & ST_GUARANTEE_MASK);
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
		throw std::runtime_error("ratss::Calc::snap: Unsupported snap type: " + std::to_string(st & ST_SNAP_TYPES_MASK));
	}
}

mpq_class
Calc::snap(const mpq_class & v, int st, int significands) const {
	mpq_class result;
	mpq_class epsq = mpq_class(mpz_class(1), mpz_class(1) << significands);
	if (st & ST_CF) {
		result = snap(v, st, epsq);
	}
	else {
		result = snap(Conversion<mpq_class>::toMpreal(v, significands+2), st, significands);
		if (abs(result - v) > epsq) {
			throw std::runtime_error("libratss::Calc::snap: could not approximate rational to requested precision");
		}
	}
	assert(abs(result - v) <= epsq);
	return result;
}

mpq_class
Calc::snap(const mpq_class & v, int st, const mpq_class & eps) const {
	mpq_class result;
	if (st & ST_CF) {
		mpq_class lower = v - eps;
		mpq_class upper = v + eps;
		result = within(lower, upper);
	}
	else {
		mpz_class tmp = eps.get_num() / eps.get_den() + int(eps.get_num() % eps.get_den() > 0);
		int epsBits = mpz_sizeinbase(tmp.get_mpz_t(), 2);
		result = snap(v, st, epsBits+1);
	}
	assert(abs(result - v) <= eps);
	return result;
}

std::size_t Calc::maxBitCount(const mpq_class &v) const {
	return std::max<std::size_t>(numBits(v.get_num()), numBits(v.get_den()));
}

std::size_t Calc::numBits(const mpz_class &v) const {
	return mpz_sizeinbase(v.get_mpz_t(), 2);
}

}//end namespace

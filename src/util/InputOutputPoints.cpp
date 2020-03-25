#include <libratss/util/InputOutputPoints.h>

namespace LIB_RATSS_NAMESPACE {

void FloatPoint::normalize() {
	c.normalize(coords.begin(), coords.end(), coords.begin());
}
void FloatPoint::setPrecision(int precision) {
	//set the precision of our input variables
	for(mpfr::mpreal & v : coords) {
		v.setPrecision(precision, MPFR_RNDZ);
	}
}
void FloatPoint::assign(std::istream & is, Format fmt, int precision, int dimension) {
	coords.clear();
	if (fmt == FM_CARTESIAN_FLOAT || fmt == FM_CARTESIAN_FLOAT128) {
		while (is.good() && is.peek() != '\n' && (int) coords.size() != dimension) {
			mpfr::mpreal tmp;
			is >> tmp;
			coords.emplace_back( std::move(tmp) );
		}
	}
	else if (fmt == FM_CARTESIAN_RATIONAL) {
		mpq_class tmp;
		while (is.good() && is.peek() != '\n' && (int) coords.size() != dimension) {
			is >> tmp;
			coords.emplace_back( Conversion<mpq_class>::toMpreal(tmp, precision) );
		}
	}
	else if (fmt == FM_CARTESIAN_SPLIT_RATIONAL) {
		mpz_class num, denom;
		mpq_class tmp;
		while (is.good() && is.peek() != '\n' && (int) coords.size() != dimension) {
			is >> num >> denom;
			tmp = mpq_class(num, denom);
			coords.emplace_back( Conversion<mpq_class>::toMpreal(tmp, precision) );
		}
	}
	else if (fmt == FM_GEO) {
		coords.resize(3);
		mpfr::mpreal lat, lon;
		is >> lat >> lon;
		precision = std::max<int>(precision, 53);
		lat.setPrecision(precision);
		lon.setPrecision(precision);
		c.cartesian(lat, lon, coords[0], coords[1], coords[2]);
	}
	else if (fmt == FM_SPHERICAL) {
		coords.resize(3);
		mpfr::mpreal theta, phi;
		is >> theta >> phi;
		precision = std::max<int>(precision, 53);
		theta.setPrecision(precision);
		phi.setPrecision(precision);
		c.cartesianFromSpherical(theta, phi, coords[0], coords[1], coords[2]);
	}
	else {
		throw std::runtime_error("ratss::FloatPoint: unsupported format");
	}
}
void FloatPoint::print(std::ostream & out) const {
	if (!coords.size()) {
		return;
	}
	std::vector<mpfr::mpreal>::const_iterator it(coords.begin()), end(coords.end());
	out << *it;
	for(++it; it != end; ++it) {
		out << ' ' << *it;
	}
}

mpfr::mpreal FloatPoint::epsUpperBound() const {
	return (sqLen() - 1)/2;
}

mpfr::mpreal FloatPoint::sqLen() const {
	return c.squaredLength(coords.cbegin(), coords.cend());
}

std::ostream & operator<<(std::ostream & out, const FloatPoint & src) {
	src.print(out);
	return out;
}

RationalPoint::RationalPoint() {}

RationalPoint::RationalPoint(const std::string & str, Format fmt, int dimension) {
	std::stringstream ss;
	ss << str;
	assign(ss, fmt, 0, dimension);
}

RationalPoint::RationalPoint(const RationalPoint & other) : coords(other.coords) {}

RationalPoint::RationalPoint(RationalPoint && other) : coords(std::move(other.coords)) {}

RationalPoint & RationalPoint::operator=(const RationalPoint & other) {
	coords = other.coords;
	return *this;
}

RationalPoint & RationalPoint::operator=(RationalPoint && other) {
	coords = std::move(other.coords);
	return *this;
}

void RationalPoint::clear() {
	coords.clear();
}

void RationalPoint::resize(std::size_t _n) {
	coords.resize(_n);
}


void RationalPoint::assign(std::istream & is, Format fmt, int precision, int dimension) {
	coords.clear();
	if (fmt == FM_CARTESIAN_RATIONAL) {
		
		while (is.good() && is.peek() != '\n' && (int) coords.size() != dimension) {
			mpq_class tmp;
			is >> tmp;
			coords.emplace_back(std::move(tmp));
		}
	}
	else if (fmt == FM_CARTESIAN_SPLIT_RATIONAL && (int) coords.size() != dimension) {
		mpz_class num, denom;
		while (is.good() && is.peek() != '\n') {
			is >> num >> denom;
			mpq_class tmp = mpq_class(num, denom);
			coords.emplace_back(std::move(tmp));
		}
	}
	else {
		FloatPoint fp;
		try {
			fp.assign(is, fmt, precision, dimension);
		}
		catch(std::runtime_error & e) {
			throw std::runtime_error("ratss::RationalPoint: unsupported format");
		}
		coords.resize(fp.coords.size());
		for(std::size_t i(0), s(fp.coords.size()); i < s; ++i) {
			coords[i] = Conversion<mpfr::mpreal>::toMpq(fp.coords[i]);
		}
	}
}

void RationalPoint::print(std::ostream & out, Format fmt) const {
	if (!coords.size()) {
		return;
	}
	std::vector<mpq_class>::const_iterator it(coords.begin()), end(coords.end());
	if (fmt == FM_RATIONAL) {
		out << *it;
		for(++it; it != end; ++it) {
			out << ' ' << *it;
		}
	}
	else if (fmt == FM_SPLIT_RATIONAL) {
		out << it->get_num() << ' ' << it->get_den();
		for(++it; it != end; ++it) {
			out << ' ' << it->get_num() << ' ' << it->get_den();
		}
	}
	else if (fmt == FM_FLOAT) {
		std::streamsize prec = out.precision();
		out.precision(std::numeric_limits<double>::digits10+1);
		out << Conversion<mpq_class>::toMpreal(*it, 53).toDouble();
		for(++it; it != end; ++it) {
			out << ' ' << Conversion<mpq_class>::toMpreal(*it, 53).toDouble();
		}
		out.precision(prec);
	}
	else if (fmt == FM_FLOAT128) {
		std::streamsize prec = out.precision();
		out.precision(128);
		out << Conversion<mpq_class>::toMpreal(*it, 128);
		for(++it; it != end; ++it) {
			out << ' ' << Conversion<mpq_class>::toMpreal(*it, 128);
		}
		out.precision(prec);
	}
	else if (fmt == FM_GEO) {
		if (coords.size() != 3) {
			throw std::runtime_error("ratss::RationalPoint::print: requesting FM_GEO which only makes sense with 3 coordinates");
		}
		std::streamsize prec = out.precision();
		out.precision(std::numeric_limits<double>::digits10+1);
		mpfr::mpreal x = Conversion<mpq_class>::toMpreal(coords.at(0), 128);
		mpfr::mpreal y = Conversion<mpq_class>::toMpreal(coords.at(1), 128);
		mpfr::mpreal z = Conversion<mpq_class>::toMpreal(coords.at(2), 128);
		mpfr::mpreal lat, lon;
		c.geo(x, y, z, lat, lon);
		out << lat << ' ' << lon;
		out.precision(prec);
	}
	else if (fmt == FM_SPHERICAL) {
		if (coords.size() != 3) {
			throw std::runtime_error("ratss::RationalPoint::print: requesting FM_SPHERICAL which only makes sense with 3 coordinates");
		}
		std::streamsize prec = out.precision();
		out.precision(std::numeric_limits<double>::digits10+1);
		mpfr::mpreal x = Conversion<mpq_class>::toMpreal(coords.at(0), 128);
		mpfr::mpreal y = Conversion<mpq_class>::toMpreal(coords.at(1), 128);
		mpfr::mpreal z = Conversion<mpq_class>::toMpreal(coords.at(2), 128);
		mpfr::mpreal theta, phi;
		c.spherical(x, y, z, theta, phi);
		out << theta << ' ' << phi;
		out.precision(prec);
	}
}

bool RationalPoint::valid() const {
	mpq_class tmp(0);
	for(const mpq_class & c : coords) {
		tmp += c*c;
	}
	return tmp == mpq_class(1);
}


}//end namespace LIB_RATSS_NAMESPACE

#include <libratss/util/InputOutputPoints.h>

namespace LIB_RATSS_NAMESPACE {

void InputPoint::normalize() {
	c.normalize(coords.begin(), coords.end(), coords.begin());
}
void InputPoint::setPrecision(int precision) {
	//set the precision of our input variables
	for(mpfr::mpreal & v : coords) {
		v.setPrecision(precision, MPFR_RNDZ);
	}
}
void InputPoint::assign(std::istream & is, Format fmt, int precision) {
	coords.clear();
	if (fmt == FM_CARTESIAN) {
		while (is.good() && is.peek() != '\n') {
			mpfr::mpreal tmp;
			is >> tmp;
			coords.emplace_back( std::move(tmp) );
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
		throw std::runtime_error("ratss::InputPoint: unsupported format");
	}
}
void InputPoint::print(std::ostream & out) const {
	if (!coords.size()) {
		return;
	}
	std::vector<mpfr::mpreal>::const_iterator it(coords.begin()), end(coords.end());
	out << *it;
	for(++it; it != end; ++it) {
		out << ' ' << *it;
	}
}

mpfr::mpreal InputPoint::epsUpperBound() const {
	return (sqLen() - 1)/2;
}

mpfr::mpreal InputPoint::sqLen() const {
	return c.squaredLength(coords.cbegin(), coords.cend());
}

OutputPoint::OutputPoint() {}

OutputPoint::OutputPoint(int dimension) : coords(dimension) {}

OutputPoint::OutputPoint(const OutputPoint & other) : coords(other.coords) {}

OutputPoint::OutputPoint(OutputPoint && other) : coords(std::move(other.coords)) {}

OutputPoint & OutputPoint::operator=(const OutputPoint & other) {
	coords = other.coords;
	return *this;
}

OutputPoint & OutputPoint::operator=(OutputPoint && other) {
	coords = std::move(other.coords);
	return *this;
}

void OutputPoint::clear() {
	coords.clear();
}

void OutputPoint::resize(std::size_t _n) {
	coords.resize(_n);
}

void OutputPoint::print(std::ostream & out, Format fmt) const {
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
			throw std::runtime_error("ratss::OutputPoint::print: requesting FM_GEO which only makes sense with 3 coordinates");
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
			throw std::runtime_error("ratss::OutputPoint::print: requesting FM_SPHERICAL which only makes sense with 3 coordinates");
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

bool OutputPoint::valid() const {
	mpq_class tmp(0);
	for(const mpq_class & c : coords) {
		tmp += c*c;
	}
	return tmp == mpq_class(1);
}


}//end namespace LIB_RATSS_NAMESPACE
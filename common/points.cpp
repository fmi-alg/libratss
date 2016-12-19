#include "points.h"

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
void InputPoint::assign(std::istream & is) {
	coords.clear();
	while (is.good() && is.peek() != '\n') {
		mpfr::mpreal tmp;
		is >> tmp;
		coords.emplace_back( std::move(tmp) );
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
}

bool OutputPoint::valid() const {
	mpq_class tmp(0);
	for(const mpq_class & c : coords) {
		tmp += c*c;
	}
	return tmp == mpq_class(1);
}


}//end namespace LIB_RATSS_NAMESPACE
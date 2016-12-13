#include "types.h"

namespace LIB_RATSS_NAMESPACE {

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
		out << Conversion<mpq_class>::toMpreal(*it, 53).toDouble();
		for(++it; it != end; ++it) {
			out << ' ' << Conversion<mpq_class>::toMpreal(*it, 53).toDouble();
		}
	}
}

}//end namespace LIB_RATSS_NAMESPACE
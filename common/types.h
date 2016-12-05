#ifndef LIB_RATSS_TESTS_TYPES_H
#define LIB_RATSS_TESTS_TYPES_H
#include <libratss/constants.h>

namespace LIB_RATSS_NAMESPACE {

	struct Bounds {
		double minLat;
		double maxLat;
		double minLon;
		double maxLon;
		Bounds() : minLat(0.0), maxLat(0.0), minLon(0.0), maxLon(0.0) {}
		explicit Bounds(double minLat, double maxLat, double minLon, double maxLon) :
		minLat(minLat), maxLat(maxLat), minLon(minLon), maxLon(maxLon) {}
		bool valid() const {
			return minLat < maxLat && minLon < maxLon;
		}
	};

}

#endif
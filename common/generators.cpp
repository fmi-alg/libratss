#include "generators.h"


namespace LIB_RATSS_NAMESPACE {

std::vector<GeoCoord> getRandomGeoPoints(std::size_t count, const Bounds & bounds) {
	std::vector<GeoCoord> result;
	result.reserve(count);
	getRandomGeoPoints(count, bounds, std::back_inserter(result));
	return result;
}

std::vector<SphericalCoord>
getRandomPolarPoints(std::size_t number_of_points) {
	std::vector<SphericalCoord> result;
	result.reserve(number_of_points);
	getRandomPolarPoints(number_of_points, std::back_inserter(result));
	return result;
}

std::vector< GeoCoord > readPoints(const std::string &fileName) {
	std::vector<GeoCoord> result;
	readPoints(fileName, std::back_inserter(result));
	return result;
}


}//end namespace ratss::tests
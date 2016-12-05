#ifndef LIB_RATSS_TESTS_GENERATORS_H
#define LIB_RATSS_TESTS_GENERATORS_H
#pragma once
#include <vector>

#include <libratss/constants.h>
#include <libratss/GeoCoord.h>
#include <libratss/SphericalCoord.h>

#include "types.h"


namespace LIB_RATSS_NAMESPACE {

std::vector<GeoCoord> getRandomGeoPoints(std::size_t count, const Bounds & bounds);
std::vector<SphericalCoord> getRandomPolarPoints(std::size_t number_of_points);
std::vector<GeoCoord> readPoints(const std::string & fileName);

template<typename T_OUTPUT_ITERATOR>
void getRandomGeoPoints(std::size_t count, const Bounds & bounds, T_OUTPUT_ITERATOR out);

///get @param number_of_points random points, there may be duplicates!
template<typename T_OUTPUT_ITERATOR>
void getRandomPolarPoints(std::size_t number_of_points, T_OUTPUT_ITERATOR out);

template<typename T_OUTPUT_ITERATOR>
void readPoints(const std::string & fileName, T_OUTPUT_ITERATOR out);

}//end namespace ratss::tests

#include <libratss/Projector.h>

#include <random>
#include <unordered_set>

#include <CGAL/point_generators_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <chrono>

//and now the definitions
namespace LIB_RATSS_NAMESPACE {

template<typename T_OUTPUT_ITERATOR>
void getRandomGeoPoints(std::size_t count, const Bounds & bounds, T_OUTPUT_ITERATOR out) {
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::uniform_real_distribution<double> urdLat(bounds.minLat, bounds.maxLat);
	std::uniform_real_distribution<double> urdLon(bounds.minLon, bounds.maxLon); 
	std::default_random_engine gen(seed);
	
	GeoCoord c;
	for(std::size_t i(0); i < count; ++i) {
		c.lat = urdLat(gen);
		c.lon = urdLon(gen);
		*out = c;
		++out;
	}
}

template<typename T_OUTPUT_ITERATOR>
void getRandomPolarPoints(std::size_t number_of_points, T_OUTPUT_ITERATOR out) {
	using K = CGAL::Exact_predicates_inexact_constructions_kernel;
	using Rand = CGAL::Random_points_on_sphere_3<K::Point_3>;
	
	Rand rnd(1.0);
	LIB_RATSS_NAMESPACE::Projector proj;
	
	SphericalCoord c;
	for (std::size_t i(0); i < number_of_points; ++i) {
		K::Point_3 p = *rnd;
		++rnd;
		proj.toSpherical(p.x(), p.y(), p.z(), c.theta, c.phi, 128);
		*out = c;
		++out;
	}
}

template<typename T_OUTPUT_ITERATOR>
void readPoints(const std::string & fileName, T_OUTPUT_ITERATOR out) {
	std::ifstream inFile(fileName);
	
	if (!inFile.is_open()) {
		return;
	}
	
	GeoCoord c;
	while( !inFile.eof() ) {
		inFile >> c.lat >> c.lon;
		*out = c;
		++out;
	}
}

}

#endif
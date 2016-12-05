#ifndef LIB_RATSS_PRECISION_H
#define LIB_RATSS_PRECISION_H
#pragma once

#include <libratss/constants.h>

#include <limits>

namespace LIB_RATSS_NAMESPACE {

/** This defines the snapping precision for points that are not on the sphere
  * There are two modes:
  * Fixed: this means that the resulting rational will use up to maxBits bits
  * Variable:
  *     Try to calculate a point that has a squared distance to the input point of up to maxSquaredDistance.
  *     The resulting rational will uses at least minBits and up to maxBits bits to represent the point
**/
struct Precision {
	Precision(int _precision) :
	fixed(true), minBits(_precision), maxBits(_precision), maxSquaredDistance(std::numeric_limits<double>::max())
	{}
	Precision(int _minBits, int _maxBits, double _maxSquaredDistance) :
	fixed(false),
	minBits(_minBits),
	maxBits(_maxBits),
	maxSquaredDistance(_maxSquaredDistance)
	{}
	Precision(int _maxBits, double _maxSquaredDistance) :
	Precision(64, _maxBits, _maxSquaredDistance)
	{}
	bool fixed;
	int minBits;
	int maxBits;
	double maxSquaredDistance;
};

}//end namespace LIB_RATSS_NAMESPACE


#endif
#ifndef LIB_RATSS_ENUM_H
#define LIB_RATSS_ENUM_H
#pragma once

#include <libratss/constants.h>

namespace LIB_RATSS_NAMESPACE {

typedef enum : int {
	SP_INVALID = 0,
	
	SP_UPPER = 3,
	SP_LOWER = -3,
	
	SP_DIM1_POSITIVE = 1,
	SP_DIM1_NEGATIVE = -1,
	SP_DIM2_POSITIVE = 2,
	SP_DIM2_NEGATIVE = -2,
	SP_DIM3_POSITIVE = 3,
	SP_DIM3_NEGATIVE = -3
	//other values of the form SP_DIMK_POSITIVE = K are valid as well but not listed
	
} PositionOnSphere ;

typedef enum : int {
	ST_NONE=0x0,
	
	ST_SPHERE=0x1, //snap point on sphere
	ST_PLANE=ST_SPHERE*2, //snap point on plane
	ST_PAPER=ST_PLANE*2, //snap point on the plane based on a normalized version of the input point, computed using CORE
	ST_PAPER2=ST_PAPER*2, //snap point on the plane based on a normalized version of the input point, computed using CORE2
	
	ST_SNAP_POSITION_MASK=ST_SPHERE|ST_PLANE|ST_PAPER|ST_PAPER2,
	
	ST_CF=ST_PAPER2*2, //snap by continous fraction, compatible with values defined in Calc
	ST_FX=ST_CF*2, //snap by fix point
	ST_FL=ST_FX*2, //snap by floating point
	ST_JP=ST_FL*2, // jacobi perron
	///ST_FPLLL variants are all based on the Method described in Lagarias(1985) which uses the
	///LLL algorithm to produce a simultaneous approximation
	ST_FPLLL=ST_JP*2, //Maximum denominator of 1/epsilon or 2^significands
	ST_FPLLL_SCALED=ST_FPLLL*2, //Result within target epsilon, fast, possibly large denominator
	ST_FPLLL_GREEDY=ST_FPLLL_SCALED*2, ///Result within target epsilon, slow, close the the smallest possible denominator using the algorithm
	
	ST_SNAP_TYPES_MASK=ST_CF|ST_FX|ST_FL|ST_JP|ST_FPLLL|ST_FPLLL_SCALED|ST_FPLLL_GREEDY,

	ST_AUTO_CF=ST_FPLLL_GREEDY*2, //add cf to auto snapping
	ST_AUTO_FX=ST_AUTO_CF*2, //add fx to auto snapping
	ST_AUTO_FL=ST_AUTO_FX*2, //add fl to auto snapping
	ST_AUTO_JP=ST_AUTO_FL*2, //add jp to auto snapping
	ST_AUTO_FPLLL=ST_AUTO_JP*2, //add fplll to auto snapping
	ST_AUTO_FPLLL_SCALED=ST_AUTO_FPLLL*2,
	ST_AUTO_FPLLL_GREEDY=ST_AUTO_FPLLL_SCALED*2, //add fplll-greedy to auto snapping
	ST_AUTO=ST_AUTO_FPLLL*2, //select snapping that produces the smallest denominators
	ST_AUTO_ALL=ST_AUTO|ST_AUTO_CF|ST_AUTO_FX|ST_AUTO_JP|ST_AUTO_FPLLL|ST_AUTO_FPLLL_SCALED|ST_AUTO_FPLLL_GREEDY, //try all snappings and use the best one
	ST_AUTO_SNAP_MASK=ST_AUTO_ALL,
	
	ST_AUTO_POLICY_MIN_SUM_DENOM=ST_AUTO*2,
	ST_AUTO_POLICY_MIN_MAX_DENOM=ST_AUTO_POLICY_MIN_SUM_DENOM*2,
	ST_AUTO_POLICY_MIN_TOTAL_LIMBS=ST_AUTO_POLICY_MIN_MAX_DENOM*2,
	ST_AUTO_POLICY_MIN_SQUARED_DISTANCE=ST_AUTO_POLICY_MIN_TOTAL_LIMBS*2,
	ST_AUTO_POLICY_MIN_MAX_NORM=ST_AUTO_POLICY_MIN_SQUARED_DISTANCE*2,
	ST_AUTO_POLICY_MASK=ST_AUTO_POLICY_MIN_SUM_DENOM|ST_AUTO_POLICY_MIN_MAX_DENOM|ST_AUTO_POLICY_MIN_TOTAL_LIMBS|ST_AUTO_POLICY_MIN_SQUARED_DISTANCE|ST_AUTO_POLICY_MIN_MAX_NORM,
	
	ST_NORMALIZE=ST_AUTO_POLICY_MIN_MAX_NORM*2,
	//Do not use the values below!
	ST__INTERNAL_NUMBER_OF_SNAPPING_TYPES=7, //this effecivly defines the shift to get from ST_* to ST_AUTO_*
	ST__INTERNAL_AUTO_POLICIES=ST_AUTO_POLICY_MIN_SUM_DENOM|ST_AUTO_POLICY_MIN_MAX_DENOM|ST_AUTO_POLICY_MIN_TOTAL_LIMBS|ST_AUTO_POLICY_MIN_SQUARED_DISTANCE|ST_AUTO_POLICY_MIN_MAX_NORM,
	ST__INTERNAL_AUTO_ALL_WITH_POLICY=ST_AUTO_ALL|ST__INTERNAL_AUTO_POLICIES
} SnapType;

}//end namespace LIB_RATSS_NAMESPACE

#endif

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
	//other values of the form SP_DIMK_POSITIV = K are valid as well but not listed
	
} PositionOnSphere ;

}//end namespace LIB_RATSS_NAMESPACE

#endif
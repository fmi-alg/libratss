#ifndef LIB_RATSS_CONSTANTS_H
#define LIB_RATSS_CONSTANTS_H
#pragma once

#define LIB_RATSS_NAMESPACE ratss

#ifdef __GNUC__
	#define DEPRECATED __attribute__((deprecated))
	#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
	#define DEPRECATED __declspec(deprecated)
#else
	#define DEPRECATED
	#define WARN_UNUSED_RESULT
#endif

#endif

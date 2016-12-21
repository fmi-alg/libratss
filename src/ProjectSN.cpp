#include <libratss/ProjectSN.h>

namespace LIB_RATSS_NAMESPACE {

std::string ProjectSN::toString(ProjectSN::SnapType st) {
	std::string result;
	#define PRINT_FIELD_NAME(__NAME) if (st & __NAME) { result += #__NAME "|"; }
	
	PRINT_FIELD_NAME(ST_SPHERE)
	PRINT_FIELD_NAME(ST_PLANE)
	PRINT_FIELD_NAME(ST_CF)
	PRINT_FIELD_NAME(ST_FX)
	PRINT_FIELD_NAME(ST_FL)
	PRINT_FIELD_NAME(ST_JP)
	PRINT_FIELD_NAME(ST_NORMALIZE)
	
	if (result.size()) {
		result.pop_back();
	}
	return result;
}

}//end namespace LIB_RATSS_NAMESPACE
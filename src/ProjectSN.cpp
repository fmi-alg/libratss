#include <libratss/ProjectSN.h>

namespace LIB_RATSS_NAMESPACE {


ProjectSN::SnapConfig::SnapConfig() :
SnapConfig(ST_FX | ST_NORMALIZE | ST_PLANE, 53)
{}

ProjectSN::SnapConfig::SnapConfig(int st, int precision, int significands) :
m_st(st),
m_precision(precision),
m_significands(significands)
{}

ProjectSN::SnapConfig::SnapConfig(int st, int significands) :
m_st(st),
m_precision(-1),
m_significands(significands)
{
	if (significands < 1) {
		throw std::underflow_error("ratss::ProjectSN::SnapConfig: significands < 1");
	}
}

ProjectSN::SnapConfig::SnapConfig(const SnapConfig & other) :
m_st(other.m_st),
m_precision(other.m_precision),
m_significands(other.m_significands)
{}

ProjectSN::SnapConfig &
ProjectSN::SnapConfig::operator=(const SnapConfig & other) {
	m_st = other.m_st;
	m_precision = other.m_precision;
	m_significands = other.m_significands;
	return *this;
}

int ProjectSN::SnapConfig::snapType() const {
	return m_st;
}

int ProjectSN::SnapConfig::precision(int dimensions) const {
	if (m_precision < 0) {
		//computation precision depends on:
		//-number of dimensions
		//-specific computation
		//-significands
		//BUG: figure this out
		assert(false);
		return m_significands*dimensions;
	}
	else {
		return m_precision;
	}
}

int ProjectSN::SnapConfig::significands(int) const {
	return m_significands;
}

std::string ProjectSN::toString(ProjectSN::SnapType st) {
	std::string result;
	#define PRINT_FIELD_NAME(__NAME) if (st & __NAME) { result += #__NAME "|"; }
	
	PRINT_FIELD_NAME(ST_SPHERE)
	PRINT_FIELD_NAME(ST_PLANE)
	PRINT_FIELD_NAME(ST_CF)
	PRINT_FIELD_NAME(ST_FX)
	PRINT_FIELD_NAME(ST_FL)
	PRINT_FIELD_NAME(ST_JP)
	PRINT_FIELD_NAME(ST_FPLLL)
	PRINT_FIELD_NAME(ST_PAPER)
	PRINT_FIELD_NAME(ST_NORMALIZE)
	
	if (result.size()) {
		result.pop_back();
	}
	return result;
}

}//end namespace LIB_RATSS_NAMESPACE
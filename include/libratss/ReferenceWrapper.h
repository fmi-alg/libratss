#ifndef LIB_RATSS_REFERENCE_WRAPPER_H
#define LIB_RATSS_REFERENCE_WRAPPER_H

#include <libratss/constants.h>

namespace LIB_RATSS_NAMESPACE {

template<typename T>
class ReferenceWrapper {
public:
	using reference_type = T&;
public:
	explicit ReferenceWrapper(T & v) : m_p(&v) {}
	explicit ReferenceWrapper(const ReferenceWrapper & other) : m_p(other.m_p) {}
	ReferenceWrapper & operator=(const ReferenceWrapper & other) {
		m_p = other.m_p;
		return *this;
	}
	ReferenceWrapper & operator=(const T & v) {
		*m_p = v;
		return *this;
	}
	operator T() {
		return *m_p;
	}
private:
	T * m_p;
};

template<typename T>
class ReferenceWrapper<const T> {
public:
	using reference_type = const T&;
public:
	explicit ReferenceWrapper(const T & v) : m_p(&v) {}
	explicit ReferenceWrapper(const ReferenceWrapper & other) : m_p(other.m_p) {}
	ReferenceWrapper & operator=(const ReferenceWrapper & other) {
		m_p = other.m_p;
		return *this;
	}
	operator T() {
		return *m_p;
	}
private:
	const T * m_p;
};

}//end namespace

#endif
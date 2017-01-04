#ifndef LIB_RATSS_REFERENCE_WRAPPER_H
#define LIB_RATSS_REFERENCE_WRAPPER_H

#include <libratss/constants.h>
#include <iterator>

namespace LIB_RATSS_NAMESPACE {
namespace internal {

template<typename T>
class ReferenceWrapper {
public:
	using value_type = T;
	using reference_type = T&;
	static constexpr bool is_const_ref = false;
public:
	ReferenceWrapper(T & v) : m_p(&v) {}
	ReferenceWrapper(const ReferenceWrapper & other) : m_p(other.m_p) {}
	ReferenceWrapper & operator=(const ReferenceWrapper & other) {
		m_p = other.m_p;
		return *this;
	}
	ReferenceWrapper & operator=(const T & v) {
		*m_p = v;
		return *this;
	}
	reference_type get() {
		return *m_p;
	}
private:
	T * m_p;
};

template<typename T>
class ReferenceWrapper<const T> {
public:
	using value_type = T;
	using reference_type = const T&;
	static constexpr bool is_const_ref = true;
public:
	ReferenceWrapper(const T & v) : m_p(&v) {}
	ReferenceWrapper(const ReferenceWrapper & other) : m_p(other.m_p) {}
	ReferenceWrapper(const ReferenceWrapper<T> & other) : m_p(other.m_p) {} 
	ReferenceWrapper & operator=(const ReferenceWrapper & other) {
		m_p = other.m_p;
		return *this;
	}
	reference_type get() const {
		return *m_p;
	}
private:
	const T * m_p;
};

template<typename T_ITERATOR, typename T_ENABLE = void>
class ReferenceWrapperIterator;

template<typename T_ITERATOR>
class ReferenceWrapperIterator<T_ITERATOR, typename std::enable_if<!std::iterator_traits<T_ITERATOR>::value_type::is_const_ref>::type > :
	public std::iterator<std::input_iterator_tag, typename std::iterator_traits<T_ITERATOR>::value_type::value_type>
{
public:
	using wrapper_type = typename std::iterator_traits<T_ITERATOR>::value_type;
	using value_type = typename wrapper_type::value_type;
	using reference_type = typename wrapper_type::reference_type;
public:
	explicit ReferenceWrapperIterator(const T_ITERATOR & base) : m_base(base) {}
	ReferenceWrapperIterator(const ReferenceWrapperIterator & other) : m_base(other.m_base) {}
	ReferenceWrapperIterator(ReferenceWrapperIterator && other) : m_base(std::move(other.m_base)) {}
	ReferenceWrapperIterator & operator=(const ReferenceWrapperIterator & other) {
		m_base = other.m_base;
		return *this;
	}
	ReferenceWrapperIterator & operator=(ReferenceWrapperIterator && other) {
		m_base = std::move(other.m_base);
		return *this;
	}
	~ReferenceWrapperIterator() {}
public:
	ReferenceWrapperIterator & operator++() { ++m_base; return *this; }
	reference_type operator*() { return m_base->get(); }
	bool operator!=(const ReferenceWrapperIterator & other) { return m_base != other.m_base; }
	bool operator==(const ReferenceWrapperIterator & other) { return m_base == other.m_base; }
private:
	T_ITERATOR m_base;
};

template<typename T_ITERATOR>
class ReferenceWrapperIterator<T_ITERATOR, typename std::enable_if<std::iterator_traits<T_ITERATOR>::value_type::is_const_ref>::type > :
	public std::iterator<std::input_iterator_tag, const typename std::iterator_traits<T_ITERATOR>::value_type::value_type>
{
public:
	using wrapper_type = typename std::iterator_traits<T_ITERATOR>::value_type;
	using value_type = typename wrapper_type::value_type;
	using reference_type = typename wrapper_type::reference_type;
public:
	explicit ReferenceWrapperIterator(const T_ITERATOR & base) : m_base(base) {}
	ReferenceWrapperIterator(const ReferenceWrapperIterator & other) : m_base(other.m_base) {}
	ReferenceWrapperIterator(ReferenceWrapperIterator && other) : m_base(std::move(other.m_base)) {}
	ReferenceWrapperIterator & operator=(const ReferenceWrapperIterator & other) {
		m_base = other.m_base;
		return *this;
	}
	ReferenceWrapperIterator & operator=(ReferenceWrapperIterator && other) {
		m_base = std::move(other.m_base);
		return *this;
	}
	~ReferenceWrapperIterator() {}
public:
	ReferenceWrapperIterator & operator++() { ++m_base; return *this; }
	reference_type operator*() const { return m_base->get(); }
	bool operator!=(const ReferenceWrapperIterator & other) { return m_base != other.m_base; }
	bool operator==(const ReferenceWrapperIterator & other) { return m_base == other.m_base; }
private:
	T_ITERATOR m_base;
};

}}//end namespace internal::LIB_RATSS_NAMESPACE

#endif
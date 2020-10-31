#ifndef LIB_RATSS_INTERNAL_SKIP_ITERATOR_H
#define LIB_RATSS_INTERNAL_SKIP_ITERATOR_H
#pragma once

#include <libratss/constants.h>
#include <iterator>
#include <assert.h>

namespace LIB_RATSS_NAMESPACE {
namespace internal {

template<typename T_PARENT_CLASS, typename TIterator, typename TPointer = typename std::iterator_traits<TIterator>::pointer>
class SkipIteratorDerefer;


template<typename T_PARENT_CLASS, typename TIterator, typename TValue>
class SkipIteratorDerefer<T_PARENT_CLASS, TIterator, TValue*>: public
	std::iterator<
		std::forward_iterator_tag,
		typename std::iterator_traits<TIterator>::value_type,
		typename std::iterator_traits<TIterator>::difference_type,
		typename std::iterator_traits<TIterator>::pointer,
		typename std::iterator_traits<TIterator>::reference
	>
{
public:
	TValue & operator*() { return *(static_cast<T_PARENT_CLASS*>(this)->it()); }
};

template<typename T_PARENT_CLASS, typename TIterator, typename TValue>
class SkipIteratorDerefer<T_PARENT_CLASS, TIterator, const TValue*>: public
	std::iterator<
		std::input_iterator_tag,
		typename std::iterator_traits<TIterator>::value_type,
		typename std::iterator_traits<TIterator>::difference_type,
		typename std::iterator_traits<TIterator>::pointer,
		typename std::iterator_traits<TIterator>::reference
	>
{
public:
	const TValue & operator*() const { return *(static_cast<const T_PARENT_CLASS*>(this)->it()); }
};

template<typename TIterator>
class SkipIteratorBase: public SkipIteratorDerefer<SkipIteratorBase<TIterator>, TIterator> {
public:
	using base_iterator_type = TIterator;
	using MyBaseClass = SkipIteratorDerefer<SkipIteratorBase<TIterator>, TIterator>;
	using value_type = typename MyBaseClass::value_type;
	using difference_type = typename MyBaseClass::difference_type;
	using reference = typename MyBaseClass::reference;
	using pointer = typename MyBaseClass::pointer;
public:
	///@param dimSkip starts from 1, set to 0 for past the end iterator
	explicit SkipIteratorBase(const base_iterator_type & iter, int skipDim) : m_it(iter) {
		assert(skipDim >= 0);
		if (skipDim == 1) {
			++m_it;
			m_skip = 0;
		}
		else if (skipDim == 0){
			m_skip = 0;
		}
		else {
			m_skip = skipDim-1;
		}
	}
	SkipIteratorBase(const SkipIteratorBase & other) : m_it(other.m_it), m_skip(other.m_skip) {}
	SkipIteratorBase & operator=(const SkipIteratorBase & other) {
		m_it = other.m_it;
		m_skip = other.m_skip;
		return *m_it;
	}
	~SkipIteratorBase() {}
	SkipIteratorBase & operator++() {
		if (m_skip == 1) {
			++m_it;
		}
		++m_it;
		if (m_skip > 0) {
			--m_skip;
		}
		return *this;
	}
	difference_type operator-(const SkipIteratorBase & other) const {
		//we have multiple cases here: skip1 < skip2, skip2 < skip1, skip1 == skip2
		//the order of the ifs is relevant!
		
		using std::distance;
		
		if (m_skip == 0 && other.m_skip == 0) { //both are after the skip
			return distance(other.m_it, m_it);
		}
		
		//other.m_skip != 0 -> m_it is before the skip -> skip is between
		//m_skip == 0 -> other.m_it is before the skip -> skip is between
		if (m_skip == 0 || other.m_skip == 0) {
			return distance(other.m_it, m_it) - 1;
		}
		
		//both are before the skip
		return distance(other.m_it, m_it);
	}
	bool operator==(const SkipIteratorBase & other) const { return m_it == other.m_it; }
	bool operator!=(const SkipIteratorBase & other) const { return m_it != other.m_it; }
public:
	const base_iterator_type & it() const { return m_it; }
	base_iterator_type & it() { return m_it; }
protected:
	base_iterator_type m_it;
	std::size_t m_skip;
};

template<typename TIterator>
using SkipIterator = SkipIteratorBase<TIterator>;

}}//end namespace LIB_RATSS_NAMESPACE::internal

namespace std {
	template<typename TIterator>
	typename LIB_RATSS_NAMESPACE::internal::SkipIterator<TIterator>::difference_type
	distance(const LIB_RATSS_NAMESPACE::internal::SkipIterator<TIterator> & __first, const LIB_RATSS_NAMESPACE::internal::SkipIterator<TIterator> & __last) {
		return __last.operator-( __first);
	}
}

#endif
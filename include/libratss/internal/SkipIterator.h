#ifndef LIB_RATSS_INTERNAL_SKIP_ITERATOR_H
#define LIB_RATSS_INTERNAL_SKIP_ITERATOR_H
#pragma once

#include <libratss/constants.h>
#include <iterator>
#include <assert.h>

namespace LIB_RATSS_NAMESPACE {
namespace internal {

template<typename T_PARENT_CLASS, typename TIterator, typename TValue = typename std::iterator_traits<base_iterator_type>::value_type>
class SkipIteratorDerefer: public
	std::iterator<
		std::forward_iterator_tag,
		typename std::iterator_traits<TIterator>::value_type,
		typename std::iterator_traits<TIterator>::difference_type,
		typename std::iterator_traits<TIterator>::pointer_type,
		typename std::iterator_traits<TIterator>::reference_type
	>
{
public:
	T & operator*() { return *(T_PARENT_CLASS::it()); }
};

template<typename T_PARENT_CLASS, typename TIterator, typename TValue>
struct SkipIteratorDerefer<T_PARENT_CLASS, TIterator, const TValue>: public
	std::iterator<
		std::forward_iterator_tag,
		typename std::iterator_traits<TIterator>::value_type,
		typename std::iterator_traits<TIterator>::difference_type,
		typename std::iterator_traits<TIterator>::pointer_type,
		typename std::iterator_traits<TIterator>::reference_type
	>
{
public:
	const T & operator*() const { return *(T_PARENT_CLASS::it()); }
};

template<typename T>
class SkipIteratorBase: public SkipIteratorDerefer<SkipIteratorBase<T>, T> {
public:
	using base_iterator_type = TIterator;
protected:
	SkipIteratorBase(const base_iterator_type & it, std::size_t skip) : m_it(it), m_skip(skip) {
		if (skip == 0) {
			++m_it;
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
	}
	difference_type operator-(const SkipIteratorBase & other) {
		//we have multiple cases here: skip1 < skip2, skip2 < skip1, skip1 == skip2
		if (m_skip < other.m_skip) {
		
		}
		else if (other.m_skip < m_skip) {
			
		}
		else { //m_skip == other.m_skip
			assert(m_it == other.m_it);
			return 0;
		}
	}
	bool operator==(const SkipIterator & other) { return m_it == other.m_it; }
	bool operator!=(const SkipIterator & other) { return m_it != other.m_it; }
protected:
	const base_iterator_type & it() const;
	base_iterator_type & it();
protected:
	base_iterator_type m_it;
	std::size_t m_skip;
};

template<typename TIterator>
using SkipIterator = SkipIteratorBase<TIterator>;

}}//end namespace LIB_RATSS_NAMESPACE::internal

#endif
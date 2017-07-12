#ifndef LIBRATSS_EXTENDED_INT_H
#define LIBRATSS_EXTENDED_INT_H

#include <CGAL/Gmpz.h>
#include <stdint.h>
#include <stdexcept>
#include <type_traits>

namespace CGAL {

class ExtendedInt final {
public:
	using base_type = int64_t;
	using unsigned_base_type = typename std::make_unsigned<base_type>::type;
	using extension_type = CGAL::Gmpz;
	static_assert( --std::numeric_limits<base_type>::min() == std::numeric_limits<base_type>::min(), "");
public:
	ExtendedInt() : m_extended(false) { set(0); }
	ExtendedInt(base_type v) : m_extended(false) { set(v); }
	explicit ExtendedInt(const extension_type & v) : m_extended(false) {
		if (v > nlmax || v < nlmin) {
			set(v);
		}
		else {
			set( static_cast<base_type>( v ) );
		}
	}
	~ExtendedInt() {
		if (isExtended()) {
			delete m_v.ptr;
			m_extended = false;
		}
		m_v.ptr = 0;
	}
	ExtendedInt(const ExtendedInt & other) : m_extended(false) {
		operator=(other);
	}
	ExtendedInt(ExtendedInt && other) : m_extended(false) {
		operator=(std::move(other));
	}
	ExtendedInt & operator=(const ExtendedInt & other) {
		if (other.isExtended()) {
			set(other.getExtened());
		}
		else {
			set(other.get());
		}
		return *this;
	}
	ExtendedInt & operator=(ExtendedInt && other) {
		if (other.isExtended()) {
			if (isExtended()) {
				using std::swap;
				swap(m_v.ptr, other.m_v.ptr);
			}
			else {
				m_v.ptr = other.m_v.ptr;
				m_extended = true;
				other.m_v.ptr = 0;
				other.m_extended = false;
			}
		}
		else {
			set(other.get());
		}
		return *this;
	}
public:
	ExtendedInt operator-() const {
		if (isExtended()) {
			return ExtendedInt( -getExtened() );
		}
		else {
			return ExtendedInt( -get() );
		}
	}
	ExtendedInt operator+() const {
		if (isExtended()) {
			return ExtendedInt( getExtened() );
		}
		else {
			return ExtendedInt( get() );
		}
	}
	ExtendedInt operator+(const ExtendedInt & other) const {
		if (isExtended() || other.isExtended()) {
			if (!isExtended()) {
				return ExtendedInt( extension_type(get()) + other.getExtened() );
			}
			else if (!other.isExtended()) {
				return ExtendedInt( getExtened() + extension_type(other.get()));
			}
			else {
				return ExtendedInt( getExtened() + other.getExtened() );
			}
		}
		else {
			using ubt = unsigned_base_type;
			base_type myv = get();
			base_type ov = other.get();
			if (
				(myv < 0 && ov < 0 && ubt(-myv) + ubt(-ov) > ubt(-nlmin) ) ||
				(myv > 0 && ov > 0 && ubt(myv) + ubt(ov) > ubt(nlmax) )
			){
				return ExtendedInt( extension_type(myv) + extension_type(ov) );
			}
			return ExtendedInt(get() + other.get());
		}
	}
	ExtendedInt operator-(const ExtendedInt & other) const {
		return *this + (-other);
	}
	
	ExtendedInt & operator+=(const ExtendedInt & other) {
		m_v = (*this + other).m_v;
		return *this;
	}
	ExtendedInt & operator-=(const ExtendedInt & other) {
		m_v = (*this - other).m_v;
		return *this;
	}
	ExtendedInt & operator*=(const ExtendedInt & other) {
		m_v = (*this * other).m_v;
		return *this;
	}
	bool operator==(const ExtendedInt & other) const {
		return asExtended() == other.asExtended();
	}
	bool operator!=(const ExtendedInt & other) const {
		return asExtended() != other.asExtended();
	}
	bool operator<(const ExtendedInt & other) const {
		return asExtended() < other.asExtended();
	}
	bool operator<=(const ExtendedInt & other) const {
		return asExtended() <= other.asExtended();
	}
	bool operator>(const ExtendedInt & other) const {
		return asExtended() > other.asExtended();
	}
	bool operator>=(const ExtendedInt & other) const {
		return asExtended() > other.asExtended();
	}
private:
	static constexpr base_type nlmax = std::numeric_limits<base_type>::max();
	static constexpr base_type nlmin = std::numeric_limits<base_type>::min();
private:
	bool isExtended() { return m_extended; }
	base_type get() { return m_v.i; }
	const extension_type & getExtened() const { return *m_v.ptr;}
	void set(base_type v) {
		if (isExtended()) {
			delete m_v.ptr;
			m_extended = false;
		}
		m_v.i = v;
	}
	void set(const extension_type & v) {
		if (isExtended()) {
			*m_v.ptr = v;
		}
		else {
			m_v.ptr = new extension_type(v);
			m_extended = true;
		}
	}
	
	extension_type asExtended() const {
		if (isExtended()) {
			return getExtened();
		}
		else {
			return extension_type( get() );
		}
	}
	
private:
	union {
		base_type i; 
		extension_type * ptr;
	} m_v;
	bool m_extended;
};

}//end namespace

#endif
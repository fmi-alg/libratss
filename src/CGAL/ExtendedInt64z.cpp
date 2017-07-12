#include <libratss/CGAL/ExtendedInt64z.h>

namespace CGAL {

ExtendedInt64z::ExtendedInt64z() :
m_isExtended(false)
{
	m_v.i = 0;
}

ExtendedInt64z::ExtendedInt64z(const ExtendedInt64z & other) :
m_isExtended(false)
{
	if (other.isExtended()) {
		set(other.getExtended());
	}
	else {
		m_v.i = other.get();
	}
}

ExtendedInt64z::ExtendedInt64z(ExtendedInt64z && other) :
m_isExtended(false)
{
	if (other.isExtended()) {
		m_v.ptr = other.m_v.ptr;
		m_isExtended = true;
		
		other.m_v.ptr = 0;
		other.m_isExtended = false;
	}
	else {
		m_v.i = other.m_v.i;
	}
}

ExtendedInt64z::ExtendedInt64z(const Gmpz & z) :
m_isExtended(false)
{
	set(z);
}

ExtendedInt64z::ExtendedInt64z(base_type i) :
m_isExtended(false)
{
	set(i);
}

ExtendedInt64z::ExtendedInt64z(int32_t i) :
ExtendedInt64z(base_type(i))
{}

ExtendedInt64z::ExtendedInt64z(uint64_t l) :
m_isExtended(false)
{
	if (l < (uint64_t)btmax) {
		set(base_type(l));
	}
	else {
		set(extension_type(l));
	}
}

ExtendedInt64z::ExtendedInt64z(double d) :
m_isExtended(false)
{
	if (double(base_type(d)) == d) {
		set(base_type(d));
	}
	else {
		set( extension_type(d) );
	}
}

ExtendedInt64z::ExtendedInt64z(const std::string& str, int base) :
m_isExtended(false)
{
	set(extension_type(str, base));
}


ExtendedInt64z& ExtendedInt64z::operator=(const ExtendedInt64z & other) {
	if (other.isExtended()) {
		set( other.getExtended() );
	}
	else {
		set( other.get() );
	}
	return *this;
}

ExtendedInt64z& ExtendedInt64z::operator=(ExtendedInt64z && other) {
	if (isExtended() && other.isExtended()) {
		using std::swap;
		swap(m_v.ptr, other.m_v.ptr);
	}
	else {
		set(other.get());
	}
	return *this;
}

size_t ExtendedInt64z::size() const {
	return sizeof(m_v) + sizeof(m_isExtended) + (isExtended() ? getExtended().size() : 0);
}

double ExtendedInt64z::to_double() const {
	return asExtended().to_double();
}

Sign ExtendedInt64z::sign() const {
	if (isExtended()) {
		return getExtended().sign();
	}
	else {
		return CGAL::sign(get());
	}
}

ExtendedInt64z & ExtendedInt64z::operator+=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() += other.getExtended();
	}
	else if (isExtended()) {
		getExtended() += other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() + other.getExtended());
	}
	else {
		set(asExtended() + other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator-=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() -= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() -= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() - other.getExtended());
	}
	else {
		set(asExtended() - other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator*=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() *= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() *= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() * other.getExtended());
	}
	else {
		set(asExtended() * other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator/=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() /= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() /= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() / other.getExtended());
	}
	else {
		get() /= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator%=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() %= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() %= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() % other.getExtended());
	}
	else {
		get() %= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator&=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() &= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() &= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() & other.getExtended());
	}
	else {
		get() &= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator|=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() |= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() |= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() | other.getExtended());
	}
	else {
		get() |= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator^=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() ^= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() ^= other.get();
	}
	else if (other.isExtended()) {
		set(asExtended() ^ other.getExtended());
	}
	else {
		get() ^= other.get();
	}
	return *this;
}

bool ExtendedInt64z::operator<(const ExtendedInt64z & other) const {
	if (isExtended() && other.isExtended()) {
		return getExtended() < other.getExtended();
	}
	else if (isExtended()) {
		return getExtended() < other.get();
	}
	else if (other.isExtended()) {
		return get() < other.getExtended();
	}
	else {
		return get() < other.get();
	}
}

bool ExtendedInt64z::operator==(const ExtendedInt64z & other) const {
	if (isExtended() && other.isExtended()) {
		return getExtended() < other.getExtended();
	}
	else if (isExtended()) {
		return getExtended() < other.get();
	}
	else if (other.isExtended()) {
		return get() < other.getExtended();
	}
	else {
		return get() < other.get();
	}
}

ExtendedInt64z ExtendedInt64z::operator+() const {
	return *this;
}

ExtendedInt64z ExtendedInt64z::operator-() const {
	if (isExtended()) {
		return ExtendedInt64z( -getExtended() );
	}
	else {
		return ExtendedInt64z( -get() );
	}
}

ExtendedInt64z & ExtendedInt64z::operator<<=(const unsigned long& i) {
	if (isExtended()) {
		getExtended() <<= i;
	}
	else {
		if (false) { //TODO: do correct overflow check here
			get() <<= i;
		}
		else {
			set( asExtended() << i );
		}
	}
	return *this;
}

ExtendedInt64z& ExtendedInt64z::operator>>= (const unsigned long& i) {
	if (isExtended()) {
		getExtended() >>= i;
	}
	else {
		if (false) { //TODO: do correct overflow check here
			get() <<= i;
		}
		else {
			set( asExtended() << i );
		}
	}
	return *this;
}

ExtendedInt64z& ExtendedInt64z::operator++() {
	if (isExtended()) {
		++getExtended();
	}
	else {
		if (get() < btmax) {
			++get();
		}
		else {
			set( ++asExtended());
		}
	}
	return *this;
}

ExtendedInt64z& ExtendedInt64z::operator--() {
	if (isExtended()) {
		--getExtended();
	}
	else {
		if (get() > btmin) {
			--get();
		}
		else {
			set( --asExtended());
		}
	}
	return *this;
}

bool ExtendedInt64z::isExtended() const {
	return m_isExtended;
}

ExtendedInt64z::base_type & ExtendedInt64z::get() {
	assert(!isExtended());
	return m_v.i;
}

const ExtendedInt64z::base_type & ExtendedInt64z::get() const {
	assert(!isExtended());
	return m_v.i;
}

ExtendedInt64z::extension_type & ExtendedInt64z::getExtended() {
	assert(isExtended());
	return *m_v.ptr;
}

const ExtendedInt64z::extension_type & ExtendedInt64z::getExtended() const {
	assert(isExtended());
	return *m_v.ptr;
}

ExtendedInt64z::extension_type ExtendedInt64z::asExtended() const {
	if (isExtended()) {
		return getExtended();
	}
	else {
		return extension_type( get() );
	}
}

void ExtendedInt64z::set(base_type v) {
	if (isExtended()) {
		delete m_v.ptr;
		m_isExtended = false;
	}
	m_v.i = v;
}

void ExtendedInt64z::set(const extension_type & v) {
	if (isExtended()) {
		*m_v.ptr = v;
	}
	else {
		m_v.ptr = new extension_type(v);
		m_isExtended = true;
	}
}

}//end namespace
#include <libratss/CGAL/ExtendedInt64z.h>

namespace CGAL {

uint64_t ExtendedInt64z::number_of_extended_allocations = 0;

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
		set(other.get());
	}
}

ExtendedInt64z::ExtendedInt64z(ExtendedInt64z && other) :
m_isExtended(false)
{
	if (other.isExtended()) {
		set(m_v.ptr);
		other.set((extension_type*)0);
	}
	else {
		set(other.get());
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
		set(config_traits::make(l));
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
	set( extension_type(str, base) );
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
		deleteExt();
		set(other.ptr());
		other.set((extension_type*)0);
	}
	else if (other.isExtended()) {
		set(other.ptr());
		other.set((extension_type*)0);
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
	if (isExtended()) {
		return getExtended().to_double();
	}
	else {
		return get();
	}
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
		getExtended() += other.getExtendedPrimitive();
	}
	else if (other.isExtended()) {
		set(asExtended() + other.getExtended());
	}
	else {//TODO: fast imp
		set(asExtended() + other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator-=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() -= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() -= other.getExtendedPrimitive();
	}
	else if (other.isExtended()) {
		set(asExtended() - other.getExtended());
	}
	else {//TODO: fast imp
		set(asExtended() - other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator*=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() *= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() *= other.getExtendedPrimitive();
	}
	else if (other.isExtended()) {
		set(asExtended() * other.getExtended());
	}
	else {//TODO: fast imp
		set(asExtended() * other.asExtended());
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator/=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() /= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() /= other.getExtendedPrimitive();
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
		getExtended() %= other.getExtendedPrimitive();
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
		getExtended() &= other.getExtendedPrimitive();
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
		getExtended() |= other.getExtendedPrimitive();
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
		getExtended() ^= other.getExtendedPrimitive();
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
		return getExtended() < other.getExtendedPrimitive();
	}
	else if (other.isExtended()) {
		return getExtendedPrimitive() < other.getExtended();
	}
	else {
		return get() < other.get();
	}
}

bool ExtendedInt64z::operator==(const ExtendedInt64z & other) const {
	if (isExtended() && other.isExtended()) {
		return getExtended() == other.getExtended();
	}
	else if (isExtended()) {
		return getExtended() == other.getExtendedPrimitive();
	}
	else if (other.isExtended()) {
		return getExtendedPrimitive() == other.getExtended();
	}
	else {
		return get() == other.get();
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
			get() >>= i;
		}
		else {
			set( asExtended() >> i );
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
			set( ++asExtended() );
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
			set( --asExtended() );
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
	return *ptr();
}

const ExtendedInt64z::extension_type & ExtendedInt64z::getExtended() const {
	assert(isExtended());
	return *ptr();
}

ExtendedInt64z::extension_type ExtendedInt64z::asExtended() const {
	if (isExtended()) {
		return getExtended();
	}
	else {
		return config_traits::make( get() );
	}
}

ExtendedInt64z::config_traits::primitive_type ExtendedInt64z::getExtendedPrimitive() const {
	return config_traits::make_primitive(get());
}

ExtendedInt64z::extension_type* ExtendedInt64z::ptr() const {
	assert(isExtended());
	return m_v.ptr;
}

void ExtendedInt64z::set(base_type v) {
	if (isExtended()) {
		deleteExt();
	}
	m_v.i = v;
}

void ExtendedInt64z::set(const extension_type & v) {
	if (::mpz_fits_slong_p(v.mpz())) {
		set( ::mpz_get_si(v.mpz()) );
	}
	else {
		if (isExtended()) {
			getExtended() = v;
		}
		else {
			set( new extension_type(v) );
			++number_of_extended_allocations;
		}
	}
}

void ExtendedInt64z::set(ExtendedInt64z::extension_type* v) {
	m_isExtended = v;
	m_v.ptr = v;
}

void ExtendedInt64z::deleteExt() {
	assert(isExtended());
	delete m_v.ptr;
	--number_of_extended_allocations;
	set( (extension_type*)0 );
}

namespace internal {

ExtendedInt64zTraits<CGAL::Gmpz>::type
ExtendedInt64zTraits<CGAL::Gmpz>::make(int64_t v) {
	return type(LIB_RATSS_NAMESPACE::gmp_int64_t(v));
}

ExtendedInt64zTraits<CGAL::Gmpz>::type
ExtendedInt64zTraits<CGAL::Gmpz>::make(uint64_t v) {
	return type(LIB_RATSS_NAMESPACE::gmp_uint64_t(v));
}

ExtendedInt64zTraits<CGAL::Gmpz>::primitive_type
ExtendedInt64zTraits<CGAL::Gmpz>::make_primitive(int64_t v) {
	return primitive_type(v);
}

}//end namespace internal


}//end namespace

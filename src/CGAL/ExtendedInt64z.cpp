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
		set(traits().make()(l));
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
		traits().addAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().addAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().add()(asExtended(), other.getExtended()));
	}
	else {//TODO: fast imp
		set(traits().add()(asExtended(),other.asExtended()));
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator-=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().subAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().subAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().sub()(asExtended(), other.getExtended()));
	}
	else {//TODO: fast imp
		set(traits().sub()(asExtended(), other.asExtended()));
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator*=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().multAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().multAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().mult()(asExtended(), other.getExtended()));
	}
	else {//TODO: fast imp
		set(traits().mult()(asExtended(), other.asExtended()));
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator/=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().divAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().divAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().div()(asExtended(), other.getExtended()));
	}
	else {
		get() /= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator%=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().modAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().modAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().mod()(asExtended(), other.getExtended()));
	}
	else {
		get() %= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator&=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().bitAndAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().bitAndAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().bitAnd()(asExtended(), other.getExtended()));
	}
	else {
		get() &= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator|=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().bitOrAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().bitOrAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().bitOr()(asExtended(), other.getExtended()));
	}
	else {
		get() |= other.get();
	}
	return *this;
}

ExtendedInt64z & ExtendedInt64z::operator^=(const ExtendedInt64z & other) {
	if (isExtended() && other.isExtended()) {
		traits().bitXorAssign()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		traits().bitXorAssign()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		set(traits().bitXor()(asExtended(), other.getExtended()));
	}
	else {
		get() ^= other.get();
	}
	return *this;
}

bool ExtendedInt64z::operator<(const ExtendedInt64z & other) const {
	if (isExtended() && other.isExtended()) {
		return traits().less()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		return traits().less()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		return traits().less()(get(), other.getExtended());
	}
	else {
		return get() < other.get();
	}
}

bool ExtendedInt64z::operator==(const ExtendedInt64z & other) const {
	if (isExtended() && other.isExtended()) {
		return traits().equal()(getExtended(), other.getExtended());
	}
	else if (isExtended()) {
		return traits().equal()(getExtended(), other.get());
	}
	else if (other.isExtended()) {
		return traits().equal()(get(), other.getExtended());
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
		traits().lShiftAssign()(getExtended(), uint64_t(i));
	}
	else {
		if (false) { //TODO: do correct overflow check here
			get() <<= i;
		}
		else {
			set( traits().lShift()(asExtended(), uint64_t(i)) );
		}
	}
	return *this;
}

ExtendedInt64z& ExtendedInt64z::operator>>= (const unsigned long& i) {
	if (isExtended()) {
		traits().rShiftAssign()(getExtended(), uint64_t(i));
	}
	else {
		if (false) { //TODO: do correct overflow check here
			get() >>= i;
		}
		else {
			set( traits().rShift()(asExtended(), uint64_t(i)) );
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
		return traits().make()( get() );
	}
}

ExtendedInt64z::config_traits ExtendedInt64z::traits() const {
	return config_traits();
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
	if (traits().fits<base_type>()(v)) {
		set( traits().toIntegral<base_type>()(v.mpz()) );
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


}//end namespace

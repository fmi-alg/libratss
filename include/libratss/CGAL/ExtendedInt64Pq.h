//This file is based on the Gmpq implementation of CGAL.
//Original Header follows

// Copyright (c) 2002,2003  
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved. 
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Andreas Fabri, Sylvain Pion

//End of original header


#ifndef LIBRATSS_CGAL_EXTENDED_INT_64_PQ_H
#define LIBRATSS_CGAL_EXTENDED_INT_64_PQ_H

#include <CGAL/Gmpq.h>

#include <libratss/constants.h>
#include <libratss/CGAL/ExtendedInt64z.h>
#include <boost/multiprecision/rational_adaptor.hpp>

namespace CGAL {
namespace internal {
	
	struct ExtendedInt64PqTraits {
		
	};
	
}

template<typename T_EXTENSION_TYPE = CGAL::Gmpq>
class ExtendedInt64Pq:
	boost::totally_ordered1< ExtendedInt64Pq<T_EXTENSION_TYPE>
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, int
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, long
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, long long
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, double
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, Gmpq
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, Gmpz
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, Gmpfr
	, boost::ordered_field_operators2< ExtendedInt64Pq<T_EXTENSION_TYPE>, ExtendedInt64z
		> > > > > > > > >
{
public:
	typedef Tag_false  Has_gcd;
	typedef Tag_true   Has_division;
	typedef Tag_false  Has_sqrt;

	typedef Tag_true   Has_exact_ring_operations;
	typedef Tag_true   Has_exact_division;
	typedef Tag_false  Has_exact_sqrt;
public:
	typedef CGAL::ExtendedInt64z::base_type base_type;
	typedef T_EXTENSION_TYPE extension_type;
	typedef ExtendedInt64z numerator_type;
	typedef ExtendedInt64z denominator_type;
public:
	static uint64_t number_of_extended_allocations;
	static uint64_t number_of_allocations;
public:
	ExtendedInt64Pq();
	ExtendedInt64Pq(const ExtendedInt64Pq & other);
	ExtendedInt64Pq(ExtendedInt64Pq&& other);

	ExtendedInt64Pq(const CGAL::Gmpq & q);

	ExtendedInt64Pq(int32_t n);
	ExtendedInt64Pq(uint32_t n);
	ExtendedInt64Pq(base_type n);
	ExtendedInt64Pq(uint64_t n);
	ExtendedInt64Pq(const Gmpz& n);
	ExtendedInt64Pq(const ExtendedInt64z & n);
	ExtendedInt64Pq(const Gmpfr &f);

	ExtendedInt64Pq(int32_t n, int32_t d);
	ExtendedInt64Pq(base_type n, base_type d);
	ExtendedInt64Pq(base_type n, uint64_t d);
	ExtendedInt64Pq(uint64_t n, uint64_t d);
	ExtendedInt64Pq(const numerator_type& n, const denominator_type& d);
	ExtendedInt64Pq(const Gmpz& n, const Gmpz& d);
	ExtendedInt64Pq(double d);
	
	ExtendedInt64Pq(const std::string& str, int base = 10);
	
	~ExtendedInt64Pq();
	
	ExtendedInt64Pq& operator=(const ExtendedInt64Pq & other);
	ExtendedInt64Pq& operator=(ExtendedInt64Pq && other);
public:
	// Gives the memory size in bytes. (not documented yet)
	std::size_t size() const;
	
	void canonicalize();

	numerator_type  numerator() const;
	denominator_type denominator() const;

	ExtendedInt64Pq operator+() const;
	ExtendedInt64Pq operator-() const;

	ExtendedInt64Pq& operator+=(const ExtendedInt64Pq &q);
	ExtendedInt64Pq& operator-=(const ExtendedInt64Pq &q);
	ExtendedInt64Pq& operator*=(const ExtendedInt64Pq &q);
	ExtendedInt64Pq& operator/=(const ExtendedInt64Pq &q);
	
	ExtendedInt64Pq operator+(const ExtendedInt64Pq & other) const;
	ExtendedInt64Pq operator-(const ExtendedInt64Pq& other) const;
	ExtendedInt64Pq operator*(const ExtendedInt64Pq & other) const;
	ExtendedInt64Pq operator/(const ExtendedInt64Pq & other) const;

	bool operator==(const ExtendedInt64Pq &q) const;
	bool operator< (const ExtendedInt64Pq &q) const;

	double to_double() const;
	Sign sign() const;

	bool isExtended() const;
	const extension_type & getExtended() const;
	extension_type & getExtended();
	extension_type asExtended() const;
	
	inline operator extension_type() const { return asExtended(); }

	// Interoperability with int32_t
	inline ExtendedInt64Pq& operator+=(int32_t z) { return (*this) += ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator-=(int32_t z) { return (*this) -= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator*=(int32_t z) { return (*this) *= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator/=(int32_t z) { return (*this) /= ExtendedInt64Pq(z); }
	inline bool operator==(int32_t z) const {return *this == ExtendedInt64Pq(z); }
	inline bool operator< (int32_t z) const {return *this < ExtendedInt64Pq(z); }
	inline bool operator> (int32_t z) const {return *this > ExtendedInt64Pq(z); }

	// Interoperability with int64_t
	inline ExtendedInt64Pq& operator+=(int64_t z) { return (*this) += ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator-=(int64_t z) { return (*this) -= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator*=(int64_t z) { return (*this) *= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator/=(int64_t z) { return (*this) /= ExtendedInt64Pq(z); }
	inline bool operator==(int64_t z) const { return *this == ExtendedInt64Pq(z); }
	inline bool operator< (int64_t z) const { return *this < ExtendedInt64Pq(z); }
	inline bool operator> (int64_t z) const { return *this > ExtendedInt64Pq(z); }

	// Interoperability with uint64_t
	inline ExtendedInt64Pq& operator+=(uint64_t z) { return (*this) += ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator-=(uint64_t z) { return (*this) -= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator*=(uint64_t z) { return (*this) *= ExtendedInt64Pq(z); }
	inline ExtendedInt64Pq& operator/=(uint64_t z) { return (*this) /= ExtendedInt64Pq(z); }
	inline bool operator==(uint64_t z) const { return (*this) == ExtendedInt64Pq(z); }
	inline bool operator< (uint64_t z) const { return (*this) < ExtendedInt64Pq(z); }
	inline bool operator> (uint64_t z) const { return (*this) > ExtendedInt64Pq(z); }

	// Interoperability with double
	inline ExtendedInt64Pq& operator+=(double d) { return (*this) += ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator-=(double d) { return (*this) -= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator*=(double d) { return (*this) *= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator/=(double d) { return (*this) /= ExtendedInt64Pq(d); }
	inline bool operator==(double d) const { return (*this) == ExtendedInt64Pq(d); }
	inline bool operator< (double d) const { return (*this) < ExtendedInt64Pq(d); }
	inline bool operator> (double d) const { return (*this) > ExtendedInt64Pq(d); }

	// Interoperability with Gmpq
	inline ExtendedInt64Pq& operator+=(Gmpq d) { return (*this) += ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator-=(Gmpq d) { return (*this) -= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator*=(Gmpq d) { return (*this) *= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator/=(Gmpq d) { return (*this) /= ExtendedInt64Pq(d); }
	inline bool operator==(Gmpq d) const { return (*this) == ExtendedInt64Pq(d); }
	inline bool operator< (Gmpq d) const { return (*this) < ExtendedInt64Pq(d); }
	inline bool operator> (Gmpq d) const { return (*this) > ExtendedInt64Pq(d); }

	// Interoperability with Gmpz
	inline ExtendedInt64Pq& operator+=(Gmpz d) { return (*this) += ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator-=(Gmpz d) { return (*this) -= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator*=(Gmpz d) { return (*this) *= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator/=(Gmpz d) { return (*this) /= ExtendedInt64Pq(d); }
	inline bool operator==(Gmpz d) const { return (*this) == ExtendedInt64Pq(d); }
	inline bool operator< (Gmpz d) const { return (*this) < ExtendedInt64Pq(d); }
	inline bool operator> (Gmpz d) const { return (*this) > ExtendedInt64Pq(d); }

	// Interoperability with Gmpfr
	inline ExtendedInt64Pq& operator+=(Gmpfr d) { return (*this) += ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator-=(Gmpfr d) { return (*this) -= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator*=(Gmpfr d) { return (*this) *= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator/=(Gmpfr d) { return (*this) /= ExtendedInt64Pq(d); }
	inline bool operator==(Gmpfr d) const { return (*this) == ExtendedInt64Pq(d); }
	inline bool operator< (Gmpfr d) const { return (*this) < ExtendedInt64Pq(d); }
	inline bool operator> (Gmpfr d) const { return (*this) > ExtendedInt64Pq(d); }

	// Interoperability with ExtendedInt
	inline ExtendedInt64Pq& operator+=(ExtendedInt64z d) { return (*this) += ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator-=(ExtendedInt64z d) { return (*this) -= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator*=(ExtendedInt64z d) { return (*this) *= ExtendedInt64Pq(d); }
	inline ExtendedInt64Pq& operator/=(ExtendedInt64z d) { return (*this) /= ExtendedInt64Pq(d); }
	inline bool operator==(ExtendedInt64z d) const { return (*this) == ExtendedInt64Pq(d); }
	inline bool operator< (ExtendedInt64z d) const { return (*this) < ExtendedInt64Pq(d); }
	inline bool operator> (ExtendedInt64z d) const { return (*this) > ExtendedInt64Pq(d); }
	
private:
	static constexpr base_type btmin = std::numeric_limits<base_type>::min();
	static constexpr base_type btmax = std::numeric_limits<base_type>::max();
	//the denomintator also stores if ptr is used
	//If den == 0, then ptr is set and valid
	struct PQ {
		PQ() : num(0), den(1) {}
		base_type num;
		base_type den;
	};
	struct Ext {
		Ext() : ptr(0) {}
		extension_type * ptr;
		char dummy[2*sizeof(base_type)-sizeof(extension_type*)];
	};
	union Storage {
		Storage() : pq() {}
		PQ pq;
		Ext ext;
	};
	static_assert(sizeof(extension_type*) <= sizeof(base_type), "extension_type* needs to be smaller or equal than base_type");
private:
	const PQ & getPq() const;
	PQ & getPq();
	extension_type * ptr() const;
	void set(const PQ & pq);
	void set(base_type num, base_type den);
	void set(const extension_type & v);
	void set(extension_type * v);
	void deleteExt();
private:
	Storage m_v;
};


#define EI64PQ_TPL_PARAMS template<typename T_EXTENSION_TYPE>
#define EI64PQ_CLS_NAME ExtendedInt64Pq<T_EXTENSION_TYPE>

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
min BOOST_PREVENT_MACRO_SUBSTITUTION(const EI64PQ_CLS_NAME& x,const EI64PQ_CLS_NAME& y){
  return (x<=y)?x:y;
}


EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
max BOOST_PREVENT_MACRO_SUBSTITUTION(const EI64PQ_CLS_NAME & x, const EI64PQ_CLS_NAME& y){
  return (x>=y)?x:y;
}

EI64PQ_TPL_PARAMS
std::ostream & operator<<(std::ostream & out, const EI64PQ_CLS_NAME & v);

// AST for ExtendedInt64Pq
EI64PQ_TPL_PARAMS
class Algebraic_structure_traits< EI64PQ_CLS_NAME > : public Algebraic_structure_traits_base< EI64PQ_CLS_NAME, Field_tag >  {
private:
	typedef EI64PQ_CLS_NAME Type;
	typedef typename Type::extension_type ExtensionType;
	typedef Algebraic_structure_traits<ExtensionType> AstExt;
	typedef Algebraic_structure_traits<Type> AstBase;
public:
	typedef Tag_true            Is_exact;
	typedef Tag_false            Is_numerical_sensitive;
	
	class Is_square: public std::binary_function<Type, Type&, bool >  {
	public:
		inline bool operator()( const Type& x_, Type& y ) const {
			auto ye = y.asExtended();
			bool ret = m_p(x_.asExtended(), ye);
			y = Type(ye);
			return ret;
		}
		inline bool operator()( const Type& x) const {
			return m_p(x.asExtended());
		}
	private:
		typename AstExt::Is_square m_p;
	};

	class Simplify: public std::unary_function< Type&, void > {
	public:
		inline void operator()(Type& x) const {
			x.canonicalize();
		}
	};

};

// RET for ExtendedInt64Pq

EI64PQ_TPL_PARAMS
class Real_embeddable_traits< EI64PQ_CLS_NAME >: public INTERN_RET::Real_embeddable_traits_base<EI64PQ_CLS_NAME, CGAL::Tag_true> {
private:
	typedef EI64PQ_CLS_NAME Type;
	typedef typename Type::extension_type ExtensionType;
	typedef Real_embeddable_traits<ExtensionType> RetExt;
public:

	class Sgn: public std::unary_function< Type, ::CGAL::Sign > {
	public:
		::CGAL::Sign operator()( const Type& x ) const {
			return x.sign();
		}
	};

	class To_double: public std::unary_function< Type, double > {
	public:
		double operator()( const Type& x ) const {
			return x.to_double();
		}
	};

	class To_interval: public std::unary_function< Type, std::pair< double, double > > {
	public:
		std::pair<double, double> operator()(const Type & x ) const {
			return m_p( x.asExtended() );
		}
	private:
		typename RetExt::To_interval m_p;
	};
};

/*! \ingroup NiX_Fraction_traits_spec
 *  \brief Specialization of Fraction_traits for ExtendedInt64Pq
 */
EI64PQ_TPL_PARAMS
class Fraction_traits< EI64PQ_CLS_NAME > {
public:
	typedef EI64PQ_CLS_NAME Type;
	typedef typename Type::extension_type ExtensionType;
	typedef ::CGAL::Tag_true Is_fraction;
	typedef typename Type::numerator_type Numerator_type;
	typedef typename Type::denominator_type Denominator_type;
	typedef typename Algebraic_structure_traits<Numerator_type>::Gcd Common_factor;
	class Decompose {
	public:
		typedef Type first_argument_type;
		typedef Numerator_type& second_argument_type;
		typedef Denominator_type& third_argument_type;
		void operator () (Type const & rat, Numerator_type & num, Denominator_type & den) {
			num = rat.numerator();
			den = rat.denominator();
		}
	};
	class Compose {
	public:
		typedef Numerator_type first_argument_type;
		typedef Denominator_type second_argument_type;
		typedef Type result_type;
		result_type operator () (const first_argument_type& num,const second_argument_type& den) {
			return result_type(num, den);
		}
	};
};

} //namespace CGAL

//definitions

namespace CGAL {

#ifdef WITH_EI64_COUNT_ALLOCATIONS
	#define EI64_INC_NUM_E_ALLOC {++number_of_extended_allocations;}
	#define EI64_DEC_NUM_E_ALLOC {--number_of_extended_allocations;}
	#define EI64_INC_NUM_ALLOC {++number_of_allocations;}
	#define EI64_DEC_NUM_ALLOC {--number_of_allocations;}
#else
	#define EI64_INC_NUM_E_ALLOC
	#define EI64_DEC_NUM_E_ALLOC
	#define EI64_INC_NUM_ALLOC
	#define EI64_DEC_NUM_ALLOC
#endif

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq()
{
	EI64_INC_NUM_ALLOC
	set(base_type(0), base_type(1));
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const ExtendedInt64Pq & other)
{
	EI64_INC_NUM_ALLOC
	if (other.isExtended()) {
		set(other.getExtended());
	}
	else {
		set(other.getPq());
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(ExtendedInt64Pq && other)
{
	EI64_INC_NUM_ALLOC
	if (other.isExtended()) {
		set(other.ptr());
		other.set((extension_type*)0);
	}
	else {
		set(other.getPq());
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const CGAL::Gmpq & q)
{
	EI64_INC_NUM_ALLOC
	set(q);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(int32_t n) :
ExtendedInt64Pq(base_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(uint32_t n) :
ExtendedInt64Pq(base_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(base_type n)
{
	EI64_INC_NUM_ALLOC
	set(n, base_type(1));
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(uint64_t n)
{
	EI64_INC_NUM_ALLOC
	if (n < (uint64_t) btmax) {
		set(base_type(n), base_type(1));
	}
	else {
		set(extension_type(n));
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const Gmpz& n) :
ExtendedInt64Pq(extension_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const ExtendedInt64z & n)
{
	EI64_INC_NUM_ALLOC
	if (n.isExtended()) {
		set( extension_type(n.getExtended()) );
	}
	else {
		set(n.get(), base_type(1));
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const Gmpfr &f) :
ExtendedInt64Pq(extension_type(f))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(int32_t n, int32_t d) :
ExtendedInt64Pq(base_type(n), base_type(d))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(base_type n, base_type d)
{
	EI64_INC_NUM_ALLOC
	set(n, d);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(base_type n, uint64_t d)
{
	EI64_INC_NUM_ALLOC
	if (d < uint64_t(btmax) ) {
		set(base_type(n), base_type(d));
	}
	else {
		set(extension_type(n, d));
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(uint64_t n, uint64_t d)
{
	EI64_INC_NUM_ALLOC
	if (n < uint64_t(btmax) && d < uint64_t(btmax) ) {
		set(base_type(n), base_type(d));
	}
	else {
		set(extension_type(n, d));
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const ExtendedInt64z& n, const ExtendedInt64z& d)
{
	EI64_INC_NUM_ALLOC
	if (n.isExtended() && d.isExtended()) {
		set( extension_type( n.getExtended(), d.getExtended()) );
	}
	else if (n.isExtended()) {
		set( extension_type( n.getExtended(), d.asExtended()) );
	}
	else if (d.isExtended()) {
		set( extension_type( n.asExtended(), d.getExtended()) );
	}
	else {
		set(n.get(), d.get());
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const Gmpz& n, const Gmpz& d) :
ExtendedInt64Pq(extension_type(n, d))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(double d)
{
	EI64_INC_NUM_ALLOC
	if (double(base_type(d)) == d) {
		set(base_type(d), base_type(1));
	}
	else {
		set(extension_type(d));
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64Pq(const std::string& str, int base) :
ExtendedInt64Pq(extension_type(str, base))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::~ExtendedInt64Pq() {
	EI64_DEC_NUM_ALLOC
	if (isExtended()) {
		deleteExt();
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator=(const ExtendedInt64Pq & other) {
	if (other.isExtended()) {
		set(other.getExtended());
	}
	else {
		set(other.getPq());
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator=(ExtendedInt64Pq && other) {
	if (isExtended() && other.isExtended()) {
		deleteExt();
		set(other.ptr());
		other.set((extension_type*)0);
	}
	else if (isExtended()) {
		set(other.getPq());
	}
	else if (other.isExtended()) {
		set(other.ptr());
		other.set((extension_type*)0);
	}
	else {
		set(other.getPq());
	}
	return *this;
}

EI64PQ_TPL_PARAMS
std::size_t
EI64PQ_CLS_NAME::size() const {
	return sizeof(m_v) + (isExtended() ? getExtended().size() : 0);
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::canonicalize() {
	if (isExtended()) {
		::mpq_canonicalize(getExtended().mpq());
	}
	else {
		auto tmp = asExtended();
		::mpq_canonicalize(tmp.mpq());
		set(tmp);
	}
}

EI64PQ_TPL_PARAMS
ExtendedInt64z
EI64PQ_CLS_NAME::numerator() const {
	if (isExtended()) {
		return ExtendedInt64z( getExtended().numerator() );
	}
	else {
		return ExtendedInt64z( getPq().num );
	}
}

EI64PQ_TPL_PARAMS
ExtendedInt64z
EI64PQ_CLS_NAME::denominator() const {
	if (isExtended()) {
		return ExtendedInt64z( getExtended().denominator() );
	}
	else {
		return ExtendedInt64z( getPq().den );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator+() const {
	return ExtendedInt64Pq(*this);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator-() const {
	if (isExtended()) {
		return ExtendedInt64Pq( -getExtended() );
	}
	else {
		return ExtendedInt64Pq( -getPq().num, getPq().den );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator+=(const ExtendedInt64Pq &q) {
	if (isExtended() && q.isExtended()) {
		getExtended() += q.getExtended();
	}
	else if (isExtended()) {
		getExtended() += q.asExtended();
	}
	else if (q.isExtended()) {
		set( asExtended() + q.getExtended() );
	}
	else {
		set( asExtended() + q.asExtended() );
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator-=(const ExtendedInt64Pq &q) {
	if (isExtended() && q.isExtended()) {
		getExtended() -= q.getExtended();
	}
	else if (isExtended()) {
		getExtended() -= q.asExtended();
	}
	else if (q.isExtended()) {
		set( asExtended() - q.getExtended() );
	}
	else {
		set( asExtended() - q.asExtended() );
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator*=(const ExtendedInt64Pq &q) {
	if (isExtended() && q.isExtended()) {
		getExtended() *= q.getExtended();
	}
	else if (isExtended()) {
		getExtended() *= q.asExtended();
	}
	else if (q.isExtended()) {
		set( asExtended() * q.getExtended() );
	}
	else {
		set( asExtended() * q.asExtended() );
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator/=(const ExtendedInt64Pq &q) {
	if (isExtended() && q.isExtended()) {
		getExtended() /= q.getExtended();
	}
	else if (isExtended()) {
		getExtended() /= q.asExtended();
	}
	else if (q.isExtended()) {
		set( asExtended() / q.getExtended() );
	}
	else {
		set( asExtended() / q.asExtended() );
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator+(const ExtendedInt64Pq & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64Pq( getExtended() + other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64Pq( getExtended() + other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64Pq( asExtended() + other.getExtended() );
	}
	else {
		return ExtendedInt64Pq( asExtended() + other.asExtended() );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator-(const ExtendedInt64Pq & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64Pq( getExtended() - other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64Pq( getExtended() - other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64Pq( asExtended() - other.getExtended() );
	}
	else {
		return ExtendedInt64Pq( asExtended() - other.asExtended() );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator*(const ExtendedInt64Pq & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64Pq( getExtended() * other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64Pq( getExtended() * other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64Pq( asExtended() * other.getExtended() );
	}
	else {
		return ExtendedInt64Pq( asExtended() * other.asExtended() );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator/(const ExtendedInt64Pq & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64Pq( getExtended() / other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64Pq( getExtended() / other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64Pq( asExtended() / other.getExtended() );
	}
	else {
		return ExtendedInt64Pq( asExtended() / other.asExtended() );
	}
}

EI64PQ_TPL_PARAMS
bool
EI64PQ_CLS_NAME::operator==(const ExtendedInt64Pq &q) const {
	return asExtended() == q.asExtended();
}

EI64PQ_TPL_PARAMS
bool
EI64PQ_CLS_NAME::operator< (const ExtendedInt64Pq &q) const {
	return asExtended() < q.asExtended();
}

EI64PQ_TPL_PARAMS
double
EI64PQ_CLS_NAME::to_double() const {
	return asExtended().to_double();
}

EI64PQ_TPL_PARAMS
Sign
EI64PQ_CLS_NAME::sign() const {
	if (isExtended()) {
		return getExtended().sign();
	}
	else {
		return CGAL::sign(getPq().num);
	}
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::extension_type const &
EI64PQ_CLS_NAME::getExtended() const {
	assert(isExtended() && ptr());
	return *ptr();
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::extension_type &
EI64PQ_CLS_NAME::getExtended() {
	assert(isExtended() && ptr());
	return *ptr();
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::extension_type*
EI64PQ_CLS_NAME::ptr() const {
	assert(isExtended());
	return m_v.ext.ptr;
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::PQ&
EI64PQ_CLS_NAME::getPq() {
	return m_v.pq;
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::ExtendedInt64Pq::PQ const &
EI64PQ_CLS_NAME::getPq() const {
	return m_v.pq;
}

EI64PQ_TPL_PARAMS
typename EI64PQ_CLS_NAME::extension_type
EI64PQ_CLS_NAME::asExtended() const {
	if (isExtended()) {
		return getExtended();
	}
	else {
		return extension_type(getPq().num, getPq().den);
	}
}

EI64PQ_TPL_PARAMS
bool
EI64PQ_CLS_NAME::isExtended() const {
	return getPq().den == 0;
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(const ExtendedInt64Pq::PQ& pq) {
	set(pq.num, pq.den);
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(base_type num, base_type den) {
	if (isExtended()) {
		deleteExt();
	}
	if (den < 0) {
		den = -den;
		num = -num;
	}
	if (den == 0) {
		throw std::domain_error("Denominator is not allowed to be zero");
	}
	getPq().num = num;
	getPq().den = den;
	assert(!isExtended());
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(const extension_type & v) {
	
	auto num = v.numerator();
	auto den = v.denominator();
	
	if (::mpz_fits_slong_p(num.mpz()) && ::mpz_fits_slong_p(den.mpz())) {
		set(::mpz_get_si(num.mpz()), ::mpz_get_si(den.mpz()));
		assert(asExtended() == v);
		assert(num == getPq().num && den == getPq().den);
	}
	else {
		if (isExtended()) {
			getExtended() = v;
		}
		else {
			set( new extension_type(v) );
			EI64_INC_NUM_E_ALLOC
		}
		assert(isExtended());
	}
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(ExtendedInt64Pq::extension_type* v) {
	if (v) {
		getPq().den = 0;
		m_v.ext.ptr = v;
	}
	else {
		getPq().num = 0;
		getPq().den = 0xFEFE;
	}
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::deleteExt() {
	assert(isExtended());
	delete ptr();
	EI64_DEC_NUM_E_ALLOC
	set((extension_type*)0);
}

EI64PQ_TPL_PARAMS
std::ostream &
operator<<(std::ostream & out, const EI64PQ_CLS_NAME & v) {
	if (v.isExtended()) {
// 		::gmp_fprintf();
	}
	else {
		out << v.numerator().get() << '/' << v.denominator().get();
	}
	return out;
}

#undef EI64_INC_NUM_E_ALLOC
#undef EI64_DEC_NUM_E_ALLOC
#undef EI64_INC_NUM_ALLOC
#undef EI64_DEC_NUM_ALLOC

#undef EI64PQ_TPL_PARAMS
#undef EI64PQ_CLS_NAME

template<>
uint64_t ExtendedInt64Pq<CGAL::Gmpq>::number_of_extended_allocations;

template<>
uint64_t ExtendedInt64Pq<CGAL::Gmpq>::number_of_allocations;

}//end namespace CGAL

#if defined(BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif

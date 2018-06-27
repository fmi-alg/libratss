//Based on Gmpz_type.h from CGAL
//Original header follows

// Copyright (c) 1999,2003,2004  
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
// Author(s)     : Andreas Fabri, Stefan Schirra, Sylvain Pion, Michael Hemmer

//end of original header

#ifndef LIBRATSS_CGAL_EXTENDED_INT64_Z_H
#define LIBRATSS_CGAL_EXTENDED_INT64_Z_H

#include <CGAL/Gmpz.h>

#include <libratss/types.h>

#include <boost/multiprecision/cpp_int.hpp>

#include <type_traits>

namespace CGAL {
namespace internal {
	
using boost_int128 = boost::multiprecision::int128_t;
using boost_int1024 = boost::multiprecision::int1024_t;


template<typename T_EXTENSION_TYPE>
struct ExtendedInt64zTraits {
	using type = T_EXTENSION_TYPE;
	using primitive_type = LIB_RATSS_NAMESPACE::gmp_int64_t;
	static type make(int64_t v);
	static type make(uint64_t v);
	static primitive_type make_primitive(int64_t v);
};

template<>
struct ExtendedInt64zTraits<CGAL::Gmpz> {
	using type = CGAL::Gmpz;
	using primitive_type = LIB_RATSS_NAMESPACE::gmp_int64_t;
	static type make(int64_t v);
	static type make(uint64_t v);
	static primitive_type make_primitive(int64_t v);
};

template<>
struct ExtendedInt64zTraits<boost_int1024> {
	using type = boost_int1024;
	using primitive_type = LIB_RATSS_NAMESPACE::gmp_int64_t;
	static type make(int64_t v);
	static type make(uint64_t v);
	static primitive_type make_primitive(int64_t v);
};

} //end namespace iternal


class ExtendedInt64z:
    boost::ordered_euclidian_ring_operators1< ExtendedInt64z
  , boost::ordered_euclidian_ring_operators2< ExtendedInt64z, int
  , boost::ordered_euclidian_ring_operators2< ExtendedInt64z, long
  , boost::ordered_euclidian_ring_operators2< ExtendedInt64z, unsigned long
  , boost::shiftable< ExtendedInt64z , long
  , boost::unit_steppable<ExtendedInt64z
  , boost::bitwise<ExtendedInt64z
> > > > > > >
{
public:
	typedef Tag_true  Has_gcd;
	typedef Tag_true  Has_division;
	typedef Tag_true  Has_sqrt;

	typedef Tag_true  Has_exact_ring_operations;
	typedef Tag_true  Has_exact_division;
	typedef Tag_false Has_exact_sqrt;
public:
	using base_type = int64_t;
	using unsigned_base_type = typename std::make_unsigned<base_type>::type;
	using extension_type = CGAL::Gmpz;
	using config_traits = internal::ExtendedInt64zTraits<extension_type>;
// 	static_assert( --std::numeric_limits<base_type>::min() == std::numeric_limits<base_type>::min(), "");
public:
public:
	static uint64_t number_of_extended_allocations;
	ExtendedInt64z();
	ExtendedInt64z(const ExtendedInt64z & other);
	ExtendedInt64z(ExtendedInt64z && other);
	ExtendedInt64z(const Gmpz & z);
	ExtendedInt64z(base_type i);
	ExtendedInt64z(int32_t i);
	ExtendedInt64z(uint64_t l);
	ExtendedInt64z(double d);
	ExtendedInt64z(const std::string& str, int base = 10);
	ExtendedInt64z& operator=(const ExtendedInt64z & other);
	ExtendedInt64z& operator=(ExtendedInt64z && other);
public:
	bool isExtended() const;
	base_type & get();
	const base_type & get() const;
	extension_type & getExtended();
	const extension_type & getExtended() const;
	extension_type asExtended() const ;
public:
	// returns the memory size in bytes
	size_t size() const;

	double to_double() const;
	Sign sign() const;

public:
	ExtendedInt64z & operator+=(const ExtendedInt64z & other);
	ExtendedInt64z & operator-=(const ExtendedInt64z & other);
	ExtendedInt64z & operator*=(const ExtendedInt64z & other);
	ExtendedInt64z & operator/=(const ExtendedInt64z & other);
	ExtendedInt64z & operator%=(const ExtendedInt64z & other);
	ExtendedInt64z & operator&=(const ExtendedInt64z & other);
	ExtendedInt64z & operator|=(const ExtendedInt64z & other);
	ExtendedInt64z & operator^=(const ExtendedInt64z & other);

	bool operator<(const ExtendedInt64z &b) const;
	bool operator==(const ExtendedInt64z &b) const;

	ExtendedInt64z operator+() const;
	ExtendedInt64z operator-() const;

	ExtendedInt64z & operator <<= (const unsigned long& i);
	ExtendedInt64z& operator >>= (const unsigned long& i);

	ExtendedInt64z& operator++();
	ExtendedInt64z& operator--();

public:
	// interoperability with int32_t
	inline ExtendedInt64z& operator+=(int32_t i) { return operator+=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator-=(int32_t i) { return operator-=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator*=(int32_t i) { return operator*=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator/=(int32_t i) { return operator/=(ExtendedInt64z(i)); }
	inline bool operator==(int32_t i) const { return (*this) == ExtendedInt64z(i); }
	inline bool operator<(int32_t i) const { return (*this) < ExtendedInt64z(i); }
	inline bool operator>(int32_t i) const { return (*this) > ExtendedInt64z(i); }

	// interoperability with base_type
	inline ExtendedInt64z& operator+=(base_type i) { return operator+=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator-=(base_type i) { return operator-=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator*=(base_type i) { return operator*=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator/=(base_type i) { return operator/=(ExtendedInt64z(i)); }
	inline bool operator==(base_type i) const { return (*this) == ExtendedInt64z(i); }
	inline bool operator<(base_type i) const { return (*this) < ExtendedInt64z(i); }
	inline bool operator>(base_type i) const { return (*this) > ExtendedInt64z(i); }

	// interoperability with uint64_t
	inline ExtendedInt64z& operator+=(uint64_t i) { return operator+=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator-=(uint64_t i) { return operator-=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator*=(uint64_t i) { return operator*=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator/=(uint64_t i) { return operator/=(ExtendedInt64z(i)); }
	inline bool operator==(uint64_t i) const { return (*this) == ExtendedInt64z(i); }
	inline bool operator<(uint64_t i) const { return (*this) < ExtendedInt64z(i); }
	inline bool operator>(uint64_t i) const { return (*this) > ExtendedInt64z(i); }
	
	// interoperability with Gmpz
	inline ExtendedInt64z& operator+=(const Gmpz & i) { return operator+=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator-=(const Gmpz & i) { return operator-=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator*=(const Gmpz & i) { return operator*=(ExtendedInt64z(i)); }
	inline ExtendedInt64z& operator/=(const Gmpz & i) { return operator/=(ExtendedInt64z(i)); }
	inline bool operator==(const Gmpz & i) const { return (*this) == ExtendedInt64z(i); }
	inline bool operator<(const Gmpz & i) const { return (*this) < ExtendedInt64z(i); }
	inline bool operator>(const Gmpz & i) const { return (*this) > ExtendedInt64z(i); }
private:
	static constexpr base_type btmax = std::numeric_limits<base_type>::max();
	static constexpr base_type btmin = std::numeric_limits<base_type>::min();
private:
	config_traits::primitive_type getExtendedPrimitive() const;
private:
	extension_type * ptr() const;
	void set(base_type v);
	void set(const extension_type & v);
	void set(extension_type * v);
	void deleteExt();
private:
	union {
		base_type i; 
		extension_type * ptr;
	} m_v;
	bool m_isExtended;
};

template <>
struct Split_double<ExtendedInt64z>
{
  void operator()(double d, ExtendedInt64z &num, ExtendedInt64z &den) const
  {
	std::pair<double, double> p = split_numerator_denominator(d);
	num = ExtendedInt64z(p.first);
	den = ExtendedInt64z(p.second);
  }
};

inline ExtendedInt64z min BOOST_PREVENT_MACRO_SUBSTITUTION(const ExtendedInt64z& x,const ExtendedInt64z& y){
  return (x<=y)?x:y;
}
inline ExtendedInt64z max BOOST_PREVENT_MACRO_SUBSTITUTION(const ExtendedInt64z& x,const ExtendedInt64z& y){
  return (x>=y)?x:y;
}

// Algebraic structure traits
template<>
class Algebraic_structure_traits<ExtendedInt64z> : public Algebraic_structure_traits_base<ExtendedInt64z, Euclidean_ring_tag >  {
private:
	typedef Algebraic_structure_traits<ExtendedInt64z::extension_type> AstExt;
	typedef Algebraic_structure_traits<ExtendedInt64z> AstBase;
public:
	typedef Tag_true            Is_exact;
	typedef Tag_false           Is_numerical_sensitive;

	typedef INTERN_AST::Is_square_per_sqrt< Type > Is_square;
	
	class Integral_division: private std::binary_function<Type, Type, Type> {
	public:
		Type operator()(const Type& x, const Type& y) const {
			return Type(m_p( x.asExtended(), y.asExtended() ) );
		}
	private:
		AstExt::Integral_division m_p;
	};

	class Gcd: private std::binary_function<Type, Type, Type> {
	public:
		Type operator()( const Type& x, const Type& y ) const {
			return Type( m_p(x.asExtended(), y.asExtended()) );
		}

		Type operator()(const Type& x, const int& y ) const {
			return Type( m_p(x.asExtended(), y) );
		}

		Type operator()(const int& x, const Type& y ) const {
			return Type( m_p(x, y.asExtended()) );
		}
	private:
		AstExt::Gcd m_p;
	};

	typedef INTERN_AST::Div_per_operator< Type > Div;
	typedef INTERN_AST::Mod_per_operator< Type > Mod;

	class Sqrt: public std::unary_function< Type, Type > {
	public:
		Type operator()(const Type& x ) const {
			return Type( m_p(x.asExtended()) );
		}
	private:
		AstExt::Sqrt m_p;
	};
};

template<>
class Real_embeddable_traits<ExtendedInt64z>: public INTERN_RET::Real_embeddable_traits_base<ExtendedInt64z , CGAL::Tag_true> {
private:
	typedef Real_embeddable_traits<ExtendedInt64z> RetBase;
	typedef Real_embeddable_traits<ExtendedInt64z::extension_type> RetExt;
public:
	class Sgn: public std::unary_function< Type, ::CGAL::Sign > {
	public:
		::CGAL::Sign operator()( const Type& x ) const {
			return x.sign();
		}
	};

	class To_double: public std::unary_function<Type, double> {
	public:
		double operator()( const Type& x ) const {
			return x.to_double();
		}
	};

	class To_interval: public std::unary_function< Type, std::pair< double, double > > {
	public:
		std::pair<double, double> operator()( const Type& x ) const {
			return m_p( x.asExtended() );
		}
	private:
		RetExt::To_interval m_p;
	};
};

template<>
class Algebraic_structure_traits< Quotient<ExtendedInt64z> >: public INTERN_QUOTIENT::Algebraic_structure_traits_quotient_base<Quotient<ExtendedInt64z> >{
private:
	typedef Algebraic_structure_traits< Quotient<ExtendedInt64z::extension_type> > AstExt;
	typedef Algebraic_structure_traits< Quotient<ExtendedInt64z> > AstBase;
public:
	typedef Quotient<ExtendedInt64z> Type;

	class To_double: public std::unary_function<Quotient<ExtendedInt64z>, double> {
	public:
		double operator()(const Quotient<ExtendedInt64z>& quot){
			return m_p(Quotient<ExtendedInt64z::extension_type>(quot.num.asExtended(), quot.den.asExtended()));
		}
	private:
		AstExt::To_double m_p;
	};
};

template <>
class Needs_parens_as_product<ExtendedInt64z> {
public:
	bool operator()(const ExtendedInt64z& x) {
		return m_p( x.asExtended() );
	}
private:
	Needs_parens_as_product<Gmpz> m_p;
};

} //namespace CGAL

#endif // LIBRATSS_CGAL_EXTENDED_INT64_Z_H

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

namespace CGAL {

class ExtendedInt64Pq:
	boost::totally_ordered1< ExtendedInt64Pq
	, boost::ordered_field_operators2< ExtendedInt64Pq, int
	, boost::ordered_field_operators2< ExtendedInt64Pq, long
	, boost::ordered_field_operators2< ExtendedInt64Pq, long long
	, boost::ordered_field_operators2< ExtendedInt64Pq, double
	, boost::ordered_field_operators2< ExtendedInt64Pq, Gmpq
	, boost::ordered_field_operators2< ExtendedInt64Pq, Gmpz
	, boost::ordered_field_operators2< ExtendedInt64Pq, Gmpfr
	, boost::ordered_field_operators2< ExtendedInt64Pq, ExtendedInt64z
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
	typedef CGAL::Gmpq extension_type;
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
	ExtendedInt64Pq(const ExtendedInt64z& n, const ExtendedInt64z& d);
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

	ExtendedInt64z numerator() const;
	ExtendedInt64z denominator() const;

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

inline ExtendedInt64Pq min BOOST_PREVENT_MACRO_SUBSTITUTION(const ExtendedInt64Pq& x,const ExtendedInt64Pq& y){
  return (x<=y)?x:y;
}
inline ExtendedInt64Pq max BOOST_PREVENT_MACRO_SUBSTITUTION(const ExtendedInt64Pq& x,const ExtendedInt64Pq& y){
  return (x>=y)?x:y;
}

// AST for ExtendedInt64Pq
template<>
class Algebraic_structure_traits<ExtendedInt64Pq> : public Algebraic_structure_traits_base< ExtendedInt64Pq, Field_tag >  {
private:
	typedef Algebraic_structure_traits<ExtendedInt64Pq::extension_type> AstExt;
	typedef Algebraic_structure_traits<ExtendedInt64Pq> AstBase;
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
		AstExt::Is_square m_p;
	};

	class Simplify: public std::unary_function< Type&, void > {
	public:
		inline void operator()(Type& x) const {
			x.canonicalize();
		}
	};

};

// RET for ExtendedInt64Pq

template<>
class Real_embeddable_traits< ExtendedInt64Pq >: public INTERN_RET::Real_embeddable_traits_base<ExtendedInt64Pq , CGAL::Tag_true > {
private:
	typedef Real_embeddable_traits< ExtendedInt64Pq::extension_type > RetExt;
	typedef Real_embeddable_traits< ExtendedInt64Pq > RetBase;
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
		RetExt::To_interval m_p;
	};
};

/*! \ingroup NiX_Fraction_traits_spec
 *  \brief Specialization of Fraction_traits for ExtendedInt64Pq
 */
template <>
class Fraction_traits< ExtendedInt64Pq > {
public:
	typedef ExtendedInt64Pq Type;
	typedef ::CGAL::Tag_true Is_fraction;
	typedef ExtendedInt64z Numerator_type;
	typedef ExtendedInt64z Denominator_type;
	typedef Algebraic_structure_traits<ExtendedInt64z>::Gcd Common_factor;
	class Decompose {
	public:
		typedef ExtendedInt64Pq first_argument_type;
		typedef ExtendedInt64z& second_argument_type;
		typedef ExtendedInt64z& third_argument_type;
		void operator () (const ExtendedInt64Pq& rat, ExtendedInt64z& num,ExtendedInt64z& den) {
			num = rat.numerator();
			den = rat.denominator();
		}
	};
	class Compose {
	public:
		typedef ExtendedInt64z first_argument_type;
		typedef ExtendedInt64z second_argument_type;
		typedef ExtendedInt64Pq result_type;
		ExtendedInt64Pq operator () (const ExtendedInt64z& num,const ExtendedInt64z& den) {
			return ExtendedInt64Pq(num, den);
		}
	};
};

} //namespace CGAL

#if defined(BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif

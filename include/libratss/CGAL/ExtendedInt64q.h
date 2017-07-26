#ifndef LIBRATSS_CGAL_EXTENDED_INT_64_Q_H
#define LIBRATSS_CGAL_EXTENDED_INT_64_Q_H
#pragma once

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


#include <libratss/CGAL/boost_int1024q_traits.h>
#include <libratss/constants.h>
#include <libratss/CGAL/ExtendedInt64z.h>

#include <CGAL/Gmpq.h>
#include <CGAL/internal/Exact_type_selector.h>

namespace CGAL {

template<typename T_EXTENSION_TYPE>
class ExtendedInt64q;

namespace internal {

	using boost_int1024 = boost::multiprecision::int1024_t;
	using boost_int1024q = boost::multiprecision::number< boost::multiprecision::rational_adaptor<boost_int1024::backend_type> >;
	
	template<typename T_EXTENSION_TYPE>
	struct ExtendedInt64qTraits {
		using type = T_EXTENSION_TYPE;
		using numerator_type = typename type::numerator_type;
		using denominator_type = typename type::denominator_type;
		
		template<typename T_FIRST_OP>
		using ordered_field_operators2 = boost::ordered_field_operators2<T_FIRST_OP, type>;
		
		static void simplify(type & v);
		static bool fits_int64(const numerator_type & v);
		static bool fits_int64(const denominator_type & v);
		static int64_t to_int64(const numerator_type & v);
		static int64_t to_int64(const denominator_type & v);
		static double to_double(const type & v);
		static CGAL::ExtendedInt64z::extension_type to_ei64z(const numerator_type & v);
		static CGAL::ExtendedInt64z::extension_type to_ei64z(const denominator_type & v);
	};
	
	template<typename T_EXTENSION_TYPE>
	struct Exact_field_selector< ExtendedInt64q<T_EXTENSION_TYPE> > {
		typedef ExtendedInt64q<T_EXTENSION_TYPE> Type;
	};
	
}

template<typename T_EXTENSION_TYPE>
class ExtendedInt64q:
	boost::totally_ordered1< ExtendedInt64q<T_EXTENSION_TYPE>
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, int
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, long
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, long long
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, double
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, Gmpz
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, ExtendedInt64z
	, boost::ordered_field_operators2< ExtendedInt64q<T_EXTENSION_TYPE>, Gmpq
		> > > > > > > >
{
public:
	using Has_gcd = Tag_false;
	using Has_division = Tag_true;
	using Has_sqrt = Tag_false;

	using Has_exact_ring_operations = Tag_true;
	using Has_exact_division = Tag_true;
	using Has_exact_sqrt = Tag_false;
public:
	using MySelf = ExtendedInt64q<T_EXTENSION_TYPE>;
	using base_type = CGAL::ExtendedInt64z::base_type;
	using unsigned_base_type = std::make_unsigned<base_type>::type;
	using extension_type = T_EXTENSION_TYPE;
	using numerator_type = ExtendedInt64z;
	using denominator_type = ExtendedInt64z;

	using config_traits = internal::ExtendedInt64qTraits<extension_type>;
	using extension_numerator_type = typename config_traits::numerator_type;
	using extension_denominator_type = typename config_traits::denominator_type;
	
	struct PQ;
	
public:
	static uint64_t number_of_extended_allocations;
	static uint64_t number_of_allocations;
	static uint32_t max_numerator_bits;
	static uint32_t max_denominator_bits;
public:
	ExtendedInt64q();
	ExtendedInt64q(const ExtendedInt64q & other);
	ExtendedInt64q(ExtendedInt64q&& other);
	
	ExtendedInt64q(const PQ & q);

	ExtendedInt64q(const extension_type & q);
	ExtendedInt64q(extension_type && q);

	ExtendedInt64q(int32_t n);
	ExtendedInt64q(uint32_t n);
	ExtendedInt64q(base_type n);
	ExtendedInt64q(uint64_t n);
	ExtendedInt64q(const extension_numerator_type & n);
	ExtendedInt64q(const ExtendedInt64z & n);

	ExtendedInt64q(int32_t n, int32_t d);
	ExtendedInt64q(base_type n, base_type d);
	ExtendedInt64q(base_type n, uint64_t d);
	ExtendedInt64q(uint64_t n, uint64_t d);
	ExtendedInt64q(const numerator_type& n, const denominator_type& d);
	ExtendedInt64q(const extension_numerator_type& n, const extension_denominator_type& d);
	ExtendedInt64q(double d);
	
	ExtendedInt64q(const std::string& str, int base = 10);
	
	~ExtendedInt64q();
	
	ExtendedInt64q& operator=(const ExtendedInt64q & other);
	ExtendedInt64q& operator=(ExtendedInt64q && other);
public:
	// Gives the memory size in bytes. (not documented yet)
	std::size_t size() const;
	
	void canonicalize();

	numerator_type  numerator() const;
	denominator_type denominator() const;

	ExtendedInt64q operator+() const;
	ExtendedInt64q operator-() const;

	ExtendedInt64q& operator+=(const ExtendedInt64q & other);
	ExtendedInt64q& operator-=(const ExtendedInt64q & other);
	ExtendedInt64q& operator*=(const ExtendedInt64q & other);
	ExtendedInt64q& operator/=(const ExtendedInt64q & other);
	
	ExtendedInt64q operator+(const ExtendedInt64q & other) const;
	ExtendedInt64q operator-(const ExtendedInt64q & other) const;
	ExtendedInt64q operator*(const ExtendedInt64q & other) const;
	ExtendedInt64q operator/(const ExtendedInt64q & other) const;

	bool operator==(const ExtendedInt64q &q) const;
	bool operator< (const ExtendedInt64q &q) const;

	double to_double() const;
	Sign sign() const;

	bool isExtended() const;
	const extension_type & getExtended() const;
	const PQ & getPq() const;
	extension_type asExtended() const;
	
	inline operator extension_type() const { return asExtended(); }

	// Interoperability with int32_t
	inline ExtendedInt64q& operator+=(int32_t z) { return (*this) += ExtendedInt64q(z); }
	inline ExtendedInt64q& operator-=(int32_t z) { return (*this) -= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator*=(int32_t z) { return (*this) *= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator/=(int32_t z) { return (*this) /= ExtendedInt64q(z); }
	inline bool operator==(int32_t z) const {return *this == ExtendedInt64q(z); }
	inline bool operator< (int32_t z) const {return *this < ExtendedInt64q(z); }
	inline bool operator> (int32_t z) const {return *this > ExtendedInt64q(z); }

	// Interoperability with int64_t
	inline ExtendedInt64q& operator+=(int64_t z) { return (*this) += ExtendedInt64q(z); }
	inline ExtendedInt64q& operator-=(int64_t z) { return (*this) -= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator*=(int64_t z) { return (*this) *= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator/=(int64_t z) { return (*this) /= ExtendedInt64q(z); }
	inline bool operator==(int64_t z) const { return *this == ExtendedInt64q(z); }
	inline bool operator< (int64_t z) const { return *this < ExtendedInt64q(z); }
	inline bool operator> (int64_t z) const { return *this > ExtendedInt64q(z); }

	// Interoperability with uint64_t
	inline ExtendedInt64q& operator+=(uint64_t z) { return (*this) += ExtendedInt64q(z); }
	inline ExtendedInt64q& operator-=(uint64_t z) { return (*this) -= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator*=(uint64_t z) { return (*this) *= ExtendedInt64q(z); }
	inline ExtendedInt64q& operator/=(uint64_t z) { return (*this) /= ExtendedInt64q(z); }
	inline bool operator==(uint64_t z) const { return (*this) == ExtendedInt64q(z); }
	inline bool operator< (uint64_t z) const { return (*this) < ExtendedInt64q(z); }
	inline bool operator> (uint64_t z) const { return (*this) > ExtendedInt64q(z); }

	// Interoperability with double
	inline ExtendedInt64q& operator+=(double d) { return (*this) += ExtendedInt64q(d); }
	inline ExtendedInt64q& operator-=(double d) { return (*this) -= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator*=(double d) { return (*this) *= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator/=(double d) { return (*this) /= ExtendedInt64q(d); }
	inline bool operator==(double d) const { return (*this) == ExtendedInt64q(d); }
	inline bool operator< (double d) const { return (*this) < ExtendedInt64q(d); }
	inline bool operator> (double d) const { return (*this) > ExtendedInt64q(d); }

	// Interoperability with extension_type
	inline ExtendedInt64q& operator+=(const extension_type & d) { return (*this) += ExtendedInt64q(d); }
	inline ExtendedInt64q& operator-=(const extension_type & d) { return (*this) -= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator*=(const extension_type & d) { return (*this) *= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator/=(const extension_type & d) { return (*this) /= ExtendedInt64q(d); }
	inline bool operator==(const extension_type & d) const { return (*this) == ExtendedInt64q(d); }
	inline bool operator< (const extension_type & d) const { return (*this) < ExtendedInt64q(d); }
	inline bool operator> (const extension_type & d) const { return (*this) > ExtendedInt64q(d); }

	// Interoperability with Gmpz
	inline ExtendedInt64q& operator+=(const extension_numerator_type & d) { return (*this) += ExtendedInt64q(d); }
	inline ExtendedInt64q& operator-=(const extension_numerator_type & d) { return (*this) -= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator*=(const extension_numerator_type & d) { return (*this) *= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator/=(const extension_numerator_type & d) { return (*this) /= ExtendedInt64q(d); }
	inline bool operator==(const extension_numerator_type & d) const { return (*this) == ExtendedInt64q(d); }
	inline bool operator< (const extension_numerator_type & d) const { return (*this) < ExtendedInt64q(d); }
	inline bool operator> (const extension_numerator_type & d) const { return (*this) > ExtendedInt64q(d); }

	// Interoperability with ExtendedInt
	inline ExtendedInt64q& operator+=(const ExtendedInt64z & d) { return (*this) += ExtendedInt64q(d); }
	inline ExtendedInt64q& operator-=(const ExtendedInt64z & d) { return (*this) -= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator*=(const ExtendedInt64z & d) { return (*this) *= ExtendedInt64q(d); }
	inline ExtendedInt64q& operator/=(const ExtendedInt64z & d) { return (*this) /= ExtendedInt64q(d); }
	inline bool operator==(const ExtendedInt64z & d) const { return (*this) == ExtendedInt64q(d); }
	inline bool operator< (const ExtendedInt64z & d) const { return (*this) < ExtendedInt64q(d); }
	inline bool operator> (const ExtendedInt64z & d) const { return (*this) > ExtendedInt64q(d); }
	
private:
	static constexpr base_type btmin = std::numeric_limits<base_type>::min();
	static constexpr base_type btmax = std::numeric_limits<base_type>::max();
public:
	//the denomintator also stores if ptr is used
	//If den == 0, then ptr is set and valid
	struct PQ {
		PQ() : num(0), den(1) {}
		PQ(base_type num, base_type den) : num(num), den(den) {}
		base_type num;
		base_type den;
	};
private:
	using uint128 = __uint128_t;
	using int128 = __int128_t;
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
	ExtendedInt64q(int128 n, int128 d);
	PQ & getPq();
	extension_type & getExtended();
	extension_type * ptr() const;
	void set(const PQ & pq);
	void set(base_type num, base_type den);
	void set(int128 num, int128 den);
	void set(const extension_type & v);
	void set(extension_type && v);
	void set(extension_type * v);
	void deleteExt();
private:
	Storage m_v;
};


#define EI64PQ_TPL_PARAMS template<typename T_EXTENSION_TYPE>
#define EI64PQ_CLS_NAME ExtendedInt64q<T_EXTENSION_TYPE>

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

// AST for ExtendedInt64q
EI64PQ_TPL_PARAMS
class Algebraic_structure_traits< EI64PQ_CLS_NAME > : public Algebraic_structure_traits_base< EI64PQ_CLS_NAME, Field_tag >  {
private:
	typedef EI64PQ_CLS_NAME Type;
	typedef typename Type::extension_type ExtensionType;
	typedef typename Type::base_type BaseType;
	typedef Algebraic_structure_traits<ExtensionType> AstExt;
	typedef CGAL::Quotient<BaseType> BaseTypeQuotient;
	typedef Algebraic_structure_traits< BaseTypeQuotient > AstBase;
public:
	typedef Tag_true            Is_exact;
	typedef Tag_false            Is_numerical_sensitive;
	
	class Is_square: public std::binary_function<Type, Type&, bool >  {
	public:
		inline bool operator()( const Type& x_, Type& y ) const {
			assert(false);
			auto ye = y.asExtended();
			bool ret = m_pe(x_.asExtended(), ye);
			y = Type(ye);
			return ret;
		}
		inline bool operator()( const Type& x) const {
			assert(false);
			if (x.isExtended()) {
				return m_pe(x.getExtended());
			}
			else {
				return m_pb( BaseTypeQuotient(x.getPq().num, x.getPq().den) );
			}
		}
	private:
		typename AstExt::Is_square m_pe;
		typename AstBase::Is_square m_pb;
	};

	class Simplify: public std::unary_function< Type&, void > {
	public:
		inline void operator()(Type& x) const {
			x.canonicalize();
		}
	};

};

// RET for ExtendedInt64q

EI64PQ_TPL_PARAMS
class Real_embeddable_traits< EI64PQ_CLS_NAME >: public INTERN_RET::Real_embeddable_traits_base<EI64PQ_CLS_NAME, CGAL::Tag_true> {
private:
	typedef EI64PQ_CLS_NAME Type;
	typedef typename Type::base_type base_type;
	typedef typename Type::extension_type ExtensionType;
	typedef Real_embeddable_traits<ExtensionType> RetExt;
	typedef Real_embeddable_traits<base_type> BaseExt;
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
			if (x.isExtended()) {
				return m_p( x.getExtended() );
			}
			else {
				Interval_nt<> quot =
				Interval_nt<>(CGAL_NTS to_interval(x.getPq().num)) /
				Interval_nt<>(CGAL_NTS to_interval(x.getPq().den));
				return std::make_pair(quot.inf(), quot.sup());
			}
		}
	private:
		typename RetExt::To_interval m_p;
	};
};

/*! \ingroup NiX_Fraction_traits_spec
 *  \brief Specialization of Fraction_traits for ExtendedInt64q
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

#define WITH_EI64_STATS
#ifdef WITH_EI64_STATS
	#define EI64_INC_NUM_E_ALLOC {++number_of_extended_allocations;}
	#define EI64_DEC_NUM_E_ALLOC {--number_of_extended_allocations;}
	#define EI64_INC_NUM_ALLOC {++number_of_allocations;}
	#define EI64_DEC_NUM_ALLOC {--number_of_allocations;}
	#define EI64_UPDATE_MAX_BITS(__x) {{ \
		max_numerator_bits = std::max<uint32_t>(max_numerator_bits, config_traits::num_bits(config_traits::numerator(__x))); \
		max_denominator_bits = std::max<uint32_t>(max_denominator_bits, config_traits::num_bits(config_traits::denominator( __x))); \
	}}
#else
	#define EI64_INC_NUM_E_ALLOC
	#define EI64_DEC_NUM_E_ALLOC
	#define EI64_INC_NUM_ALLOC
	#define EI64_DEC_NUM_ALLOC
	#define EI64_UPDATE_MAX_BITS
#endif

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q()
{
	EI64_INC_NUM_ALLOC
	set(base_type(0), base_type(1));
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(const ExtendedInt64q & other)
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
EI64PQ_CLS_NAME::ExtendedInt64q(ExtendedInt64q && other)
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
EI64PQ_CLS_NAME::ExtendedInt64q(const PQ & q)
{
	EI64_INC_NUM_ALLOC
	set(q);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(const extension_type & q)
{
	EI64_INC_NUM_ALLOC
	set(q);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(extension_type && q)
{
	EI64_INC_NUM_ALLOC
	set(std::move(q));
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(int32_t n) :
ExtendedInt64q(base_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(uint32_t n) :
ExtendedInt64q(base_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(base_type n)
{
	EI64_INC_NUM_ALLOC
	set(n, base_type(1));
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(uint64_t n)
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
EI64PQ_CLS_NAME::ExtendedInt64q(const extension_numerator_type & n) :
ExtendedInt64q(extension_type(n))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(const ExtendedInt64z & n)
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
EI64PQ_CLS_NAME::ExtendedInt64q(int32_t n, int32_t d) :
ExtendedInt64q(base_type(n), base_type(d))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(base_type n, base_type d)
{
	EI64_INC_NUM_ALLOC
	set(n, d);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(int128 n, int128 d)
{
	set(n, d);
	if (isExtended()) {
		EI64_INC_NUM_E_ALLOC
	}
	else {
		EI64_INC_NUM_ALLOC
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(base_type n, uint64_t d)
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
EI64PQ_CLS_NAME::ExtendedInt64q(uint64_t n, uint64_t d)
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
EI64PQ_CLS_NAME::ExtendedInt64q(const ExtendedInt64z& n, const ExtendedInt64z& d)
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
EI64PQ_CLS_NAME::ExtendedInt64q(const extension_numerator_type & n, const extension_denominator_type & d) :
ExtendedInt64q(extension_type(n, d))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::ExtendedInt64q(double d)
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
EI64PQ_CLS_NAME::ExtendedInt64q(const std::string& str, int base) :
ExtendedInt64q(extension_type(str, base))
{}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME::~ExtendedInt64q() {
	EI64_DEC_NUM_ALLOC
	if (isExtended()) {
		deleteExt();
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator=(const ExtendedInt64q & other) {
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
EI64PQ_CLS_NAME::operator=(ExtendedInt64q && other) {
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
		config_traits::simplify(getExtended());
	}
	else {
		int64_t gcd = std::__gcd(std::abs<base_type>(getPq().num), getPq().den);
		getPq().num /= gcd;
		getPq().den /= gcd;
	}
}

EI64PQ_TPL_PARAMS
ExtendedInt64z
EI64PQ_CLS_NAME::numerator() const {
	if (isExtended()) {
		return ExtendedInt64z( config_traits::to_ei64z(config_traits::numerator(getExtended())) );
	}
	else {
		return ExtendedInt64z( getPq().num );
	}
}

EI64PQ_TPL_PARAMS
ExtendedInt64z
EI64PQ_CLS_NAME::denominator() const {
	if (isExtended()) {
		return ExtendedInt64z( config_traits::to_ei64z( config_traits::denominator(getExtended()) ) );
	}
	else {
		return ExtendedInt64z( getPq().den );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator+() const {
	return ExtendedInt64q(*this);
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator-() const {
	if (isExtended()) {
		return ExtendedInt64q( -getExtended() );
	}
	else {
		return ExtendedInt64q( -getPq().num, getPq().den );
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator+=(const ExtendedInt64q &other) {
	if (isExtended() && other.isExtended()) {
		getExtended() += other.getExtended();
	}
	else if (isExtended()) {
		getExtended() += other.asExtended();
	}
	else if (other.isExtended()) {
		set( asExtended() + other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num)*int128(other.getPq().den) + int128(other.getPq().num)*int128(getPq().den);
		int128 den = int128(getPq().den)*int128(other.getPq().den);
		set(num, den);
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator-=(const ExtendedInt64q &other) {
	if (isExtended() && other.isExtended()) {
		getExtended() -= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() -= other.asExtended();
	}
	else if (other.isExtended()) {
		set( asExtended() - other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num)*int128(other.getPq().den) - int128(other.getPq().num)*int128(getPq().den);
		int128 den = int128(getPq().den)*int128(other.getPq().den);
		set(num, den);
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator*=(const ExtendedInt64q & other) {
	if (isExtended() && other.isExtended()) {
		getExtended() *= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() *= other.asExtended();
	}
	else if (other.isExtended()) {
		set( asExtended() * other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num) * int128(other.getPq().num);
		int128 den = int128(getPq().den) * int128(other.getPq().den);
		set(num, den);
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME&
EI64PQ_CLS_NAME::operator/=(const ExtendedInt64q &other) {
	if (isExtended() && other.isExtended()) {
		getExtended() /= other.getExtended();
	}
	else if (isExtended()) {
		getExtended() /= other.asExtended();
	}
	else if (other.isExtended()) {
		set( asExtended() / other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num)*int128(other.getPq().den);
		int128 den = int128(getPq().den)*int128(other.getPq().num);
		set(num, den);
	}
	return *this;
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator+(const ExtendedInt64q & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64q( getExtended() + other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64q( getExtended() + other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64q( asExtended() + other.getExtended() );
	}
	else {
		const int128 num1 = int128(getPq().num)*int128(other.getPq().den);
		const int128 num2 = int128(other.getPq().num)*int128(getPq().den);
		const int128 num = num1 + num2;
		const int128 den = int128(getPq().den)*int128(other.getPq().den);
		assert( ExtendedInt64q(num, den) == (asExtended() + other.asExtended()) );
		return ExtendedInt64q(num, den);
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator-(const ExtendedInt64q & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64q( getExtended() - other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64q( getExtended() - other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64q( asExtended() - other.getExtended() );
	}
	else {
		//TODO: check if this can overflow
		int128 num = int128(getPq().num)*int128(other.getPq().den) - int128(other.getPq().num)*int128(getPq().den);
		int128 den = int128(getPq().den)*int128(other.getPq().den);
		assert( ExtendedInt64q(num, den) == (asExtended() - other.asExtended()) );
		return ExtendedInt64q(num, den);
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator*(const ExtendedInt64q & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64q( getExtended() * other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64q( getExtended() * other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64q( asExtended() * other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num) * int128(other.getPq().num);
		int128 den = int128(getPq().den) * int128(other.getPq().den);
		assert(ExtendedInt64q(num, den) == asExtended() * other.asExtended());
		return ExtendedInt64q(num, den);
	}
}

EI64PQ_TPL_PARAMS
EI64PQ_CLS_NAME
EI64PQ_CLS_NAME::operator/(const ExtendedInt64q & other) const {
	if (isExtended() && other.isExtended()) {
		return ExtendedInt64q( getExtended() / other.getExtended() );
	}
	else if (isExtended()) {
		return ExtendedInt64q( getExtended() / other.asExtended() );
	}
	else if (other.isExtended()) {
		return ExtendedInt64q( asExtended() / other.getExtended() );
	}
	else {
		int128 num = int128(getPq().num) * int128(other.getPq().den);
		int128 den = int128(getPq().den) * int128(other.getPq().num);
		assert( ExtendedInt64q(num, den) == (asExtended() / other.asExtended()) );
		return ExtendedInt64q(num, den);
	}
}

EI64PQ_TPL_PARAMS
bool
EI64PQ_CLS_NAME::operator==(const ExtendedInt64q & other) const {
	if (isExtended() && other.isExtended()) {
		return getExtended() == other.getExtended();
	}
	else if (isExtended()) {
		return getExtended() == other.asExtended();
	}
	else if (other.isExtended()) {
		return asExtended() == other.getExtended();
	}
	else {
		return (int128(getPq().num) * int128(other.getPq().den)) == (int128(other.getPq().num) * int128(getPq().den));
	}
}

EI64PQ_TPL_PARAMS
bool
EI64PQ_CLS_NAME::operator< (const ExtendedInt64q &other) const {
	if (isExtended() && other.isExtended()) {
		return getExtended() < other.getExtended();
	}
	else if (isExtended()) {
		return getExtended() < other.asExtended();
	}
	else if (other.isExtended()) {
		return asExtended() < other.getExtended();
	}
	else {
		return (int128(getPq().num) * int128(other.getPq().den)) < (int128(other.getPq().num) * int128(getPq().den));
	}
}

EI64PQ_TPL_PARAMS
double
EI64PQ_CLS_NAME::to_double() const {
	if (isExtended()) {
		return config_traits::to_double(getExtended());
	}
	else {
		return CGAL::to_double( Quotient<base_type>(getPq().num, getPq().den) );
	}
}

EI64PQ_TPL_PARAMS
Sign
EI64PQ_CLS_NAME::sign() const {
	if (isExtended()) {
		return CGAL::sign( getExtended() );
	}
	else {
		return CGAL::sign( getPq().num );
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
typename EI64PQ_CLS_NAME::ExtendedInt64q::PQ const &
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
EI64PQ_CLS_NAME::set(const ExtendedInt64q::PQ& pq) {
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
EI64PQ_CLS_NAME::set(int128 num, int128 den) {
	if (den < 0) {
		num = -num;
		den = -den;
	}
	if (den == 0) {
		throw std::domain_error("Denominator is not allowed to be zero");
	}
	int128 gcd = std::__gcd(num < 0 ? -num : num, den);
	num /= gcd;
	den /= gcd;
	if (btmin <= num && num <= btmax && btmin <= den && den <= btmax) {
		set(base_type(num), base_type(den));
	}
	else {
		uint128 num_us = num < 0 ? uint128(-num) : uint128(num);
		uint128 den_us = uint128(den);
		
		typename config_traits::numerator_type num_e(unsigned_base_type(num_us >> std::numeric_limits<unsigned_base_type>::digits));
		num_e <<= std::numeric_limits<unsigned_base_type>::digits;
		num_e |= unsigned_base_type(num_us);
		if (num < 0) {
			num_e = -num_e;
		}
		
		typename config_traits::denominator_type den_e(unsigned_base_type(den_us >> std::numeric_limits<unsigned_base_type>::digits));
		den_e <<= std::numeric_limits<unsigned_base_type>::digits;
		den_e |= unsigned_base_type(den_us);
		
		set( extension_type(num_e, den_e) );
	}
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(const extension_type & v) {
	auto num = config_traits::numerator(v);
	auto den = config_traits::denominator(v);
	
	if (config_traits::fits_int64(num) && config_traits::fits_int64(den)) {
		set(config_traits::to_int64(num), config_traits::to_int64(den));
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
		EI64_UPDATE_MAX_BITS( getExtended() );
	}
}
EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(extension_type && v) {
	auto num = config_traits::numerator(v);
	auto den = config_traits::denominator(v);
	
	if (config_traits::fits_int64(num) && config_traits::fits_int64(den)) {
		set(config_traits::to_int64(num), config_traits::to_int64(den));
		assert(asExtended() == v);
		assert(num == getPq().num && den == getPq().den);
	}
	else {
		if (isExtended()) {
			getExtended() = std::move(v);
		}
		else {
			set( new extension_type(std::move(v)) );
			EI64_INC_NUM_E_ALLOC
		}
		assert(isExtended());
		EI64_UPDATE_MAX_BITS( getExtended() );
	}
}

EI64PQ_TPL_PARAMS
void
EI64PQ_CLS_NAME::set(ExtendedInt64q::extension_type* v) {
	if (v) {
		getPq().den = 0;
		m_v.ext.ptr = v;
		EI64_UPDATE_MAX_BITS(getExtended());
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
		out << v.getExtended();
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

} //end namespace CGAL

// Specializations

namespace CGAL {
namespace internal {

template<>
struct ExtendedInt64qTraits<CGAL::Gmpq> {
	typedef CGAL::Gmpq type;
	typedef CGAL::Gmpz numerator_type;
	typedef CGAL::Gmpz denominator_type;
	static void simplify(type & v);
	static bool fits_int64(const numerator_type & v);
	static int64_t to_int64(const numerator_type & v);
	static double to_double(const type & v);
	static numerator_type numerator(const type & v);
	static denominator_type denominator(const type & v);
	static CGAL::ExtendedInt64z::extension_type const & to_ei64z(const numerator_type & v);
	static uint32_t num_bits(const numerator_type &);
};

template<>
struct ExtendedInt64qTraits<boost_int1024q> {
	typedef boost_int1024q type;
	typedef boost_int1024 numerator_type;
	typedef boost_int1024 denominator_type;
	static void simplify(type & v);
	static bool fits_int64(const numerator_type & v);
	static int64_t to_int64(const numerator_type & v);
	static double to_double(const type & v);
	static numerator_type numerator(const type & v);
	static denominator_type denominator(const type & v);
	static CGAL::ExtendedInt64z::extension_type to_ei64z(const numerator_type & v);
	static uint32_t num_bits(const numerator_type & v);
};

} //end namespace internal

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_extended_allocations;

template<>
uint64_t ExtendedInt64q<CGAL::Gmpq>::number_of_allocations;

template<>
uint32_t ExtendedInt64q<CGAL::Gmpq>::max_numerator_bits;

template<>
uint32_t ExtendedInt64q<CGAL::Gmpq>::max_denominator_bits;

template<>
uint64_t ExtendedInt64q<CGAL::internal::boost_int1024q>::number_of_extended_allocations;

template<>
uint64_t ExtendedInt64q<CGAL::internal::boost_int1024q>::number_of_allocations;

template<>
uint32_t ExtendedInt64q<CGAL::internal::boost_int1024q>::max_numerator_bits;

template<>
uint32_t ExtendedInt64q<CGAL::internal::boost_int1024q>::max_denominator_bits;


}//end namespace CGAL

#if defined(BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif

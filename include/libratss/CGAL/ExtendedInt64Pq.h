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


#ifndef LIBRATSS_INT64_PQ_H
#define LIBRATSS_INT64_PQ_H

#include <CGAL/Gmpq.h>

#include <libratss/constants.h>
#include <libratss/ExtendedInt64.h>

namespace CGAL {

class Int64Pq:
    boost::totally_ordered1< Int64Pq
  , boost::ordered_field_operators2< Int64Pq, int
  , boost::ordered_field_operators2< Int64Pq, long
  , boost::ordered_field_operators2< Int64Pq, long long
  , boost::ordered_field_operators2< Int64Pq, double
  , boost::ordered_field_operators2< Int64Pq, Gmpq
  , boost::ordered_field_operators2< Int64Pq, Gmpz
  , boost::ordered_field_operators2< Int64Pq, Gmpfr
  , boost::ordered_field_operators2< Int64Pq, ExtendedInt
    > > > > > > > > >
{
public:
  typedef Tag_false  Has_gcd;
  typedef Tag_true   Has_division;
  typedef Tag_false  Has_sqrt;

  typedef Tag_true   Has_exact_ring_operations;
  typedef Tag_true   Has_exact_division;
  typedef Tag_false  Has_exact_sqrt;
  typedef CGAL::Gmpq extension_type;
  typedef CGAL::ExtendedInt::base_type base_type;
public:
  Int64Pq() {}
  Int64Pq(const Int64Pq & other);
  Int64Pq(const Int64Pq && other);
  
  Int64Pq(const CGAL::Gmpq & q);
  
  Int64Pq(int n);
  Int64Pq(unsigned int n);
  Int64Pq(long n);
  Int64Pq(unsigned long n);
  Int64Pq(unsigned long long n);
  Int64Pq(long long n);
  Int64Pq(const Gmpz& n);
  Int64Pq(const ExtendedInt & n);
  Int64Pq(const Gmpfr &f);
  
  Int64Pq(int n, int d);
  Int64Pq(signed long n, unsigned long d);
  Int64Pq(unsigned long n, unsigned long d);
  Int64Pq(const Gmpz& n, const Gmpz& d);
  Int64Pq(double d);

  Int64Pq(const std::string& str, int base = 10);
  
   ~Int64Pq();

  // Gives the memory size in bytes. (not documented yet)
  std::size_t size() const;

  ExtendedInt numerator() const;
  ExtendedInt denominator() const;

  Int64Pq operator+() const;
  Int64Pq operator-() const;

  Int64Pq& operator+=(const Int64Pq &q);
  Int64Pq& operator-=(const Int64Pq &q);
  Int64Pq& operator*=(const Int64Pq &q);
  Int64Pq& operator/=(const Int64Pq &q);

  bool operator==(const Int64Pq &q) const;
  bool operator< (const Int64Pq &q) const;

  double to_double() const;
  Sign sign() const;

  const extension_type & getExtended() const;
  extension_type & getExtended();

  // Interoperability with int
  inline Int64Pq& operator+=(int z) { return (*this) += Int64Pq(z); }
  inline Int64Pq& operator-=(int z) { return (*this) -= Int64Pq(z); }
  inline Int64Pq& operator*=(int z) { return (*this) *= Int64Pq(z); }
  inline Int64Pq& operator/=(int z) { return (*this) /= Int64Pq(z); }
  inline bool operator==(int z) const {return *this == Int64Pq(z); }
  inline bool operator< (int z) const {return *this < Int64Pq(z); }
  inline bool operator> (int z) const {return *this > Int64Pq(z); }

  // Interoperability with long
  inline Int64Pq& operator+=(long z) { return (*this) += Int64Pq(z); }
  inline Int64Pq& operator-=(long z) { return (*this) -= Int64Pq(z); }
  inline Int64Pq& operator*=(long z) { return (*this) *= Int64Pq(z); }
  inline Int64Pq& operator/=(long z) { return (*this) /= Int64Pq(z); }
  inline bool operator==(long z) const { return *this == Int64Pq(z); }
  inline bool operator< (long z) const { return *this < Int64Pq(z); }
  inline bool operator> (long z) const { return *this > Int64Pq(z); }

  // Interoperability with long long
  inline Int64Pq& operator+=(long long z) { return (*this) += Int64Pq(z); }
  inline Int64Pq& operator-=(long long z) { return (*this) -= Int64Pq(z); }
  inline Int64Pq& operator*=(long long z) { return (*this) *= Int64Pq(z); }
  inline Int64Pq& operator/=(long long z) { return (*this) /= Int64Pq(z); }
  inline bool operator==(long long z) const { return (*this) == Int64Pq(z); }
  inline bool operator< (long long z) const { return (*this) < Int64Pq(z); }
  inline bool operator> (long long z) const { return (*this) > Int64Pq(z); }

  // Interoperability with double
  inline Int64Pq& operator+=(double d) { return (*this) += Int64Pq(d); }
  inline Int64Pq& operator-=(double d) { return (*this) -= Int64Pq(d); }
  inline Int64Pq& operator*=(double d) { return (*this) *= Int64Pq(d); }
  inline Int64Pq& operator/=(double d) { return (*this) /= Int64Pq(d); }
  inline bool operator==(double d) const { return (*this) == Int64Pq(d); }
  inline bool operator< (double d) const { return (*this) < Int64Pq(d); }
  inline bool operator> (double d) const { return (*this) > Int64Pq(d); }

  // Interoperability with Gmpq
  inline Int64Pq& operator+=(Gmpq d) { return (*this) += Int64Pq(d); }
  inline Int64Pq& operator-=(Gmpq d) { return (*this) -= Int64Pq(d); }
  inline Int64Pq& operator*=(Gmpq d) { return (*this) *= Int64Pq(d); }
  inline Int64Pq& operator/=(Gmpq d) { return (*this) /= Int64Pq(d); }
  inline bool operator==(Gmpq d) const { return (*this) == Int64Pq(d); }
  inline bool operator< (Gmpq d) const { return (*this) < Int64Pq(d); }
  inline bool operator> (Gmpq d) const { return (*this) > Int64Pq(d); }
  
  // Interoperability with Gmpz
  inline Int64Pq& operator+=(Gmpz d) { return (*this) += Int64Pq(d); }
  inline Int64Pq& operator-=(Gmpz d) { return (*this) -= Int64Pq(d); }
  inline Int64Pq& operator*=(Gmpz d) { return (*this) *= Int64Pq(d); }
  inline Int64Pq& operator/=(Gmpz d) { return (*this) /= Int64Pq(d); }
  inline bool operator==(Gmpz d) const { return (*this) == Int64Pq(d); }
  inline bool operator< (Gmpz d) const { return (*this) < Int64Pq(d); }
  inline bool operator> (Gmpz d) const { return (*this) > Int64Pq(d); }

  // Interoperability with Gmpfr
  inline Int64Pq& operator+=(Gmpfr d) { return (*this) += Int64Pq(d); }
  inline Int64Pq& operator-=(Gmpfr d) { return (*this) -= Int64Pq(d); }
  inline Int64Pq& operator*=(Gmpfr d) { return (*this) *= Int64Pq(d); }
  inline Int64Pq& operator/=(Gmpfr d) { return (*this) /= Int64Pq(d); }
  inline bool operator==(Gmpfr d) const { return (*this) == Int64Pq(d); }
  inline bool operator< (Gmpfr d) const { return (*this) < Int64Pq(d); }
  inline bool operator> (Gmpfr d) const { return (*this) > Int64Pq(d); }
  
  // Interoperability with ExtendedInt
  inline Int64Pq& operator+=(ExtendedInt d) { return (*this) += Int64Pq(d); }
  inline Int64Pq& operator-=(ExtendedInt d) { return (*this) -= Int64Pq(d); }
  inline Int64Pq& operator*=(ExtendedInt d) { return (*this) *= Int64Pq(d); }
  inline Int64Pq& operator/=(ExtendedInt d) { return (*this) /= Int64Pq(d); }
  inline bool operator==(ExtendedInt d) const { return (*this) == Int64Pq(d); }
  inline bool operator< (ExtendedInt d) const { return (*this) < Int64Pq(d); }
  inline bool operator> (ExtendedInt d) const { return (*this) > Int64Pq(d); }
private:
	bool isExtended() {
		return m_isExtended;
	}
	void set(uint64_t num, uint64_t den) {
		if (isExtended()) {
			delete m_v.ptr;
			m_isExtended = false;
		}
		m_v.pq.num = num;
		m_v.pq.den = den;
	}
	void set(const extension_type & v) {
		if (isExtended()) {
			getExtended() = v;
		}
		else {
			m_v.ptr = new extension_type(v);
			m_isExtended = true;
		}
	}
private:
	union {
		struct PQ {
			uint64_t num;
			uint64_t den;
		} pq;
		extension_type * ptr;
	} m_v;
	bool m_isExtended;
};


inline
Gmpq
Gmpq::operator-() const
{
    Gmpq Res;
    mpq_neg(Res.mpq(), mpq());
    return Res;
}

inline
Gmpq
Gmpq::operator+() const
{
  return Gmpq(mpq());
}

inline
Gmpq
operator+(const Gmpq &x, const Gmpq &y)
{
    Gmpq Res;
    mpq_add(Res.mpq(), x.mpq(), y.mpq());
    return Res;
}

inline
Gmpq&
Gmpq::operator+=(const Gmpq &z)
{
    (*this + z).swap(*this);
    return *this;
}

inline
Gmpq
operator-(const Gmpq &x, const Gmpq &y)
{
    Gmpq Res;
    mpq_sub(Res.mpq(), x.mpq(), y.mpq());
    return Res;
}

inline
Gmpq&
Gmpq::operator-=(const Gmpq &z)
{
    (*this - z).swap(*this);
    return *this;
}

inline
Gmpq
operator*(const Gmpq &x, const Gmpq &y)
{
    Gmpq Res;
    mpq_mul(Res.mpq(), x.mpq(), y.mpq());
    return Res;
}

inline
Gmpq&
Gmpq::operator*=(const Gmpq &z)
{
    (*this * z).swap(*this);
    return *this;
}

inline
Gmpq
operator/(const Gmpq &x, const Gmpq &y)
{
    CGAL_precondition(y != 0);
    Gmpq Res;
    mpq_div(Res.mpq(), x.mpq(), y.mpq());
    return Res;
}

inline
Gmpq&
Gmpq::operator/=(const Gmpq &z)
{
    (*this / z).swap(*this);
    return *this;
}

inline
Gmpq& Gmpq::operator+=(const Gmpz &z){
  if(unique()){
    mpz_addmul(mpq_numref(mpq()),mpq_denref(mpq()),z.mpz());
  }else{
    Gmpq result;
    mpz_mul(mpq_numref(result.mpq()),
            mpq_denref(mpq()),
            z.mpz());
    mpz_add(mpq_numref(result.mpq()),
            mpq_numref(mpq()),
            mpq_numref(result.mpq()));
    mpz_set(mpq_denref(result.mpq()),mpq_denref(mpq()));
    swap(result);
  }
  return *this;
}

inline
Gmpq& Gmpq::operator-=(const Gmpz &z){
  if(unique()){
    mpz_submul(mpq_numref(mpq()),mpq_denref(mpq()),z.mpz());
  }else{
    Gmpq result;
    mpz_mul(mpq_numref(result.mpq()),
            mpq_denref(mpq()),
            z.mpz());
    mpz_sub(mpq_numref(result.mpq()),
            mpq_numref(mpq()),
            mpq_numref(result.mpq()));
    mpz_set(mpq_denref(result.mpq()),mpq_denref(mpq()));
    swap(result);
  }
  return *this;
}

inline
Gmpq& Gmpq::operator*=(const Gmpz &z){
  if(unique()){
    mpz_mul(mpq_numref(mpq()),mpq_numref(mpq()),z.mpz());
    mpq_canonicalize(mpq());
  }else{
    Gmpq result;
    mpz_mul(mpq_numref(result.mpq()),mpq_numref(mpq()),z.mpz());
    mpz_set(mpq_denref(result.mpq()),mpq_denref(mpq()));
    mpq_canonicalize(result.mpq());
    swap(result);
  }
  return *this;
}

inline
Gmpq& Gmpq::operator/=(const Gmpz &z){
  if(unique()){
    mpz_mul(mpq_denref(mpq()),mpq_denref(mpq()),z.mpz());
    mpq_canonicalize(mpq());
  }else{
    Gmpq result;
    mpz_mul(mpq_denref(result.mpq()),mpq_denref(mpq()),z.mpz());
    mpz_set(mpq_numref(result.mpq()),mpq_numref(mpq()));
    mpq_canonicalize(result.mpq());
    swap(result);
  }
  return *this;
}

inline
double
Gmpq::to_double() const
{ return mpq_get_d(mpq()); }

inline
Sign
Gmpq::sign() const
{ return static_cast<Sign>(mpq_sgn(mpq())); }

inline
std::ostream&
operator<<(std::ostream& os, const Gmpq &z)
{
  os << z.numerator() << "/" << z.denominator();
  return os;
}

// inline
// std::istream&
// operator>>(std::istream& is, Gmpq &z)
// {
//   char c;
//   Gmpz n, d;
//   is >> n;
//   is >> c;
//   //CGAL_assertion(!is || c == '/');
//   if (c != '/'){
//     is.setstate(std::ios_base::failbit);
//     return is;
//   }
//   is >> d;
//   if (!is.fail()) {
//     z = Gmpq(n,d);
//   }
//   return is;
// }



inline
std::istream&
operator>>(std::istream& is, Gmpq &z)
{
  internal::read_float_or_quotient<Gmpz,Gmpq>(is, z);
  return is;
}


inline Gmpq min BOOST_PREVENT_MACRO_SUBSTITUTION(const Gmpq& x,const Gmpq& y){
  return (x<=y)?x:y;
}
inline Gmpq max BOOST_PREVENT_MACRO_SUBSTITUTION(const Gmpq& x,const Gmpq& y){
  return (x>=y)?x:y;
}

} //namespace CGAL

#if defined(BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif // CGAL_GMPQ_TYPE_H

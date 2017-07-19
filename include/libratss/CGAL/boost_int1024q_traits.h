#ifndef LIBRATSS_BOOST_INT_1024_Q_TRAITS_H
#define LIBRATSS_BOOST_INT_1024_Q_TRAITS_H
#pragma once

#include <CGAL/Algebraic_structure_traits.h>
#include <CGAL/Gmpq.h>

#include <boost/multiprecision/cpp_int.hpp>

#include <libratss/Conversion.h>

namespace CGAL {

// AST for boost
template<>
class Algebraic_structure_traits<
	boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
		>
	>
> : public Algebraic_structure_traits_base<
		boost::multiprecision::number<
			boost::multiprecision::rational_adaptor<
				boost::multiprecision::int1024_t::backend_type
			>
		>,
		Field_tag
	> 
{
private:
	typedef boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
			>
		> Type;
	typedef CGAL::Gmpq BaseType;
	typedef Algebraic_structure_traits<BaseType> BaseAst;
public:
	typedef Tag_false            Is_exact;
	typedef Tag_false            Is_numerical_sensitive;
	
	class Is_square: public std::binary_function<Type, Type&, bool >  {
	public:
		inline bool operator()( const Type& x_, Type& y ) const {
			auto xtmp = LIB_RATSS_NAMESPACE::convert<BaseType>(x_);
			auto ytmp = LIB_RATSS_NAMESPACE::convert<BaseType>(y);
			bool ret = m_p(xtmp, ytmp);
			y = LIB_RATSS_NAMESPACE::convert<Type>(ytmp);
			return ret;
		}
		inline bool operator()( const Type& x) const {
			return m_p( LIB_RATSS_NAMESPACE::convert<BaseType>(x) );
		}
	private:
		typename BaseAst::Is_square m_p;
	};

	class Simplify: public std::unary_function< Type&, void > {
	public:
		void operator()(Type& /*x*/) const {}
	};

};

// RET for ExtendedInt64q

template<>
class Real_embeddable_traits<
	boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
		>
	>

>:
public 
	INTERN_RET::Real_embeddable_traits_base<
		boost::multiprecision::number<
			boost::multiprecision::rational_adaptor<
				boost::multiprecision::int1024_t::backend_type
			>
		>,
		CGAL::Tag_true
	>
{
private:
	typedef boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
		>
	> Type;
	typedef CGAL::Gmpq BaseType;
	typedef Real_embeddable_traits<BaseType> RetBase;
public:

	class Sgn: public std::unary_function< Type, ::CGAL::Sign > {
	public:
		::CGAL::Sign operator()( const Type& x ) const {
			assert(false);
			return CGAL::sign( x.sign() );
		}
	};

	class To_double: public std::unary_function< Type, double > {
	public:
		double operator()( const Type& x ) const {
			return x.convert_to<double>();
		}
	};

	class To_interval: public std::unary_function< Type, std::pair< double, double > > {
	public:
		std::pair<double, double> operator()(const Type & x ) const {
			return m_p( LIB_RATSS_NAMESPACE::convert<BaseType>(x) );
		}
	private:
		typename RetBase::To_interval m_p;
	};
};

};

#endif
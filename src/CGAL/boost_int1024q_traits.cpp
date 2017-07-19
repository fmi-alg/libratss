#include <libratss/CGAL/boost_int1024q_traits.h>
#include <libratss/Conversion.h>

namespace CGAL {

using AST = Algebraic_structure_traits<
	boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
		>
	>
>;

bool AST::Is_square::operator()( const Type& x_, Type& y ) const {
	auto xtmp = LIB_RATSS_NAMESPACE::convert<BaseType>(x_);
	auto ytmp = LIB_RATSS_NAMESPACE::convert<BaseType>(y);
	bool ret = m_p(xtmp, ytmp);
	y = LIB_RATSS_NAMESPACE::convert<Type>(ytmp);
	return ret;
}

bool AST::Is_square::operator()( const Type& x) const {
	return m_p( LIB_RATSS_NAMESPACE::convert<BaseType>(x) );
}

using RET = Real_embeddable_traits<
	boost::multiprecision::number<
		boost::multiprecision::rational_adaptor<
			boost::multiprecision::int1024_t::backend_type
		>
	>
>;

std::pair<double, double> RET::To_interval::operator()(const Type & x ) const {
	return m_p( LIB_RATSS_NAMESPACE::convert<BaseType>(x) );
}


}//end namespace CGAL
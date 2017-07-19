//Base on GMP_arithmetic_kernel.h of CGAL. Original header follows

// Copyright (c) 2008 Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
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
// Author(s)     : Michael Hemmer <mhemmer@uni-mainz.de>
//
// ============================================================================
//
//    \brief provide class Arithmetic_kernel, a collection of number types. 
//

//end of original header

#ifndef LIBRATSS_CGAL_EXTENDED_INT64_ARITHMETIC_KERNEL_H
#define LIBRATSS_CGAL_EXTENDED_INT64_ARITHMETIC_KERNEL_H

#include <CGAL/config.h>

#ifdef CGAL_USE_GMP 

#include <CGAL/Arithmetic_kernel/Arithmetic_kernel_base.h>
#include <CGAL/Get_arithmetic_kernel.h>

#include <libratss/CGAL/ExtendedInt64z.h>
#include <libratss/CGAL/ExtendedInt64Pq.h>

namespace CGAL {

/*! \ingroup CGAL_Arithmetic_kernel
 *  \brief  The GMP set of exact number types
 */
template<typename T_EXTENSION_TYPE>
class ExtendedInt_arithmetic_kernel : public internal::Arithmetic_kernel_base {
public:
	typedef CGAL::ExtendedInt64Pq<T_EXTENSION_TYPE> Rational;
	typedef typename Rational::numerator_type Integer;
};

template <>
struct Get_arithmetic_kernel< ExtendedInt64Pq<CGAL::Gmpq> >{
	typedef ExtendedInt_arithmetic_kernel<CGAL::Gmpq> Arithmetic_kernel;
};
    
template<>
struct Get_arithmetic_kernel< ExtendedInt64z > {
	typedef ExtendedInt_arithmetic_kernel<CGAL::Gmpq> Arithmetic_kernel;
};


} //namespace CGAL

#endif //CGAL_USE_GMP

#endif // CGAL_ARITHMETIC_KERNEL_H
// EOF

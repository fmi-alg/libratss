#include <libratss/ProjectS2.h>

namespace LIB_RATSS_NAMESPACE {

void ProjectS2::snap(const mpfr::mpreal &flxs, const mpfr::mpreal &flys, const mpfr::mpreal &flzs, mpq_class &xs, mpq_class &ys, mpq_class &zs, int significands) const {
	using ConstRefWrap = internal::ReferenceWrapper<const mpfr::mpreal>;
	using RefWrap = internal::ReferenceWrapper<mpq_class>;
	using ConstRefWrapIt = internal::ReferenceWrapperIterator<ConstRefWrap * >;
	using RefWrapIt = internal::ReferenceWrapperIterator<RefWrap *>;
	
	ConstRefWrap input[3] = { flxs, flys, flzs};
	RefWrap output[3] = {xs, ys, zs};
	ConstRefWrapIt inputBegin(input);
	ConstRefWrapIt inputEnd(input+3);
	RefWrapIt outputBegin(output);
	
	ProjectSN::snap<ConstRefWrapIt, RefWrapIt>(inputBegin, inputEnd, outputBegin, ST_FX | ST_PLANE | ST_NORMALIZE, significands);

	assert(xs*xs + ys*ys + zs*zs == 1);
}

}//end namespace LIB_RATSS_NAMESPACE
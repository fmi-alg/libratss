#include "stats.h"


namespace LIB_RATSS_NAMESPACE {

BitCount::BitCount() {}

void BitCount::update(mpq_class v) {
	v.canonicalize();
	std::size_t sizeNum = mpz_sizeinbase(v.get_num().get_mpz_t(), 2);
	std::size_t sizeDenom = mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	
	numBits.update(sizeNum);
	denomBits.update(sizeDenom);

	sizeNum = __GMPXX_BITS_TO_LIMBS(sizeNum);
	sizeDenom = __GMPXX_BITS_TO_LIMBS(sizeDenom);
	
	numLimbs.update(sizeNum);
	denomLimbs.update(sizeDenom);
	
}

void BitCount::print(std::ostream & out) const {
	out << "Bit counts:\n";
	numBits.print(out, "\tNumerator ");
	out << '\n';
	denomBits.print(out, "\tDenominator ");
	out << '\n';
	out << "Limb counts:\n";
	numLimbs.print(out, "\tNumerator ");
	out << '\n';
	denomLimbs.print(out, "\tDenominator ");
}

std::ostream & operator<<(std::ostream& out, const BitCount & bc) {
	bc.print(out);
	return out;
}

}//end namespace LIB_RATSS_NAMESPACE
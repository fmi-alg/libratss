#include "stats.h"


namespace LIB_RATSS_NAMESPACE {

BitCount::BitCount() :
count(0),
maxNofBitsNum(0),
maxNofBitsDenom(0),
sumNofBitsNum(0),
sumNofBitsDenom(0)
{}

void BitCount::update(mpq_class v) {
	v.canonicalize();
	++count;
	std::size_t sizeNum = mpz_sizeinbase(v.get_num().get_mpz_t(), 2);
	std::size_t sizeDenom = mpz_sizeinbase(v.get_den().get_mpz_t(), 2);
	
	maxNofBitsNum = std::max<std::size_t>(sizeNum, maxNofBitsNum);
	maxNofBitsDenom = std::max<std::size_t>(sizeDenom, maxNofBitsDenom);
	
	sumNofBitsNum += sizeNum;
	sumNofBitsDenom += sizeDenom;
}

void BitCount::print(std::ostream & out) const {
	out << "MaxSizeNum   [Bits]: " << maxNofBitsNum << '\n';
	out << "MaxSizeDenom [Bits]: " << maxNofBitsDenom << '\n';
	if ( count) {
		out << "MeanSizeNum [Bits]: " << (sumNofBitsDenom/(3*count)) << '\n';
		out << "MeanSizeDenom [Bits]: " << (sumNofBitsDenom/(3*count));
	}
}

std::ostream & operator<<(std::ostream& out, const BitCount & bc) {
	bc.print(out);
	return out;
}

}//end namespace LIB_RATSS_NAMESPACE
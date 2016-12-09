#include <libratss/ProjectS2.h>

namespace LIB_RATSS_NAMESPACE {

void ProjectS2::snap(const mpfr::mpreal &flxs, const mpfr::mpreal &flys, const mpfr::mpreal &flzs, mpq_class &xs, mpq_class &ys, mpq_class &zs, int precision) const {

	//shouldn't we clip coordinates to the sphere in 3d?
	//then this would always yield the "closest" point on the sphere
	mpfr::mpreal squaredDistance3d( m_calc.squaredDistance(flxs, flys, flzs) );
	if (squaredDistance3d > 1) { //move it on to the unit sphere
		int prevPrecision = std::max<int>(std::max<int>(flxs.getPrecision(), flys.getPrecision()), flzs.getPrecision());
		mpfr::mpreal vecLen = m_calc.sqrt(squaredDistance3d);
		mpfr::mpreal flxs_n(flxs), flys_n(flys), flzs_n(flzs);
		flxs_n.setPrecision(prevPrecision*4);
		flys_n.setPrecision(prevPrecision*4);
		flzs_n.setPrecision(prevPrecision*4);
		vecLen.setPrecision(std::max<int>(prevPrecision*4, vecLen.getPrecision()));
		flxs_n /= vecLen;
		flys_n /= vecLen;
		flzs_n /= vecLen;
		flxs_n.setPrecision(prevPrecision, MPFR_RNDZ);
		flys_n.setPrecision(prevPrecision, MPFR_RNDZ);
		flzs_n.setPrecision(prevPrecision, MPFR_RNDZ);
		if (m_calc.squaredDistance(flxs_n, flys_n, flzs_n) <= 1) {
			return snap(flxs_n, flys_n, flzs_n, xs, ys, zs, precision);
		}
		//else: let's try to move it on to the unit circle in 2D
		//if that doesn't work out, freak out
	}

	mpfr::mpreal sp_flx, sp_fly, sp_flz;
	
	//project with stereographic projection
	auto pos = sphere2Plane(flxs, flys, flzs, sp_flx, sp_fly, sp_flz);
	{ //the following is only good if we try to project points very close to the spheres
		mpfr::mpreal sp_sqd(m_calc.squaredDistance(sp_flx, sp_fly));
		if (sp_sqd > 1) {
			int prevPrecision = std::max<int>(sp_flx.getPrecision(), sp_fly.getPrecision());
			sp_sqd.setPrecision(4*sp_sqd.getPrecision());
			sp_flx.setPrecision(4*sp_sqd.getPrecision());
			sp_fly.setPrecision(4*sp_sqd.getPrecision());
			mpfr::mpreal sp_sqrt = m_calc.sqrt(sp_sqd);
			sp_flx /= sp_sqrt;
			sp_fly /= sp_sqrt;
			sp_flx.setPrecision(prevPrecision, MPFR_RNDZ);
			sp_fly.setPrecision(prevPrecision, MPFR_RNDZ);
		}
		assert(m_calc.squaredDistance(sp_flx, sp_fly) <= 1);
	}
	
	//set the precision for our clipping
	sp_flx.setPrecision(precision/2, MPFR_RNDZ);
	sp_fly.setPrecision(precision/2, MPFR_RNDZ);
	
	//clip to rational coordinates
	mpq_class sp_x = Conversion<mpfr::mpreal>::toMpq(sp_flx);
	mpq_class sp_y = Conversion<mpfr::mpreal>::toMpq(sp_fly);
	mpq_class sp_z = Conversion<mpfr::mpreal>::toMpq(sp_flz);
	
// 	assert(sp_x*sp_x + sp_y * sp_y <= 1);
	
	//reproject onto sphere
	plane2Sphere(sp_x, sp_y, sp_z, pos, xs, ys, zs);
	
	assert((flzs < 0 && zs < 0) || (flzs >= 0 && zs >= 0));
	assert(xs*xs + ys*ys + zs*zs == 1);
	
	{
		mpq_class sp_x2, sp_y2, sp_z2;
		sphere2Plane(xs, ys, zs, sp_x2, sp_y2, sp_z2);
		if (sp_x != sp_x2 || sp_y != sp_y2) {
			std::cerr << "Bijektion failed:" << std::endl;
			std::cerr << sp_x << "!=?" << sp_x2.get_mpq_t() << std::endl;
			std::cerr << sp_y << "!=?" << sp_y2.get_mpq_t() << std::endl;
			std::cerr << sp_z << "!=?" << sp_z2.get_mpq_t() << std::endl;
		}
		assert(sp_x == sp_x2 && sp_y == sp_y2);
	}
}

}//end namespace LIB_RATSS_NAMESPACE
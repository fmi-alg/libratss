#ifndef LIBRATSS_UTIL_READERS_H
#define LIBRATSS_UTIL_READERS_H

#include <libratss/constants.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>
#include <libratss/ProjectSN.h>

namespace LIB_RATSS_NAMESPACE {

class FileReader {
public:
	FileReader(const BasicCmdLineOptions & cfg, InputOutput & io);
	~FileReader();
public:
	///@param visitor functor with operator(const FloatPoint & floatPoint, const RationalPoint & rationalPoint);
	template<typename T_VISITOR>
	void visit(T_VISITOR visitor);
private:
	const BasicCmdLineOptions & m_cfg;
	InputOutput & m_io;
	FloatPoint m_fp;
	RationalPoint m_rp;
};

} // end LIB_RATSS_NAMESPACE


//implementation

namespace LIB_RATSS_NAMESPACE {

template<typename T_VISITOR>
void FileReader::visit(T_VISITOR visitor) {
	auto & io = m_io;
	const auto & cfg = m_cfg;
	auto & ip = m_fp;
	auto & op = m_rp;
	
	ProjectSN proj;

	if (cfg.progress) {
		io.info() << std::endl;
	}
	std::size_t counter = 0;

	while( io.input().good() ) {
		for( ; io.input().good() && io.input().peek() == '\n'; ) {
			io.input().get();
			io.output().put('\n');
		}
		if (!io.input().good()) {
			break;
		}
		bool opFromIp = !cfg.rationalPassThrough;
		if (cfg.rationalPassThrough) {
			op.assign(io.input(), cfg.inFormat, cfg.precision);
			if (!op.valid()) {
				if (!cfg.normalize) {
					std::cerr << "Input point read that is not on sphere but no normalization was requested" << std::endl;
				}
				else {
					ip.assign(op.coords.begin(), op.coords.end(), cfg.precision);
					opFromIp = true;
				}
			}
		}
		else {
			ip.assign(io.input(), cfg.inFormat, cfg.precision);
		}
		
		if (opFromIp) {
			if (cfg.normalize) {
				if (cfg.verbose) {
					io.info() << "Normalizing (" << ip << ") to ";
				}
				ip.normalize();
				if (cfg.verbose) {
					io.info() << '(' << ip << ')' << '\n';
				}
			}
			ip.setPrecision(cfg.precision);
			op.clear();
			op.resize(ip.coords.size());
			proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), cfg.snapType, cfg.significands);
		}
		visitor(ip, op);
		
		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			io.info() << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
	if (cfg.progress) {
		io.info() << std::endl;
	}
}


} //end namespace LIB_RATSS_NAMESPACE

#endif
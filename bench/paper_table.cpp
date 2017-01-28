#include <time.h>
#include <sys/time.h>
#include <cstdlib> 
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/Readers.h>
#include "../common/stats.h"

class MemUsage {
public:
	MemUsage() {
		update();
	}
	void update() {
		const char* statm_path = "/proc/self/statm";

		FILE * f = fopen(statm_path,"r");
		if(!f){
			perror(statm_path);
			return;
		}
		if(7 != fscanf(f,"%ld %ld %ld %ld %ld %ld %ld", &size,&resident,&share,&text,&lib,&data,&dt)) {
			perror(statm_path);
		}
		fclose(f);
	}
	unsigned long size,resident,share,text,lib,data,dt;
};


class TimeMeasurer {
private:
	struct timeval m_begin, m_end;
public:
	TimeMeasurer() {
		::memset(&m_begin, 0, sizeof(struct timeval));
		::memset(&m_end, 0, sizeof(struct timeval));
	}
	
	~TimeMeasurer() {}
	
	inline void begin() {
		gettimeofday(&m_begin, NULL);
	}
	
	inline void end() {
		gettimeofday(&m_end, NULL);
	}
	
	inline long beginTime() const {
		return m_begin.tv_sec;
	}
	
	/** @return returns the elapsed time in useconds  */
	inline long elapsedTime() const {
		long mtime, seconds, useconds;
		seconds  = m_end.tv_sec  - m_begin.tv_sec;
		useconds = m_end.tv_usec - m_begin.tv_usec;
		mtime = (long)((double)((seconds) * 1000*1000 + useconds) + 0.5);
		return mtime;
	}
	
	inline long elapsedUseconds() const {
		return elapsedTime();
	}

	inline long elapsedMilliSeconds() const {
		return elapsedTime()/1000;
	}
	
	inline long elapsedSeconds() const {
		return elapsedTime()/1000000;
	}

	inline long elapsedMinutes() {
		return elapsedSeconds()/60;
	}
};

using namespace LIB_RATSS_NAMESPACE;

class Config: public ratss::BasicCmdLineOptions {
public:
	Config() {}
	virtual ~Config() {}
public:
	void help(std::ostream & out) const {
		out << "prg OPTIONS\n"
			"Options:\n";
		BasicCmdLineOptions::options_help(out);
		out << std::endl;
	}
	void print(std::ostream & out) const {
		BasicCmdLineOptions::options_selection(out);
	}
};

std::string ec2Str(int significand, int st) {
	std::string ret;
	if (st & ProjectSN::ST_AUTO_POLICY_MIN_MAX_DENOM) {
		ret = "md";
	}
	else if (st & ProjectSN::ST_AUTO_POLICY_MIN_SUM_DENOM) {
		ret = "msd";
	}
	else if (st & ProjectSN::ST_AUTO_POLICY_MIN_TOTAL_LIMBS) {
		ret = "ml";
	}
	else if (st & ProjectSN::ST_AUTO_POLICY_MIN_SQUARED_DISTANCE) {
		ret = "md2";
	}
	else if (st & ProjectSN::ST_AUTO_POLICY_MIN_MAX_NORM) {
		ret = "mmn";
	}
	else if (st & ProjectSN::ST_FX) {
		ret = "fx";
	}
	else if (st & ProjectSN::ST_CF) {
		ret = "cf";
	}
	else if (st & ProjectSN::ST_JP) {
		ret = "jp";
	}
	else {
		throw std::runtime_error("Invalid EntryConfig");
	}
	return ret + ":" + std::to_string(significand);
}

struct Stats {
	Calc c;
	MinMaxMeanStats<mpfr::mpreal> diff;
	MinMaxMeanStats<std::size_t> denom; //denom in bits
	TimeMeasurer tm;
	void start() {
		diff.reset();
		denom.reset();
		tm.begin();
	}
	void stop() {
		tm.end();
	}
	void update(const std::vector<mpfr::mpreal> & ip, const RationalPoint & sp) {
		assert(ip.size() == sp.coords.size());

		denom.update( c.maxDenom(sp.coords.begin(), sp.coords.end()) );
		diff.update( Conversion<mpq_class>::toMpreal(c.maxNorm(ip.begin(), ip.end(), sp.coords.begin()), 256));
	}
};

std::ostream & operator<<(std::ostream & out, const Stats & s) {
	out << s.diff.sum()/s.diff.count() << " " <<  s.denom.mean() << " " << s.denom.max() << " " << (double) s.tm.elapsedUseconds()/s.diff.count();
	return out;
}

int main(int argc, char ** argv) {
	Config cfg;

	bool ret = cfg.parse(argc, argv);
	if (ret <= 0) {
		cfg.help(std::cerr);
		return ret;
	}
	
	InputOutput io;
	io.setInput(cfg.inFileName);
	io.setOutput(cfg.outFileName);

	//print the header
	io.output() << "#snaptype:bits mean-error mean-denom[2^] max-denom[2^] time[us]" << std::endl;
	
	
	std::vector< std::vector<mpfr::mpreal> > points;
	
	FloatPoint ip;

	if (cfg.progress) {
		io.info() << std::endl;
	}
	std::size_t counter = 0;

	while( io.input().good() ) {
		for( ; io.input().good() && io.input().peek() == '\n'; ) {
			io.input().get();
		}
		if (!io.input().good()) {
			break;
		}
		ip.assign(io.input(), cfg.inFormat, cfg.precision);
		ip.setPrecision(cfg.precision);
		ip.normalize();
		points.emplace_back( std::move(ip.coords) );
		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			io.info() << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
	if (cfg.progress) {
		io.info() << std::endl;
	}
	std::size_t maxDimension = 0;
	for(const auto & x : points) {
		maxDimension = std::max<std::size_t>(maxDimension, x.size());
	}
	
	RationalPoint op(maxDimension);
	ProjectSN proj;
	Stats stats;

	//now do the calculations
	std::vector<int> significands = {{23, 31, 53, 113}};
	std::vector<int> snappingTypes;
	
	if (maxDimension == 3) {
		snappingTypes = {{
			ProjectSN::ST_FX,
			ProjectSN::ST_CF,
			ProjectSN::ST_JP,
			ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_MAX_DENOM,
			ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_TOTAL_LIMBS,
			ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_SUM_DENOM,
			ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_SQUARED_DISTANCE,
			ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_MAX_NORM
		}};
	}
	else {
		snappingTypes = {{
			ProjectSN::ST_FX,
			ProjectSN::ST_CF,
// 			(ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_MAX_DENOM) - ProjectSN::ST_AUTO_JP,
// 			(ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_TOTAL_LIMBS) - ProjectSN::ST_AUTO_JP,
// 			(ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_SUM_DENOM) - ProjectSN::ST_AUTO_JP,
// 			(ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_SQUARED_DISTANCE) - ProjectSN::ST_AUTO_JP,
// 			(ProjectSN::ST_AUTO_ALL|ProjectSN::ST_AUTO_POLICY_MIN_MAX_NORM) - ProjectSN::ST_AUTO_JP
		}};
	}

	for(int significand : significands) {
		for(int st : snappingTypes) {
			st |= ProjectSN::ST_PLANE | ProjectSN::ST_NORMALIZE;
			io.output() << ec2Str(significand, st) << " ";
			stats.start();
			for(const std::vector<mpfr::mpreal> & point : points) {
				proj.snap(point.begin(), point.end(), op.coords.begin(), st, significand);
				stats.update(point, op);
			}
			stats.stop();
			io.output() << stats << std::endl;
		}
	}
	io.output() << std::flush;
	return 0;
}
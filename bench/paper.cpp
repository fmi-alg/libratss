#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/Readers.h>
#include "../common/stats.h"

#include <array>

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

struct StatEntry {
	mpq_class diff;
	mpz_class denom;
};


using EntryConfig = std::pair<int, int>;

std::string ec2Str(const EntryConfig & ec) {
	std::string ret;
	if (ec.first & ProjectSN::ST_FX) {
		ret = "fx";
	}
	else if (ec.first & ProjectSN::ST_CF) {
		ret = "cf";
	}
	else if (ec.first & ProjectSN::ST_JP) {
		ret = "jp";
	}
	else {
		throw std::runtime_error("Invalid EntryConfig");
	}
	return ret + ":" + std::to_string(ec.second);
}

constexpr std::size_t num_entries = 9;

std::array<EntryConfig, num_entries> entryConfigs = {
	EntryConfig(ProjectSN::ST_FX | ProjectSN::ST_PLANE, 10),
	EntryConfig(ProjectSN::ST_FX | ProjectSN::ST_PLANE, 20),
	EntryConfig(ProjectSN::ST_FX | ProjectSN::ST_PLANE, 30),
	
	EntryConfig(ProjectSN::ST_CF | ProjectSN::ST_PLANE, 10),
	EntryConfig(ProjectSN::ST_CF | ProjectSN::ST_PLANE, 20),
	EntryConfig(ProjectSN::ST_CF | ProjectSN::ST_PLANE, 30),
	
	EntryConfig(ProjectSN::ST_JP | ProjectSN::ST_PLANE, 10),
	EntryConfig(ProjectSN::ST_JP | ProjectSN::ST_PLANE, 20),
	EntryConfig(ProjectSN::ST_JP | ProjectSN::ST_PLANE, 30),
};

struct PointStatEntry {
	std::array<StatEntry, num_entries> data;
	PointStatEntry() {}
	PointStatEntry(const PointStatEntry & other) : data(other.data) {}
	PointStatEntry(PointStatEntry && other) : data(std::move(other.data)) {}
	PointStatEntry & operator=(const PointStatEntry & other) { data = other.data; return *this; }
	PointStatEntry & operator=(PointStatEntry && other) { data = std::move(other.data); return *this; }
	PointStatEntry(const FloatPoint & src, RationalPoint & tmp, ProjectSN & proj) { assign(src, tmp, proj); }
	void assign(const FloatPoint & src, RationalPoint & tmp, ProjectSN & proj);
	void print(std::ostream & out) const;
};

void PointStatEntry::assign(const FloatPoint& ip, RationalPoint& op, ProjectSN& proj) {
	using std::abs;
	
	op.clear();
	op.resize(ip.coords.size());
	
	MinMaxMeanStats<mpz_class> denom;
	MinMaxMeanStats<mpq_class> diff;
	
	for(std::size_t i(0); i < num_entries; ++i) {
		const EntryConfig & ec = entryConfigs.at(i);
		if (ec.first & ProjectSN::ST_JP && (ip.coords.size() > 3)) {
			data[i].denom = -1;
			data[i].diff = -1;
			continue;
		}
		proj.snap(ip.coords.begin(), ip.coords.end(), op.coords.begin(), ec.first, ec.second);
		
		diff.reset();
		denom.reset();
		for(std::size_t i(0), s(ip.coords.size()); i < s; ++i) {
			diff.update( abs(op.coords[i] - Conversion<mpfr::mpreal>::toMpq(ip.coords[i])) );
			denom.update(op.coords[i].get_den());
		}
		data[i].denom = denom.max();
		data[i].diff = diff.max();
	}
}

void PointStatEntry::print(std::ostream& out) const {
	out << data[0].diff << ' ' << data[0].denom;
	for(std::size_t i(1); i < num_entries; ++i) {
		out << ' ' << data[i].diff << ' ' << data[i].denom;
	}
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
	io.output() << "#";
	for(const EntryConfig & ec : entryConfigs) {
		io.output() << ec2Str(ec) << ' ';
	}
	io.output() << std::endl;
	
	FloatPoint ip;
	RationalPoint op;
	PointStatEntry pse;
	
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
		ip.assign(io.input(), cfg.inFormat, cfg.precision);
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
		pse.assign(ip, op, proj);
		
		pse.print(io.output());
		io.output() << '\n';

		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			io.info() << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
	if (cfg.progress) {
		io.info() << std::endl;
	}
	io.output() << std::flush;
	return 0;
}
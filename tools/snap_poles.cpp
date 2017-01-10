#include <libratss/ProjectSN.h>
#include <libratss/util/BasicCmdLineOptions.h>
#include <libratss/util/InputOutputPoints.h>
#include <libratss/util/InputOutput.h>

/**
  * InputLine r g b a
  */
  
using namespace LIB_RATSS_NAMESPACE;

struct RGBA {
	RGBA() : RGBA(0,0,0,0) {}
	RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

std::ostream & operator<<(std::ostream & out, const RGBA & c) {
	out << (int) c.r << ' ' << (int) c.g << ' ' << (int) c.b << ' ' << (int) c.a;
}

class Config: public BasicCmdLineOptions {
public:
	Config() {}
	virtual ~Config() {}
public:
	void help(std::ostream & out) {
		out << "snap_poles OPTIONS:\n";
		BasicCmdLineOptions::options_help(out);
		out << std::endl;
	}
	void print(std::ostream & out) {
		BasicCmdLineOptions::options_selection(out);
		out << std::endl;
	}
};

int main(int argc, char ** argv) {
	Config cfg;
	
	int ret = cfg.parse(argc, argv);
	if (ret <= 0) {
		cfg.help(std::cout);
	}
	
	InputOutput io;
	io.setInput(cfg.inFileName);
	io.setOutput(cfg.outFileName);
	
	ProjectSN proj;
	FloatPoint ip;
	
	std::map<PositionOnSphere, RGBA> colMap;
	colMap[SP_DIM1_POSITIVE] = RGBA(255, 0, 0, 255);
	colMap[SP_DIM1_NEGATIVE] = RGBA(255, 0, 0, 120);
	colMap[SP_DIM2_POSITIVE] = RGBA(0, 255, 0, 255);
	colMap[SP_DIM2_NEGATIVE] = RGBA(0, 255, 0, 120);
	colMap[SP_DIM3_POSITIVE] = RGBA(0, 0, 255, 255);
	colMap[SP_DIM3_NEGATIVE] = RGBA(0, 0, 255, 120);
	
	std::stringstream ss;
	std::string line;
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
		
		std::getline(io.input(), line);
		ss << line;
		ip.assign(ss, cfg.inFormat, cfg.precision);
		ss.clear();
		if (cfg.normalize) {
			ip.normalize();
		}
		
		PositionOnSphere pos = proj.positionOnSphere(ip.coords.begin(), ip.coords.end());
		
		const RGBA * color = 0;
		try {
			color = &colMap.at(pos);
		}
		catch (std::out_of_range & e) {
			std::cout << "Discarding point " << line << " with snap pole=" << (int) pos << std::endl;
		}
		if (color) {
			io.output() << line << ' ' << *color << '\n';
		}
		
		++counter;
		if (cfg.progress && counter % 1000 == 0) {
			io.info() << '\xd' << counter/1000 << "k" << std::flush;
		}
	}
}

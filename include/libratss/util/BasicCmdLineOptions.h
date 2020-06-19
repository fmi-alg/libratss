#ifndef LIB_RATSS_BASIC_CMD_LINE_OPTIONS_H
#define LIB_RATSS_BASIC_CMD_LINE_OPTIONS_H
#pragma once

#include <libratss/constants.h>
#include <libratss/util/InputOutputPoints.h>
#include <exception>
#include <map>

namespace LIB_RATSS_NAMESPACE {

/** This is a simple class to parse command line options usualy needed when dealing with programs using libratss
  * 
  *
  *
  *
  */

class BasicCmdLineOptions {
public:
	class ParseError: public std::exception {
	private:
		std::string m_msg;
	public:
		ParseError(const std::string & what) : m_msg(what) {}
		virtual ~ParseError() throw() {}
		virtual const char* what() const throw() override { return m_msg.c_str(); }
	};

	class SnapTypeHelper final {
	public:
		SnapTypeHelper();
		~SnapTypeHelper() {}
	public:
		inline std::vector<int> const & types() const { return m_st; }
	public:
		std::string description(SnapType st) const;
		std::string toString(int st) const;
		int fromString(std::string const & str) const;
	private:
		std::vector<int> m_st;
		std::map<int, std::string> m_st2str;
		std::map<std::string, int> m_str2st;
		std::map<int, std::string> m_desc;
	};

	typedef enum { BM_SIGNIFICANDS, BM_EPSILON, BM_MAX_DEN} BoundMode;
	typedef enum { SM_NONE=0x0, SM_SUM=0x1, SM_EACH=0x2, SM_SIZE_IN_BITS=0x4, SM_DISTANCE_RATIONAL=0x8, SM_DISTANCE_DOUBLE=0x10} StatsMode;
public:
	std::string inFileName;
	std::string outFileName;
	int precision{-1};
	int significands{-1};
	mpq_class epsilon;
	mpz_class maxDen;
	BoundMode boundMode{BM_SIGNIFICANDS};
	int snapType{ST_NONE};
	bool verbose{false};
	bool progress{false};
	bool rationalPassThrough{false};
	FloatPoint::Format inFormat{FloatPoint::FM_CARTESIAN_FLOAT};
	RationalPoint::Format outFormat{RationalPoint::FM_RATIONAL};
	int statsMode{SM_NONE};
public:
	BasicCmdLineOptions();
	virtual ~BasicCmdLineOptions();
public:
	int parse(int argc, char ** argv);
public:
	//override this to parse extra options, return true if token was consumed
	virtual bool parse(const std::string & currentToken,int & i, int argc, char ** argv);
	
	//override this to do stuff after parsing completed
	virtual void parse_completed();
public:
	void options_help(std::ostream & out) const;
public:
	void options_selection(std::ostream & out) const;
protected:
	SnapTypeHelper m_sth;
};

}//end namespace LIB_RATSS_NAMESPACE

#endif
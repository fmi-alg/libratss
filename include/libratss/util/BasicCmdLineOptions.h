#ifndef LIB_RATSS_BASIC_CMD_LINE_OPTIONS_H
#define LIB_RATSS_BASIC_CMD_LINE_OPTIONS_H
#pragma once

#include <libratss/constants.h>
#include <libratss/util/InputOutputPoints.h>
#include <exception>

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
public:
	std::string inFileName;
	std::string outFileName;
	int precision;
	int significands;
	int snapType;
	bool normalize;
	bool verbose;
	bool progress;
	bool rationalPassThrough;
	InputPoint::Format inFormat;
	OutputPoint::Format outFormat;
public:
	BasicCmdLineOptions();
	virtual ~BasicCmdLineOptions();
public:
	int parse(int argc, char ** argv);
public:
	//override this to parse extra options, return true if token was consumed
	virtual bool parse(const std::string & currentToken,int & i, int argc, char ** argv);
public:
	void options_help(std::ostream & out) const;
public:
	void options_selection(std::ostream & out) const;
};

}//end namespace LIB_RATSS_NAMESPACE

#endif
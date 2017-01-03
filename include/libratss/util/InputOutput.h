#ifndef LIB_RATSS_UTIL_INPUT_OUTPUT_H
#define LIB_RATSS_UTIL_INPUT_OUTPUT_H
#pragma once

#include <libratss/constants.h>

#include <istream>
#include <ostream>
#include <fstream>

namespace LIB_RATSS_NAMESPACE {

class InputOutput {
public:
	std::istream & input();
	std::ostream & output();
	std::ostream & info();
	void setInput(const std::string & inFileName);
	void setOutput(const std::string & outFileName);
private:
	std::istream * inFile = 0;
	std::ostream * outFile = 0;
	std::ostream * infoOut = 0;
	
	std::ifstream inFileHandle;
	std::ofstream outFileHandle;
	
};

}//end namespace
#endif
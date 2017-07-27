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
	InputOutput();
	//input, info have to be valid during the lifetime of this instance
	explicit InputOutput(std::istream & input, std::ostream & info);
	//input, info, output have to be valid during the lifetime of this instance
	explicit InputOutput(std::istream & input, std::ostream & info, std::ostream & output);
	~InputOutput();
public:
	std::istream & input();
	std::ostream & output();
	std::ostream & info();
	void setInput(const std::string & inFileName, std::ios_base::openmode mode = std::ios_base::in);
	void setOutput(const std::string & outFileName, std::ios_base::openmode mode = std::ios_base::out);
private:
	std::istream * inFile = 0;
	std::ostream * outFile = 0;
	std::ostream * infoOut = 0;
	
	std::ifstream inFileHandle;
	std::ofstream outFileHandle;
	
};

}//end namespace
#endif
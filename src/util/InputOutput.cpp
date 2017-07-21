#include <libratss/util/InputOutput.h>
#include <assert.h>
#include <stdexcept>
#include <iostream>

namespace LIB_RATSS_NAMESPACE {

InputOutput::InputOutput() :
inFile(&std::cin),
outFile(&std::cout),
infoOut(&std::cerr)
{}

InputOutput::InputOutput(std::istream& input, std::ostream& info) :
inFile(&input),
outFile(&info),
infoOut(&info)
{}

InputOutput::InputOutput(std::istream& input, std::ostream& info, std::ostream& output) :
inFile(&input),
outFile(&info),
infoOut(&output)
{}

std::ostream& InputOutput::info() {
	return *infoOut;
}

std::istream & InputOutput::input() {
	return *inFile;
}

std::ostream & InputOutput::output() {
	return *outFile;
}

void InputOutput::setInput(const std::string & inFileName) {
	assert(!inFile);
	if (inFileName.size()) {
		inFileHandle.open(inFileName);
		if (!inFileHandle.is_open()) {
			throw std::runtime_error("Could not open input file: " + inFileName);
		}
		inFile = &inFileHandle;
	}
	else {
		inFile = &std::cin;
	}
}

void InputOutput::setOutput(const std::string & outFileName) {
	assert(!outFile);
	if (outFileName.size()) {
		outFileHandle.open(outFileName);
		if (!outFileHandle.is_open()) {
			throw std::runtime_error("Could not open output file: " + outFileName);
		}
		outFile = &outFileHandle;
		infoOut = &std::cout;
	}
	else {
		outFile = &std::cout;
		infoOut = &std::cerr;
	}
}

}//end namespace
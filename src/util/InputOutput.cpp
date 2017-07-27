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

InputOutput::~InputOutput() {
	if (outFileHandle.is_open()) {
		outFileHandle.flush();
		outFileHandle.close();
	}
	if (inFileHandle.is_open()) {
		inFileHandle.close();
	}
}


std::ostream& InputOutput::info() {
	return *infoOut;
}

std::istream & InputOutput::input() {
	return *inFile;
}

std::ostream & InputOutput::output() {
	return *outFile;
}

void InputOutput::setInput(const std::string & inFileName, std::ios_base::openmode mode) {
	if (inFileName.size()) {
		if (inFileHandle.is_open()) {
			inFileHandle.close();
		}
		inFileHandle.open(inFileName, mode);
		if (!inFileHandle.is_open()) {
			throw std::runtime_error("Could not open input file: " + inFileName);
		}
		inFile = &inFileHandle;
	}
	else {
		inFile = &std::cin;
	}
}

void InputOutput::setOutput(const std::string & outFileName, std::ios_base::openmode mode) {
	if (outFileName.size()) {
		if (outFileHandle.is_open()) {
			outFileHandle.flush();
			outFileHandle.close();
		}
		outFileHandle.open(outFileName, mode);
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
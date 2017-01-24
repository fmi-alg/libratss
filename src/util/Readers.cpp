#include <libratss/util/Readers.h>

namespace LIB_RATSS_NAMESPACE {

FileReader::FileReader(const BasicCmdLineOptions& cfg, InputOutput& io) :
m_cfg(cfg),
m_io(io)
{}

FileReader::~FileReader() {}

} //end namespace LIB_RATSS_NAMESPACE
#include "DSVWriter.h"

CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall = false);

CDSVWriter::~CDSVWriter() {}

bool WriteRow(const std::vector< std::string > &row);
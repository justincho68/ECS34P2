#include "DSVReader.h"
#include <vector>
#include <sstream>
#include <queue>


struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    char Delimiter;
    std::string Buffer;
    std::queue<std::vector<std::string>> Rows;

    SImplementation(std::shared_ptr<CDataSource> src) : DataSource(src), Delimiter(Delimiter) {
    }

    bool End() const {
        return DataSource->End();
    }

    bool readLine
    
    bool ReadRow(std::vector< std::string > &row) {
        if (End()) {
            return false;
        }
        std::string line;
        if(!DataSource->Read(line)) {
            return false;
        }
    }

};
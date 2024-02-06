DSVReader Class

Purpose is to read data from a DSV file.

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    char Delimiter;
    std::string Buffer;
    std::queue<std::vector<std::string>> Rows;

    SImplementation(std::shared_ptr< CDataSource > src, char delimiter) {
        DataSource = src;
        Delimiter = delimiter;
    }

    bool End() const {
        return DataSource->End();
    }


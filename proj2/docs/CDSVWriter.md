DSVWriter Class

Purpose is to write data into a DSV file.

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    char Delimiter;
    bool Quoteall;
    SImplementation(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall) {
        Sink = sink;
        Delimiter = delimiter;
        Quoteall = quoteall;
    }

    bool WriteRow(const std::vector<std::string> &row) {
        std::string line;
        //Loop through every value in row to check for quoteall
        for(const auto &field : row) {
            if (Quoteall) {
                line += '"' + field + '"' + Delimiter;
            }
            else {
                line += field + Delimiter;
            }
        }
        if (!line.empty()) {
            line.pop_back();
        }
        std::vector<char> buffer(line.begin(), line.end());
        return Sink->Write(buffer);
    }
};

Firstly, an SImplementation struct is created with a specified data sink,
delimiter, and boolean quoteall variable. The method WriteRow is the main
way that this class will write to a DSV file. The function creates a row of 
data represented through a string vector. Each item in the vector is checked 
for the quoteall variable, if it is true, then the word will have quotes around it.
Finally, the last item in the vector is removed because it is a delimiter value.
The string vector is converted to a char vector and written to the data sink.

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink> sink, char delimiter, bool quoteall) {
    DImplementation = std::make_unique<SImplementation>(sink, delimiter, quoteall);
}

CDSVWriter::~CDSVWriter() {

}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    return DImplementation->WriteRow(row);
}

Public constructors are written to initalize DImplementation unique pointer
as a new instance of SImplementation. The destructor is used to free all
used memory. The final function calls WriteRow on the newly created
DImplementation instance.

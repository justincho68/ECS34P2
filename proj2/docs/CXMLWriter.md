CXMLWriter Class 

Purpose is to write to an XML file.

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DataSink;
    std::queue<SXMLEntity> ItemQueue;

    SImplementation(std::shared_ptr<CDataSink> sink) {
        DataSink = sink;

    }

Initializes the SImplementation struct. This is the private implementation of the 
CXMLWriter class using the PIMPL idiom. The private SImplementation declares the DataSink
that output will be written to.

    bool WriteEntity(const SXMLEntity &entity) {
        std::string output;
        if(entity.DType == SXMLEntity::EType::StartElement) {
            output += "<" + entity.DNameData;
            for (const auto &attr : entity.DAttributes) {
                output += " " + attr.first + "=\"" + attr.second + "\"";
            }
            output += ">";
        }
        else if (entity.DType == SXMLEntity::EType::EndElement) {
            output += "</" + entity.DNameData + ">";
        }
        else if (entity.DType == SXMLEntity::EType::CharData) {
            output += entity.DNameData;
        }
        std::vector<char> data(output.begin(),output.end());
        DataSink->Write(data);
        return true;
    }
};

This is the main function for writing data to the output XML file. This checks from
the input whether the character is a start element, end element, or character data.
If it is a start or end element, it adds the appropriate brackets. If it is just 
character data, it writes the data in between the start and end tags.

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) {
    DImplementation = std::make_unique<SImplementation>(std::move(sink));
}

CXMLWriter::~CXMLWriter() {
    Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    DImplementation->ItemQueue.push(entity);
    return true;
}

bool CXMLWriter::Flush() {
    while (!DImplementation->ItemQueue.empty()) {
        const auto &entity = DImplementation->ItemQueue.front();
        DImplementation->WriteEntity(entity);
        DImplementation->ItemQueue.pop();
    }
    return true;
}

A public constructor of CXMLWriter is created. It initalizes DImplementation and passes
the sink to it. This allows the CXMLWriter class to keep its implementation details private.
The destructor calls FLush to ensure that all queued elements are written into the data 
sink before it is destroyed and the memory accesses are gone. This is because the flush 
function will close opened tags. CXMLWriter::WriteEntity queues data for writing. Finally,
Flush function is to write out all data that is already queued. It returns true if the flush
is successful.
#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include <stdio.h>

struct CXMLReader::SImplementation {
    std::shared_ptr< CDataSource > DDataSource;
    XML_Parser DXMLParser;
    std::queue<SXMLEntity> DEntityQueue;

    SImplementation(std::shared_ptr< CDataSource > src) {
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser, StartElementHandlerCallback);
        XML_SetEndElementHandler(DXMLParser, EndElementHandlerCallback);
        XML_SetCharacterDataHandler(DXMLParser, CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser, this);
    };

    ~SImplementation()
    {
        XML_ParserFree(DXMLParser);
    }

    bool End() const {
        return DDataSource->End() && DEntityQueue.empty();
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata) {
        std::vector<char> DataBuffer(1024);
        while (DEntityQueue.empty() && !DDataSource->End()) {
            size_t numRead = DDataSource->Read(DataBuffer, DataBuffer.size());
            if (numRead == 0 && DDataSource->End()) {
                // Indicate end of data to parser if no more data is read and data source reports end
                XML_Parse(DXMLParser, nullptr, 0, XML_TRUE);
                break; // Break out of loop if no more data is available
            }
            
            XML_Parse(DXMLParser, DataBuffer.data(), static_cast<int>(numRead), XML_FALSE);

            // Skip character data if necessary
            while (skipcdata && !DEntityQueue.empty() && DEntityQueue.front().DType == SXMLEntity::EType::CharData) {
                DEntityQueue.pop();
            }
        }

        // After processing, check if an entity is available for return
        if (!DEntityQueue.empty()) {
            entity = DEntityQueue.front();
            DEntityQueue.pop();
            return true; // Successfully read an entity
        }
        return false; // No entities available to read
    }


    void StartElementHandler(const std::string &name, const std::vector<std::string> &attrs) {
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::StartElement;
        for (size_t Index = 0; Index < attrs.size(); Index += 2) {
            TempEntity.SetAttribute(attrs[Index], attrs[Index+1]);
        }
        DEntityQueue.push(TempEntity);
    };

    void EndElementHandler(const std::string &name) {
        SXMLEntity temp;
        temp.DNameData = name;
        temp.DType = SXMLEntity::EType::EndElement;
        DEntityQueue.push(temp);
    }

    void CharacterDataHandler(const std::string &data) {
        if (!data.empty()) {
            SXMLEntity temp;
            temp.DType = SXMLEntity::EType::CharData;
            temp.DNameData = data;
            DEntityQueue.push(temp);
        }
    }

    static void StartElementHandlerCallback(void *context, const XML_Char *name, const XML_Char **atts) {
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        std::vector<std::string> Attributes;
        auto AttrPtr = atts;
        while (*AttrPtr) {
            Attributes.push_back(*AttrPtr);
            AttrPtr++;
        }
        ReaderObject->StartElementHandler(name,Attributes);
    };

    static void EndElementHandlerCallback(void *context, const XML_Char *name) {
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        ReaderObject->EndElementHandler(name);
    };

    static void CharacterDataHandlerCallback(void *context, const XML_Char *s, int len) {
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        ReaderObject->CharacterDataHandler(std::string(s,len));
    };

};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src)
{
    DImplementation = std::make_unique<SImplementation>(src);
}

CXMLReader::~CXMLReader()
{}

bool CXMLReader::End() const 
{
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata)
{
    return DImplementation->ReadEntity(entity, skipcdata);
}

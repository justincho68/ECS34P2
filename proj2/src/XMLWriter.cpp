#include "XMLWriter.h"
#include <expat.h>
#include <queue>

// Constructor for XML writer, sink specifies the data destination
struct CXMLWriter::SImplementation
{
    std::shared_ptr< CDataSink > sink;
    XML_Parser DXMLParser;
    std::queue<SXMLEntity> DEntityQueue;

    SImplementation(std::shared_ptr< CDataSink > src) {
        sink = src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser, StartElementHandlerCallback);
        XML_SetEndElementHandler(DXMLParser, EndElementHandlerCallback);
        XML_SetCharacterDataHandler(DXMLParser, CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser, this);
    };
    // Destructor for XML writer
    ~CXMLWriter()
    {

    };

    // Outputs all end elements for those that have been started
    bool Flush()
    {

    };

    // Writes out the entity to the output stream
    bool WriteEntity(const SXMLEntity &entity)
    {
        std::vector<char> data;
        while (!entity.IsEmpty())
        {

        }
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
            temp.Dtype = SXMLEntity::EType::CharData;
            temp.DNameData = data;
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

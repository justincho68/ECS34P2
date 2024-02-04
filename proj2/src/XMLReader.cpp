#include "XMLReader.h"
#include <expat.h>

struct CXMLReader::SImplementation {
    std::shared_ptr< CDataSource > DDataSource;
    XML_Parser DXMLParser;

    SImplementation(std::shared_ptr< DataSource > src) {
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
    };

    bool End() const {

    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata) {

    };

    void StartElementHandler(const std::string &name, const std::vector<std::string> &atts) {

    }

    void EndElementHandler(const std::string &name) {

    }

    void CharacterDataHandler(const std::string &data) {

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

    static void CharacterDataHandlerCallback

    SImplementation(std::shared_ptr< CDataSource > src) {
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser, StartElementHandlerCallback);
        XML_SetEndElementHandler(DXMLParser, EndElementHandlerCallback);
        XML_SetCharacterDataHandler(DXMLParser, CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser, this);
    };

};
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

    bool End() const {
        return DDataSource->End() && DEntityQueue.empty();
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata) {
        //Reader fromm source if necessary
        //Pass to XML_Parse function
        //Return Entity
        while (DEntityQueue.empty() && !DDataSource->End()) {
            std::vector<char> DataBuffer(256);
            size_t numRead = DDataSource->Read(DataBuffer, 256);
            if (numRead > 0) {
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 256);
            }
            else {
                XML_Parse(DXMLParser,DataBuffer.data(),0,true);
            }
            while(skipcdata && !DEntityQueue.empty() && DEntityQueue.front().DType == SXMLEntity::EType::CharData) {
                DEntityQueue.pop(); // Remove the item to skip the char data
                if (DEntityQueue.empty()) {
                    numRead = DDataSource->Read(DataBuffer);
                    if(numRead>0) {

                    }
            }
        }

        }
        //Check for skipcdata
        if(DEntityQueue.empty()) {
            return false;
        }
        entity = DEntityQueue.front();
        DEntityQueue.pop();
        return true;
    };

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



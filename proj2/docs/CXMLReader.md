CXML Reader Class
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

Constructor creates an XML Reader instance and a Data source that it will read from. 
A queue is used to process the data because the data will come in a FIFO manner and
should be tracked as such. It then creates an instance of SImplementation and declares 
callback functions that Expat will use when it sees tags. The Data handler method is 
also used by Expat when it encounters CData.

    bool End() const {
        return DDataSource->End() && DEntityQueue.empty();
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata) {
        //Reader fromm source if necessary
        //Pass to XML_Parse function
        //Return Entity
        std::vector<char> DataBuffer;
        while (DEntityQueue.empty()) {
            if (DDataSource->Read(DataBuffer,256)) {
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 256);
            }
            else {
                XML_Parse(DXMLParser,DataBuffer.data(),0,true);
            }
        }
        if(DEntityQueue.empty()) {
            return false;
        }
        entity = DEntityQueue.front();
        DEntityQueue.pop();
        return true;
    };

Function End is used to check if the parsing of data is complete. It will return true when 
there is no more data to read and the queue of data is empty. ReadEntity function is for
reading the individual entities that show up in an XML file. It reads the individual start, 
end, and possible Cdata of each element.

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

    These are the callback functions. Start and End element handler are used when the 
    reader encounters a start or end tag. These functions will create an entity and 
    push it onto the queue. The character handler creates an entity for character data
    and adds it to the queue.

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

These static callback functions are what Expat uses. These functions cast a void pointer
and call the corresponding function to handle the event. It is used so that the Expat 
library can properly respond to SImplementation.
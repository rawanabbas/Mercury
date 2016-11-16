/**
 * class Message
 * This class is meant to for packaging the packets sent from the server and to the client or vice versa to this Message
 * \note Not complete yet
 * Author: Group 3
 * Date: 6th November 2016
 *  Project One - CSCE 4411
 */
#include "message.hpp"

Message::Message() {
    _timestamp =  std::time(nullptr);
}

Message::Message(char * msg, MessageType type) {
    setMessage(msg);
    setMessageType(type);
    _timestamp =  std::time(nullptr);
}


char * Message::getMessage() {
    return &_msg[0];
}

void Message::setMessage(char *msg) {
    strcpy(_msg, msg);
    _size = sizeof(msg);
}

MessageType Message::getMessageType() {
    return _type;
}

void Message::setMessageType(MessageType type) {
    _type = type;
}

size_t Message::getMessageSize() {
    return _size;
}

char * Message::serialize() {  //Convert input to Base64 Data stream
 std::string encoding_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int mType;
    std::string serialized = "";
    int n = _msg.size();
      

	for (int i = 0; i < n;)
    {
        uint32_t octet_a = i < n ? (unsigned char)_msg[i++] : 0;
        uint32_t octet_b = i < n ? (unsigned char)_msg[i++] : 0;
        uint32_t octet_c = i < n ? (unsigned char)_msg[i++] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        serialized = serialized + encoding_table[(triple >> 3 * 6) & 0x3F];
        serialized = serialized + encoding_table[(triple >> 2 * 6) & 0x3F];
        serialized = serialized + encoding_table[(triple >> 1 * 6) & 0x3F];
        serialized = serialized + encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    
    if (n % 3 != 0)
    {
        if (n % 3 == 1) 
        {
            serialized[serialized.size() - 1] = serialized[serialized.size() - 2] = '=';
          
        }
        if (n % 3 == 2)
        {
             serialized[serialized.size() - 1] = '=';
        }
    }
  
	switch (_type)
	{
	case Request:
	mType = 0 ;
	break;
	case Reply:
	mType = 1;
	break;
	case Ping:
	mType = 2;
	break;
	case Pong:
	mType = 3;
	break;
	case Info:
	mType = 4;
	break;
	default:
	mType = -1;
	}
    serialized = "S: " + _size + " M: " + serialized + " MT: " + mType + " T: " + _timestamp;
 // std::cout << "Serialized Message: " << serialized << std::endl;
    return serialized.c_str();
}
}

void Message::deserialize(char * serialized)
{
    int mType;
   // std::string encodingTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   // std::string decodingTable;
   //std::string unserialized = "";
   // decodingTable.resize(256);
   // std::string msgTemp;
    std::stringstream ss(serialized);
    std::string tokens;
    
    while (ss >> tokens)
    {
        if (tokens == "S:")
            ss >> _size;
        
        if (tokens == "M:")
            ss >> _msg;
  
        if (tokens == "MT:")
         {  	
		 ss >> mType;
		switch (mType)	
		{
		 case 0 :
		_type = Request;
		break;
		case 1:
		_type = Reply;
		break;
		case 2:
		_type = Ping;
		break;
		case 3:
		_type = Pong;
		break;
		case 4:
		_type = Info;
		break;
		default:
		_type = Undefined;
		
		}
         }
         if (tokens == "T:")
            ss >> _timestamp;
    }
    
  /*  for (int i = 0 ; i <  64; i++)
        decodingTable[(unsigned char)encodingTable[i]] = i;
      
    
    int j = 0 ;
    for (int i = 0 ; i < msgTemp.size();)
    {
        uint32_t a = msgTemp[i] == '=' ? 0 & i++ : decodingTable[i++];
        uint32_t b = msgTemp[i] == '=' ? 0 & i++ : decodingTable[i++];
        uint32_t c = msgTemp[i] == '=' ? 0 & i++ : decodingTable[i++];
        uint32_t d = msgTemp[i] == '=' ? 0 & i++ : decodingTable[i++];
        
        
        uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);
       
        unserialized = unserialized + (triple>> 2* 8)& 0xFF;
        unserialized = unserialized + (triple>> 1* 8)& 0xFF;
        unserialized = unserialized + (triple>> 0* 8)& 0xFF;  
        
        
    }
    _msg = unserialized;*/
}

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
    std:: string msgTemp;
    std::stringstream ss(serialized);
    std::string tokens;
    
    while (ss >> tokens)
    {
        if (tokens == "S:")
            ss >> _size;
        
        if (tokens == "M:")
            ss >> msgTemp;
  
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
	//Base64 to string
    	std::string unserialized = "";
	std::string encodingTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::vector <int> temp;
	std::string bin = "";
	for (int i = 0 ; i < msgTemp.size(); i++)
	{
		if (msgTemp[i] != '=')
			temp.push_back(encodingTable.find(msgTemp[i]));
		else
			temp.push_back (0);
	}

	for (int i = 0; i < temp.size(); i++)
	{
		std::string tempString= "";
		int digit = temp[i];
		int counter = 0;

		while (digit != 0)
		{
			
			tempString += to_string(digit & 1);
			digit = digit >> 1;
			counter++;
		}
		while (counter < 6)
		{
			tempString += '0';
			counter++;
		}
		counter = 0;
		for (int j = 0; j < tempString.size() / 2; j++)
	{
		swap(tempString[j], tempString[tempString.size() - 1 - j]);
	}
		bin += tempString;	
	}

	if (msgTemp[msgTemp.size() -1] == '=')
	{
		bin.erase (bin.size() -2 , 2);
		if (msgTemp[msgTemp.size() - 2] == '=')
			bin.erase (bin.size() -2 , 2);
	}

	
	cout << bin << endl;
	string octet = "";
	int counter = 0 ;
	for (int i = 0 ; i < bin.size(); i++)
	{
		octet += bin[i];
		counter++;
		if (counter == 8)
		{
			counter = 0;
			int ascii = 0;
			for (int j = octet.size() - 1; j >= 0; j--)
			{
				ascii = ascii | ((int(octet[j]) - 48) << counter);
				counter++;
			}
			octet = "";
			cout << ascii << endl;
			if (ascii != 0)
				unserialized += char(ascii);
			counter = 0;
		}

	}
  
    _msg = unserialized;
}

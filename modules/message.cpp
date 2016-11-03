#include "message.hpp"

Message::Message() {
    _timestamp =  std::time(nullptr);;
}

Message::Message(char * msg, MessageType type) {
    setMessage(msg);
    setMessageType(type);
    _timestamp =  std::time(nullptr);;
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

char * Message::serialize() {
    std::string serialized;
    serialized = "S: " + _size + " M: " + std::string(_msg) + " MT: " + _type + " T: " + _timestamp;
    std::cout << "Serialized Message: " << serialized << std::endl;
    return serialized.c_str();
}

Message Message::deserialize(char * serialized) {
    
}

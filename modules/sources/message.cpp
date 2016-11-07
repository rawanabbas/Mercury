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

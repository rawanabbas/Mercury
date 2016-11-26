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

Message::Message(const char *msg, MessageType type, RPC rpcId, ReplyType replyType) {
    setMessage(msg);
    setMessageType(type);
    setRpcId(rpcId);
    setReplyType(replyType);
    _timestamp =  std::time(nullptr);;
}


char * Message::getMessage() {
    return &_msg[0];
}

void Message::setMessage(const char *msg) {
    strcpy(_msg, msg);
    _size = strlen(msg);
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

std::string Message::serialize() {
    std::string serialized;
    serialized = "S: " + std::to_string(_size) + " MT: " + std::to_string((int)_type) + " R: " + std::to_string ((int)_rpcId) + " RT: " + std::to_string((int)_replyType) + " M: " + std::string(_msg);
    return serialized;
}

Message Message::deserialize(std::string serialized) {
    std::cout << "MESSAGE: deserialize(): " << serialized << std::endl;
    int size, type, rpcId = 1, replyType = 2;
    char msg [MAX_RECV];
    sscanf(serialized.c_str(), "S: %d MT: %d R: %d RT: %d M: %1024c", &size, &type, &rpcId, &replyType, msg);
    std::cout << "Deserialized Message: " << msg << std::endl;
    return Message(msg, (MessageType) type, (RPC) rpcId, (ReplyType) replyType);
}

RPC Message::getRpcId() const {
    return _rpcId;
}

void Message::setRpcId(const RPC &rpcId) {
    _rpcId = rpcId;
}


ReplyType Message::getReplyType() const {
    return _replyType;
}

void Message::setReplyType(const ReplyType &replyType) {
    _replyType = replyType;
}

Message::~Message() {

}

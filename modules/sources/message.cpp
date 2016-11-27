/**
* class Message
* This class is meant to for packaging the packets sent from the server and to the client or vice versa to this Message
* Author: Group 3
* Date: 6th November 2016
*  Project One - CSCE 4411
*/
#include "message.hpp"

//Static Definitions
const std::string Message::SizeToken = "S:";
const std::string Message::MessageTypeToken = "MT:";
const std::string Message::MessageToken = "M:";
const std::string Message::TimestampToken = "T:";
const std::string Message::ReplyTypeToken = "RT:";
const std::string Message::RPCToken = "R:";
const std::string Message::UserIdToken = "UID:";

Message::Message() {
    _timestamp =  std::time(nullptr);
}

Message::Message(std::string msg, MessageType type, RPC rpcId, ReplyType replyType) {
    setMessage(msg);
    setMessageType(type);
    setRpcId(rpcId);
    setReplyType(replyType);
    _timestamp =  std::time(nullptr);
}

Message::Message(std::string msg, MessageType type, RPC rpcId, ReplyType replyType, time_t timestamp) {
    setMessage(msg);
    setMessageType(type);
    setRpcId(rpcId);
    setReplyType(replyType);
    _timestamp =  timestamp;
}


std::string Message::getMessage() {
    return _msg;
}

void Message::setMessage(std::string msg) {
    _msg = msg;
    _size = msg.length();
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
    /*Rawan's Part
     std::string serialized;
     serialized = "S: " + std::to_string(_size) + " MT: " + std::to_string((int)_type) + " R: " + std::to_string ((int)_rpcId) + " RT: " + std::to_string((int)_replyType) + " M: " + std::string(_msg);
     return serialized; */

    std::string serialized = "";
    Decoder::encode(_msg, &serialized);
    serialized = SizeToken + " " + std::to_string(_size) + " " + MessageToken + " " + serialized + " " + MessageTypeToken + " "
                 + std::to_string((int)_type) + " " + TimestampToken + " " + std::to_string(_timestamp) + " " + RPCToken + " " +
                 std::to_string((int)_rpcId) + " " + ReplyTypeToken + " " + std::to_string((int)_replyType);
    std::cout << "Serialized Message: " << serialized <<std::endl;
    return serialized;
}

void Message::_parseMessage(std::string serialized, ReplyType &reply, RPC &rpc, MessageType &type, int &size, time_t &timestamp, std::string &encodedMsg) {
    std::stringstream ss(serialized);
    std::string token;

    while (ss >> token) {
        if (token == SizeToken) {
            ss >> size;
        } else if (token == MessageToken) {
            ss >> encodedMsg;
        } else if (token == MessageTypeToken) {
            int msgType;
            ss >> msgType;
            type = (MessageType) msgType;
        } else if (token == TimestampToken) {
            ss >> timestamp;
        } else if (token == RPCToken) {
            int rpcId;
            ss >> rpcId;
            rpc = (RPC) rpcId;
        } else if (token == ReplyTypeToken) {
            int replyType;
            ss >> replyType;
            reply = (ReplyType)replyType;
        }
    }
}

std::string Message::_serializeHeaders() {

}

Message Message::deserialize(std::string serialized) {
    ReplyType reply;
    RPC rpc;
    MessageType type;
    int size;
    time_t timestamp;
    std::string encodedMsg;
    std::string unserialized("");
    std::cout <<"Serialize in desrialize: " << serialized << std::endl;
    _parseMessage(serialized, reply, rpc, type, size, timestamp, encodedMsg);
    Decoder::decode(encodedMsg, &unserialized);
    return Message(unserialized, type, rpc, reply, timestamp);
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

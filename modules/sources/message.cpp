/**
* class Message
* This class is meant to for packaging the packets sent from the server and to the client or vice versa to this Message
* Author: Group 3
* Date: 6th November 2016
*  Project One - CSCE 4411
*/
#include "message.hpp"

//Static Definitions
const std::string Message::SizeToken = "Size:";
const std::string Message::MessageTypeToken = "Message-Type:";
const std::string Message::MessageToken = "Message:";
const std::string Message::TimestampToken = "Timestamp:";
const std::string Message::ReplyTypeToken = "Reply-Type:";
const std::string Message::RPCToken = "RPC:";
const std::string Message::OwnerIdToken = "Owner-Id:";
const std::string Message::UsernameToken = "Username:";
const std::string Message::FileModeToken = "File-Mode:";
const std::string Message::FileDescriptorToken = "File-Descriptor:";
const std::string Message::FileNameToken = "File-Name:";
const std::string Message::DecodedLengthToken = "Decoded-Length:";

Message::Message() {

    _timestamp =  std::time(nullptr);
    addHeader(TimestampToken, std::to_string(_timestamp));

}

Message::Message(std::string ownerId, std::string username, std::string msg, MessageType type, RPC rpcId, ReplyType replyType) {

    setMessage(msg);
    setMessageType(type);
    setRpcId(rpcId);
    setReplyType(replyType);
    setOwnerId(ownerId);

    _username = username;
    _timestamp =  std::time(nullptr);

    addHeader(UsernameToken, _username);
    addHeader(TimestampToken, std::to_string(_timestamp));

}

Message::Message(std::string msg, HeadersMap headers) {

    setMessage(msg);
    _headers = headers;

    HeadersMap::iterator it;

    for (it = headers.begin(); it != headers.end(); ++it) {

        if (it->first == MessageTypeToken) {

            setMessageType((MessageType)std::stoi(it->second));

        } else if (it -> first == RPCToken) {

            setRpcId((RPC) std::stoi(it->second));

        } else if (it -> first == ReplyTypeToken) {

            setReplyType((ReplyType) std::stoi(it->second));

        } else if (it -> first == OwnerIdToken) {

            setOwnerId(it->second);

        } else if (it -> first == TimestampToken) {

            _timestamp = std::stol(it->second);

        } else if (it -> first == UsernameToken) {

            _username = it -> second;

        }

    }


}

Message::Message(std::string ownerId, std::string username, std::string msg, MessageType type, RPC rpcId, ReplyType replyType, time_t timestamp) {

    setMessage(msg);
    setMessageType(type);
    setRpcId(rpcId);
    setReplyType(replyType);
    setOwnerId(ownerId);

    _username = username;
    _timestamp =  timestamp;

    addHeader(UsernameToken, _username);
    addHeader(TimestampToken, std::to_string(_timestamp));
}


std::string Message::getMessage() {
    return _msg;
}

void Message::setMessage(std::string msg) {

    _msg = msg;
    _size = msg.length();
    addHeader(SizeToken, std::to_string(_size));

}

MessageType Message::getMessageType() {
    return _type;
}

void Message::setMessageType(MessageType type) {
    _type = type;
    addHeader(MessageTypeToken, std::to_string((int)_type));
}

size_t Message::getMessageSize() {
    return _size;
}

std::string Message::serialize() {

    std::string serialized = "";

    Decoder::encode(_msg, &serialized);
    addHeader(MessageToken, serialized);

    serialized = _serializeHeaders();

    return serialized;
}


std::string Message::getOwnerId() const {
    return _ownerId;
}

void Message::setOwnerId(const std::string &ownerId) {
    _ownerId = ownerId;
    addHeader(OwnerIdToken, _ownerId);
}

//void Message::_parseMessage(std::string serialized, std::string &ownerId, ReplyType &reply, RPC &rpc, MessageType &type, int &size, time_t &timestamp, std::string &encodedMsg) {
//    std::stringstream ss(serialized);
//    std::string token;

//    while (ss >> token) {

//        if (token == SizeToken) {

//            ss >> size;

//        } else if (token == MessageToken) {

//            ss >> encodedMsg;

//        } else if (token == MessageTypeToken) {

//            int msgType;
//            ss >> msgType;
//            type = (MessageType) msgType;

//        } else if (token == TimestampToken) {

//            ss >> timestamp;

//        } else if (token == RPCToken) {

//            int rpcId;
//            ss >> rpcId;
//            rpc = (RPC) rpcId;

//        } else if (token == ReplyTypeToken) {

//            int replyType;
//            ss >> replyType;
//            reply = (ReplyType)replyType;

//        } else if (token == OwnerIdToken) {

//            ss >> ownerId;

//        }
//    }
//}

void Message::_parseMessage(std::string serialized, std::string &encodedMsg, HeadersMap &headers) {

    std::stringstream ss(serialized);
    std::string token;

    while (ss >> token) {

        if (token == SizeToken) {

            ss >> headers[SizeToken];

        } else if (token == MessageToken) {

            ss >> encodedMsg;

        } else if (token == MessageTypeToken) {

            ss >> headers[MessageTypeToken];

        } else if (token == TimestampToken) {

            ss >> headers[TimestampToken];

        } else if (token == RPCToken) {

            ss >> headers[RPCToken];

        } else if (token == ReplyTypeToken) {

            ss >> headers[ReplyTypeToken];

        } else if (token == OwnerIdToken) {

            ss >> headers[OwnerIdToken];

        } else  if (token == FileModeToken) {

            ss >> headers[FileModeToken];

        } else if (token == FileDescriptorToken) {

            ss >> headers[FileDescriptorToken];

        } else if (token == FileNameToken) {

            ss >> headers[FileNameToken];

        } else if (token == DecodedLengthToken) {

            ss >> headers[DecodedLengthToken];

        } else if (token == UsernameToken) {

            ss >> headers[UsernameToken];

        } else {

            ss >> headers[token];

        }
    }
}

std::string Message::_serializeHeaders() {

    std::string serialized = "";

    std::map<std::string, std::string>::iterator it;


    for (it = _headers.begin(); it != _headers.end(); it++) {
        serialized += it -> first + " " + it -> second + " ";
    }

    return serialized;
}

Message Message::deserialize(std::string serialized) {

    std::string encodedMsg;
    std::string unserialized("");


    HeadersMap headers;

    _parseMessage(serialized, encodedMsg, headers);

    Decoder::decode(encodedMsg, &unserialized);
    return Message(unserialized, headers);
}

RPC Message::getRpcId() const {
    return _rpcId;
}

void Message::setRpcId(const RPC &rpcId) {
    _rpcId = rpcId;
    addHeader(RPCToken, std::to_string((int)_rpcId));
}


ReplyType Message::getReplyType() const {
    return _replyType;
}

void Message::setReplyType(const ReplyType &replyType) {
    _replyType = replyType;
    addHeader(ReplyTypeToken, std::to_string((int)_replyType));
}

void Message::addHeader(std::string key, std::string value) {
    _headers[key] = value;
}

void Message::editHeader(std::string key, std::string value) {
    _headers[key] = value;
}

std::string Message::getHeader(std::string key) {
    return _headers[key];
}

Message::~Message() {

}

/**
 * class Message
 * This class is meant to for packaging the packets sent from the server and to the client or vice versa to this Message
 * \note Not complete yet
 * Author: Group 3
 * Date: 6th November 2016
 *  Project One - CSCE 4411
 */
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "stdafx.h"
#include "decoder.hpp"

enum class MessageType {
    Request = 0,
    Reply = 1,
    Ping = 2,
    Pong = 3,
    Info = 4,
    Exit = 5,
    EstablishConnection = 6,
    Authenticate = 7,
    Authenticated = 8,
    Unauthorized = 9,
    Register = 10,
    Query = 11,
    QueryAll = 12,
    Result = 13,
    EndFile = 14,
    SendFile = 15,
    Undefined = -1
};

enum class RPC {
    CreateFile,
    ReadFile,
    WriteToFile,
    OpenFile,
    CloseFile,
    Undefined = -1
};

enum class ReplyType {
    Success,
    Failure,
    NoReply
};

class Message {
private:


    //Private Variables
    MessageType _type;
    ReplyType _replyType;
    size_t _size;
    RPC _rpcId;

    std::string _msg;
    std::string _timestamp;
    std::string _ownerId;
    std::string _username;

    HeadersMap _headers;

    //Private Functions
    static void _parseMessage(std::string serialized, std::string &encodedMsg, HeadersMap &headers);

    std::string _serializeHeaders();

public:

    //Tokens:
    static const std::string SizeToken;
    static const std::string MessageTypeToken;
    static const std::string MessageToken;
    static const std::string TimestampToken;
    static const std::string ReplyTypeToken;
    static const std::string RPCToken;
    static const std::string OwnerIdToken;
    static const std::string UsernameToken;
    static const std::string FileModeToken;
    static const std::string FileDescriptorToken;
    static const std::string FileNameToken;
    static const std::string DecodedLengthToken;
    static const std::string NumberOfMessagesToken;

    Message ();
    Message (std::string msg, HeadersMap headers);
    Message (std::string ownerId, std::string username, std::string msg, MessageType type, RPC rpcId, ReplyType replyType, std::string timestamp);
    Message (std::string ownerId, std::string username, std::string msg, MessageType type = MessageType::Undefined, RPC rpcId = RPC::Undefined, ReplyType replyType = ReplyType::NoReply);

    std::string getMessage();
    void setMessage(std::string);

    MessageType getMessageType();
    void setMessageType(MessageType type);

    size_t getMessageSize();

    std::string serialize();
    static Message deserialize(std::string serialized);

    RPC getRpcId() const;
    void setRpcId(const RPC &rpcId);

    ReplyType getReplyType() const;
    void setReplyType(const ReplyType &replyType);

    void addHeader(std::string key, std::string value);
    void editHeader(std::string key, std::string value);
    std::string getHeader(std::string key);

    virtual ~Message ();

    std::string getOwnerId() const;
    void setOwnerId(const std::string &ownerId);
};

#endif // MESSAGE_HPP

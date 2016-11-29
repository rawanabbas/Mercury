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

#include "stdfax.h"
#include "decoder.hpp"

enum class MessageType {
    Request,
    Reply,
    Ping,
    Pong,
    Info,
    Exit,
    EstablishConnection,
    Authenticate,
    Authenticated,
    Unauthorized,
    Register,
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
    //Tokens:
    static const std::string SizeToken;
    static const std::string MessageTypeToken;
    static const std::string MessageToken;
    static const std::string TimestampToken;
    static const std::string ReplyTypeToken;
    static const std::string RPCToken;
    static const std::string OwnerIdToken;

    //Private Variables
    MessageType _type;
    std::string _msg;
    std::time_t _timestamp;
    RPC _rpcId;
    ReplyType _replyType;
    size_t _size;
    std::string _ownerId;

    std::map<std::string, std::string> _headers;

    //Private Functions
    static void _parseMessage(std::string serialized, std::string &ownerId, ReplyType &reply,
                              RPC &rpc, MessageType &type, int &size, time_t &timestamp,
                              std::string &encodedMsg);

    std::string _serializeHeaders();

public:

    Message ();
    Message (std::string ownerId, std::string msg, MessageType type, RPC rpcId, ReplyType replyType, time_t timestamp);
    Message (std::string ownerId, std::string msg, MessageType type = MessageType::Undefined, RPC rpcId = RPC::Undefined, ReplyType replyType = ReplyType::NoReply);

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

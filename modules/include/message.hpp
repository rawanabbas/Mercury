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

enum class MessageType {
    Request,
    Reply,
    Ping,
    Pong,
    Info,
    Exit,
    Undefined = -1};

enum class RPC {
    CreateFile,
    ReadFile,
    WriteToFile,
    OpenFile,
    Undefined = -1
};

enum class ReplyType {
    Success,
    Failure,
    NoReply
};


class Message {
private:
    MessageType _type;
    char _msg[MAX_RECV];
    std::time_t _timestamp;
    RPC _rpcId;
    ReplyType _replyType;
    size_t _size;
public:
    Message ();
    Message (const char * msg, MessageType type = MessageType::Undefined, RPC rpcId = RPC::Undefined, ReplyType replyType = ReplyType::NoReply);

    char * getMessage();
    void setMessage(const char *msg);

    MessageType getMessageType();
    void setMessageType(MessageType type);

    size_t getMessageSize();

    std::string serialize();
    static Message deserialize(std::string serialized);

    RPC getRpcId() const;
    void setRpcId(const RPC &rpcId);

    ReplyType getReplyType() const;
    void setReplyType(const ReplyType &replyType);

    virtual ~Message ();
    int getTest() const;
    void setTest(int value);
};

#endif // MESSAGE_HPP

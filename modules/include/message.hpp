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

enum MessageType {Request, Reply, Ping, Pong, Info, Undefined = -1};

class Message {
private:
    MessageType _type;
    char _msg[MAX_RECV];
    std::time_t _timestamp;
    size_t _size;
public:
    Message ();
    Message (char * msg, MessageType type = Undefined);

    char * getMessage();
    void setMessage(char * msg);

    MessageType getMessageType();
    void setMessageType(MessageType type);

    size_t getMessageSize();

    char * serialize();
    Message deserialize(char * serialized);

    virtual ~Message ();
};

#endif // MESSAGE_HPP

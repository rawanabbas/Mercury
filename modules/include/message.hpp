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

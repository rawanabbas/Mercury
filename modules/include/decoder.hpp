#ifndef DECODER_HPP
#define DECODER_HPP
#include "stdafx.h"
class Decoder {
    static const char EncodingTable[];
    static inline void _a3_to_a4(unsigned char * a4, unsigned char * a3);
    static inline void _a4_to_a3(unsigned char * a3, unsigned char * a4);
    static inline unsigned char _base64Lookup(unsigned char c);

public:
    Decoder();
    static bool encode(const std::string &in, std::string *out);
    static bool encode(const char *input, size_t input_length, char *out, size_t out_length);
    static bool decode(const std::string &in, std::string *out);
    static bool decode(const char *input, size_t input_length, char *out, size_t out_length);
    static int decodedLength(const char *in, size_t in_length);
    static int decodedLength(const std::string &in);
    inline static int encodedLength(size_t length);
    inline static int encodedLength(const std::string &in);
    inline static void stripPadding(std::string *in);
    virtual ~Decoder();
};

#endif // DECODER_HPP

#ifndef STEGANOGRAPHY_HPP
#define STEGANOGRAPHY_HPP

#include "stdafx.h"
#include "file.hpp"

class Steganography
{
    static const std::string SteganographyCommand;

    static bool _embedData(std::string file, std::string data, std::string secret);
    static bool _extractData(std::string src, std::string destination, std::string secret);

public:
    Steganography();


    static bool embedImage(std::string original, std::string secretImage, std::string data, std::string &destination, std::string secret);
    static bool extractImage(std::string src, std::string &destination, std::string secret);

    static bool incrementViews(std::string image, std::string secret);
    static bool decrementViews(std::string image, std::string secret);

    virtual ~Steganography();
};

#endif // STEGANOGRAPHY_HPP

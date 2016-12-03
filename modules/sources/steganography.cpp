#include "steganography.hpp"

const std::string Steganography::SteganographyCommand = "steghide";

Steganography::Steganography() {

}

bool Steganography::_embedData(std::string file, std::string data, std::string secret) {

    std::string command = SteganographyCommand + " embed -ef " + data + " -cf " + file + " -p " + secret;

    if (system(command.c_str()) != -1) {

        std::cout << "Data is now hidded!" << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, data is not hidden!" << std::endl;
        return false;

    }


}

bool Steganography::_extractData(std::string src, std::string destination, std::string secret) {

    std::string command = SteganographyCommand + " extract -sf " + src + " -xf " + destination + " -p " + secret;

    if (system(command.c_str()) != -1) {

        std::cout << "The data is now available at " << destination << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, data could not be extracted!" << std::endl;
        return false;

    }

}


bool Steganography::embedImage(std::string original, std::string secretImage, std::string data, std::string destination, std::string secret) {


    if(_embedData(original, data, secret)) {

        std::string command = SteganographyCommand + " embed -ef " + secretImage + " -cf " + original + " -p " + secret + " -sf " + destination;

        if (system(command.c_str()) != -1) {

            std::cout << "The data is now available at " << destination << std::endl;
            return true;

        } else {

            std::cerr << "An error has occured, data could not be extracted!" << std::endl;
            return false;

        }
    } else {

        return false;

    }

}

bool Steganography::extractImage(std::string src, std::string destination, std::string secret) {

    std::string command = SteganographyCommand + " extract -sf " + src + " -xf " + destination + " -p " + secret;

    if(system(command.c_str()) != -1) {

        std::cout << "The data is now available at " << destination << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, image couldn't be extracted!" << std::endl;
        return false;

    }


}

bool Steganography::incrementViews(std::string image, std::string userId, std::string secret) {

    //TODO

    if (extractImage(image, "secretImage.jpg", secret)) {

        if (_extractData("secretImage.jpg", "views.txt", secret)) {

            std::ifstream viewsFile("views.txt");
            
            if (viewsFile.is_open()) {

                std::string user;
                int views;
                std::streampos pos;
                //TO-DO: KEFAYA EL RAQAM.
                while (viewsFile >> user) {

                    if (user == userId) {

                        pos = viewsFile.tellg();
                        viewsFile >> views;
                        break;

                    }
                }

                views++;

                viewsFile.close();

                std::ofstream updatedViewsFile("views.txt");

                if (updatedViewsFile.is_open()) {

                    updatedViewsFile.seekp(pos);
                    updatedViewsFile.write(" " + std::to_string(views), (std::to_string(views) + "\n").length());
                    updatedViewsFile.close();

                }

            } else {

                std::cerr << "Cannot edit views!" << std::endl;

            }

        }

    } else {
        return false;
    }

}

bool Steganography::decrementViews(std::string image, std::string userId) {
    return true;
}


Steganography::~Steganography() {

}

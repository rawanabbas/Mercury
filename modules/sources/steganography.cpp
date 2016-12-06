#include "steganography.hpp"

const std::string Steganography::SteganographyCommand = "steghide";

Steganography::Steganography() {

}

bool Steganography::_embedData(std::string file, std::string data, std::string secret) {


    std::string dir = dirname(strdup(file.c_str()));
    std::string views = dir + "/views.txt";

    int fd = creat(views.c_str(), S_IRWXU);
    write(fd, data.c_str(), data.size());
    close(fd);

    std::string command = SteganographyCommand + " embed -ef " + views + " -cf " + file + " -p " + secret + " --force";
    std::cout << command << std::endl;


    if (system(command.c_str()) != -1) {
        unlink(views.c_str());
        std::cout << "Data is now hidded!" << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, data is not hidden!" << std::endl;
        return false;

    }


}

bool Steganography::_extractData(std::string src, std::string destination, std::string secret) {

    std::string command = SteganographyCommand + " extract -sf " + src + " -xf " + destination + " -p " + secret + " --force";

    if (system(command.c_str()) != -1) {

        std::cout << "The data is now available at " << destination << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, data could not be extracted!" << std::endl;
        return false;

    }

}


bool Steganography::embedImage(std::string original, std::string secretImage, std::string data, std::string destination, std::string secret) {


    if(_embedData(secretImage, data, secret)) {

        std::string command = SteganographyCommand + " embed -ef " + secretImage + " -cf " + original + " -p " + secret + " -sf " + destination + " --force";
        std::cout << command << std::endl;
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

    std::string command = SteganographyCommand + " extract -sf " + src + " -xf " + destination + " -p " + secret + " --force";
    std::cout << command << std::endl;


    if(system(command.c_str()) != -1) {

        std::cout << "The data is now available at " << destination << std::endl;
        return true;

    } else {

        std::cerr << "An error has occured, image couldn't be extracted!" << std::endl;
        return false;

    }


}

bool Steganography::incrementViews(std::string image, std::string secret) {

    //TODO

    if (extractImage(image, "temp.jpg", secret)) {

        if (_extractData("temp.jpg", "views.txt", secret)) {

            std::ifstream viewsFile("views.txt");

            if (viewsFile.is_open()) {

                std::string user;
                int views;
                std::streampos pos;
                //TO-DO: KEFAYA EL RAQAM.
                viewsFile >> views;
                views++;

                viewsFile.close();

                /*std::ofstream updatedViewsFile("views.txt");

                if (updatedViewsFile.is_open()) {
                    updatedViewsFile << views;
                }*/

               int rc  = embedImage(image, "temp.jpg", std::to_string(views), image, secret);
               unlink("views.txt");
               unlink("temp.jpg");
               return rc;
            } else {

                std::cerr << "Cannot edit views!" << std::endl;
                return false;

            }

        }

    } else {

        return false;

    }

}

bool Steganography::decrementViews(std::string image, std::string secret) {

    if (extractImage(image, "temp.jpg", secret)) {

        if (_extractData("temp.jpg", "views.txt", secret)) {

            std::ifstream viewsFile("views.txt");

            if (viewsFile.is_open()) {

                std::string user;
                int views;
                std::streampos pos;
                //TO-DO: KEFAYA EL RAQAM.
                viewsFile >> views;
                views--;

                viewsFile.close();

                /*std::ofstream updatedViewsFile("views.txt");

                if (updatedViewsFile.is_open()) {
                    updatedViewsFile << views;
                }*/

               int rc  = embedImage(image, "temp.jpg", std::to_string(views), image, secret);
               unlink("views.txt");
               unlink("temp.jpg");
               if (views == 0)
                   unlink(image.c_str());
               return rc;
            } else {

                std::cerr << "Cannot edit views!" << std::endl;
                return false;

            }

        }

    } else {

        return false;

    }
}


Steganography::~Steganography() {

}

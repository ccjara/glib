#include <iostream>
#include <fstream>
#include <Windows.h>

int main(int argc, char *argv[]) {
    unsigned int numberOfFiles = 0;
    unsigned int headerSize = 0;
    std::string archive;

    if (argc != 2) {
        std::cout << "Usage: Drag the GLIB file to extract onto the GLIB Extractor." << std::endl;
        system("PAUSE");
        return 0;
    }

    archive = argv[1];
    std::ifstream inputStream(archive, std::ios::binary);

    if (!inputStream) {
        std::cout << "Could not open input file " << argv[1] << std::endl;
        system("PAUSE");
        return 0;
    }

    inputStream.read(reinterpret_cast<char *> (&numberOfFiles), 4);
    std::cout << "Number of files: " << numberOfFiles << std::endl;

    if (numberOfFiles == 0) {
        std::cout << "No files to extract" << std::endl;
        system("PAUSE");
        return 0;
    }

    std::string dir = archive + "_Files";
    CreateDirectory(dir.c_str(), nullptr);
    std::cout << "Extracting to: " << dir.c_str() << std::endl;

    inputStream.ignore(22 + 2); // skip to header size bytes
    inputStream.read(reinterpret_cast<char *> (&headerSize), 4);

    for (unsigned int i = 0; i < numberOfFiles; i++) {
        char name[22];
        unsigned short size = 0;
        unsigned int endOfFile = 0;
        inputStream.read(name, 22);
        inputStream.read(reinterpret_cast<char *> (&size), 2);
        inputStream.read(reinterpret_cast<char *> (&endOfFile), 4);
        std::streamoff offset = inputStream.tellg();
        inputStream.seekg(endOfFile - size);

        std::string path = dir + "\\" + name;
        std::ofstream outputStream(path, std::ios::binary);
        char *data = nullptr;
        if (size != 0) {
            data = new char[size];
            inputStream.read(data, size);
            outputStream.write(data, size);
            delete[] data;
        }
        inputStream.seekg(offset);
        std::cout << "Exporting " << name << " (" << size << " bytes)" << std::endl;
    }

    std::cout << "Done." << std::endl;
    system("PAUSE");
    return 0;
}

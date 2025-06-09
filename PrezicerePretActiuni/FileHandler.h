#pragma once
#include <iostream>
#include <fstream>
#include <string>

class FileHandler {
public:
    
    void writeToFile(const std::string& fileName, const std::string& content) {
        std::ofstream file(fileName);
        if (!file) {
            throw std::runtime_error("Nu s-a putut deschide fisierul pentru scriere.");
        }
        file << content;
        file.close();
    }

    std::string readFromFile(const std::string& fileName) {
        std::ifstream file(fileName);
        if (!file) {
            throw std::runtime_error("Nu s-a putut deschide fisierul pentru citire.");
        }

        std::string content;
        std::getline(file, content, '\0');
        file.close();
        return content;
    }
};

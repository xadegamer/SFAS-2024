#pragma once

#include <map>
#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <utility>

class ObjectSerializer
{

private:
        static std::string encryptionKey;

public:

	ObjectSerializer() = delete;
	~ObjectSerializer() = delete;

    template <typename T>
    static void SaveToFile(const T& data, std::string filename, bool encrypt = true)
    {
        std::string completeFilename = "Resource/SavedData/" + filename + ".txt";
        std::ofstream file(completeFilename, std::ios::out | std::ios::binary);
        if (!file.is_open()) 
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
            std::string command = "mkdir " + completeFilename;
            system(command.c_str());
        }

        std::stringstream ss;
        ss << data; 
        std::string serializedData = ss.str();

        if (encrypt)
        XOREncryptDecrypt(serializedData, encryptionKey);

        file << serializedData; // Use stream operator to save the data
        file.close();
    }

    template <typename T>
    static bool LoadFromFile(T& data, std::string filename, bool encrypt = true)
    {

        std::string completeFilename = "Resource/SavedData/" + filename + ".txt";
        std::ifstream file(completeFilename, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open the file for reading." << std::endl;
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string encryptedData = buffer.str();

        if (encrypt)
        XOREncryptDecrypt(encryptedData, encryptionKey);

        buffer.str(encryptedData);
        buffer >> data;  // Load the decrypted data

        file.close();

        return true;
    }

    static void XOREncryptDecrypt(std::string& data, const std::string& key);
};
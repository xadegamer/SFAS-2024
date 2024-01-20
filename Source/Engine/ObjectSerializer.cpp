#include "ObjectSerializer.h"

std::string ObjectSerializer::encryptionKey = "Pelumi";


void ObjectSerializer::XOREncryptDecrypt(std::string& data, const std::string& key)
{
    for (size_t i = 0; i < data.size(); ++i) 
    {
        data[i] ^= key[i % key.size()];
    }
}

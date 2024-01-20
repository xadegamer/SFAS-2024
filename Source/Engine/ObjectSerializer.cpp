#include "ObjectSerializer.h"

std::string ObjectSerializer::encryptionKey = "Pelumi";

void ObjectSerializer::Example()
{
    MyTestSaveClass object_to_save;

    object_to_save.money = 42;
    object_to_save.gem = 100;
    object_to_save.characterID = "Knight";

    SaveToFile(object_to_save, "GameData");

    MyTestSaveClass loaded_object;
    if (LoadFromFile(loaded_object, "GameData"))
    {
        std::cout << "Loaded Data: " << loaded_object.money << " " << loaded_object.gem << " " << loaded_object.characterID << std::endl;
    }
    else 
    {
        std::cerr << "Failed to load GameData." << std::endl;
    }

    SaveToFile(1000, "IntData");

    int intData = 0;
    if (LoadFromFile(intData, "IntData"))
    {
        std::cout << "Loaded int: " << intData << std::endl;
    }
    else
    {
        std::cerr << "Failed to load IntData." << std::endl;
    }
}

void ObjectSerializer::XOREncryptDecrypt(std::string& data, const std::string& key)
{
    for (size_t i = 0; i < data.size(); ++i) 
    {
        data[i] ^= key[i % key.size()];
    }
}

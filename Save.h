#pragma once
#include <string>
#include <vector>
#include <list>
#include "Item.h"

class Save {
public:
    static void SaveItems(const std::vector<Item>& items, const std::string& filePath, char separator = ',');
    static void LoadItems(std::vector<Item>& items, const std::string& filePath);
    static void SaveSettings(const std::list<std::string>& settings);
    static std::list<std::string> LoadSettings();

private:
    static std::string EscapeForCSV(const std::string& field);
    static std::vector<std::string> ParseCSVLine(const std::string& line);
    static bool FileDoesNotExist(const std::string& filePath);
};

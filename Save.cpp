#define _HAS_STD_BYTE 0
#define _CRT_SECURE_NO_WARNINGS

#include "Save.h"
#include <fstream>
#include <filesystem>
#include <wx/wx.h>

std::string Save::EscapeForCSV(const std::string& field) {
    std::string escapedField = "\"";
    for (char c : field) {
        if (c == '"') {
            escapedField += "\"\"";
        }
        else {
            escapedField += c;
        }
    }
    escapedField += "\"";
    return escapedField;
}

void Save::SaveItems(const std::vector<Item>& items, const std::string& filePath, char separator) {
    std::error_code ec;
    if (!std::filesystem::exists("save")) {
        std::filesystem::create_directory("save", ec);
        if (ec) {
            wxMessageBox("Failed to create save directory", "Error", wxICON_ERROR);
            return;
        }
    }

    std::ofstream file(filePath);
    if (!file.is_open()) {
        wxMessageBox("Error: Could not open file for saving.", "Save Error", wxICON_ERROR);
        return;
    }

    for (const auto& item : items) {
        file << EscapeForCSV(item.getName()) << separator
            << EscapeForCSV(item.getDescription()) << separator
            << item.getAmount() << separator
            << EscapeForCSV(item.getImage()) << separator
            << EscapeForCSV(item.getCategory()) << "\n";
    }

    file.close();
    wxMessageBox("Items saved successfully!", "Save Complete", wxICON_INFORMATION);
}


void Save::SaveToCfg(const std::vector<Item>& items, const std::vector<std::string>& categories, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        wxMessageBox("Error: Could not open file for saving.", "Save Error", wxICON_ERROR);
        return;
    }

    // Save categories
    file << "[Categories]\n";
    for (const auto& category : categories) {
        file << category << "\n";
    }

    // Save items
    file << "[Items]\n";
    for (const auto& item : items) {
        file << EscapeForCSV(item.getName()) << ","
            << EscapeForCSV(item.getDescription()) << ","
            << item.getAmount() << ","
            << EscapeForCSV(item.getImage()) << ","
            << EscapeForCSV(item.getCategory()) << "\n";
    }

    file.close();
    wxMessageBox("Items and categories saved successfully!", "Save Complete", wxICON_INFORMATION);
}

void Save::LoadFromCfg(std::vector<Item>& items, std::vector<std::string>& categories, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        wxMessageBox("Error: Could not open file for loading.", "Load Error", wxICON_ERROR);
        return;
    }

    items.clear();
    categories.clear();

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line == "[Categories]") {
            currentSection = "Categories";
            continue;
        }
        else if (line == "[Items]") {
            currentSection = "Items";
            continue;
        }

        if (currentSection == "Categories") {
            categories.push_back(line);
        }
        else if (currentSection == "Items") {
            std::vector<std::string> fields = ParseCSVLine(line);
            if (fields.size() == 5) {
                try {
                    Item item(fields[0], fields[1], std::stoi(fields[2]), fields[3]);
                    item.setCategory(fields[4]);
                    items.push_back(item);
                }
                catch (const std::invalid_argument& e) {
                    wxMessageBox("Warning: Invalid number format in line. Skipping.", "Load Warning", wxICON_WARNING);
                }
            }
        }
    }

    file.close();
}


std::vector<std::string> Save::ParseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        }
        else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

void Save::LoadItems(std::vector<Item>& items, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        wxMessageBox("Error: Could not open file for loading.", "Load Error", wxICON_ERROR);
        return;
    }

    items.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> fields = ParseCSVLine(line);
        if (fields.size() == 5) {  // Updated to check for 5 fields
            try {
                Item item(fields[0], fields[1], std::stoi(fields[2]), fields[3]);
                item.setCategory(fields[4]);  // Set the category
                items.push_back(item);
            }
            catch (const std::invalid_argument& e) {
                wxMessageBox("Warning: Invalid number format in line. Skipping. Error: " + std::string(e.what()), "Load Warning", wxICON_WARNING);
            }
        }
    }
    file.close();
    wxMessageBox("Items loaded successfully!", "Load Complete", wxICON_INFORMATION);
}

bool Save::FileDoesNotExist(const std::string& filePath) {
    std::ifstream file(filePath);
    return !file.is_open();
}

void Save::SaveSettings(const std::list<std::string>& settings) {
    std::ofstream file("save/settings.cfg");
    if (!file.is_open()) {
        wxMessageBox("Error: Could not open settings file for saving.", "Settings Error", wxICON_ERROR);
        return;
    }

    for (const auto& setting : settings) {
        file << setting << std::endl;
    }
    file.close();
}

std::list<std::string> Save::LoadSettings() {
    std::list<std::string> settings;
    if (FileDoesNotExist("save/settings.cfg")) {
        SaveSettings(std::list<std::string>{","});
    }

    std::ifstream file("save/settings.cfg");
    std::string line;
    while (std::getline(file, line)) {
        settings.push_back(line);
    }
    file.close();
    return settings;
}

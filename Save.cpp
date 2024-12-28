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
            << EscapeForCSV(item.getImage()) << "\n";
    }

    file.close();
    wxMessageBox("Items saved successfully!", "Save Complete", wxICON_INFORMATION);
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
        if (fields.size() == 4) {
            try {
                items.emplace_back(fields[0], fields[1], std::stoi(fields[2]), fields[3]);
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

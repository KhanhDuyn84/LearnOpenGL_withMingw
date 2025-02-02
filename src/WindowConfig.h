#pragma once

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

class WindowConfig : public NonCopyable
{
private:
    void readConfig(const std::string& filename) {
        std::ifstream configFile(filename);
        if (configFile.is_open()) {
            nlohmann::json json;
            configFile >> json;

            width = json["width"];
            height = json["height"];
            title = json["title"];

            // Handle potential exceptions or missing keys
            if (!json.contains("width") || !json.contains("height") || !json.contains("title")) {
                // Handle missing keys in JSON
                std::cerr << "Error: Missing keys in " << filename << std::endl;
            } 
        } else {
            // Handle error: Config file not found
            std::cerr << "Error: Could not open " << filename << std::endl;
        }
    }

    int width;
    int height;
    std::string title;

public:
    WindowConfig(const std::string& filename) : width(800), height(600), title("defaultWindow") {
        readConfig(filename);
    }

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::string& getTitle() const { return title; }
};
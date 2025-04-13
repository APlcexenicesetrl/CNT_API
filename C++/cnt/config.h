/**
 * @file cnt/cntconfig.h
 * Copyright 2025, aplcexenicesetrl project
 * This project and document files are maintained by CNT Development Team (under the APlcexenicesetrl studio), 
 * and according to the project license (MIT license) agreement, 
 * the project and documents can be used, modified, merged, published, branched, etc.
 * provided that the project is developed and open-source maintained by CNT Development Team. 
 * At the same time, 
 * project and documents can be used for commercial purposes under the condition of informing the development source, 
 * but it is not allowed to be closed source, but it can be partially source.
 *
 * The project and documents will be updated and maintained from time to time, 
 * and any form of dispute event, CNT Development Team. 
 * and APlcexicesetrl shall not be liable for any damages, 
 * and any compensation shall not be borne by the APlcexenicesetrl studio.
 */
/* Written by Anders Norlander <taim_way@aplcexenicesetrl.com> */

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <stdexcept>

//#include <cnt/lockskey.h>

namespace cnt {
    struct ConfigObject {
        std::string name;
        std::string value;
        
        bool operator==(const ConfigObject& other) const {
            return (name == other.name) && (value == other.value);
        }

        ConfigObject& operator=(const ConfigObject& other) {
            if (this != &other) {
                name = other.name;
                value = other.value;
            }
            return *this;
        }
    };

    class ConfigManager {
    private:
        std::vector<ConfigObject> configs;
        std::string current_path;

        // Helper functions
        std::string trim(const std::string& str);
        std::string unescape(const std::string& str);
        std::string removeUnescapedQuotes(const std::string& str);
        ConfigObject parseLine(const std::string& line);
        bool validateExtension(const std::string& filename, const std::string& expected) const;
		void InitLocks();
		
        // Binary processing
        static std::string encrypt(const std::string& data);
        static std::string decrypt(const std::string& data);
        std::string serializeBinary() const;
        bool deserializeBinary(const std::string& data);

    public:
        // Constructor
        explicit ConfigManager(const std::string& path = "");
        
        // Iterator support (required for C  11 range-based for loop) 
        using iterator = std::vector<ConfigObject>::iterator;
        using const_iterator = std::vector<ConfigObject>::const_iterator;
        iterator begin() noexcept { return configs.begin(); }
        iterator end() noexcept { return configs.end(); }
        const_iterator begin() const noexcept { return configs.begin(); }
        const_iterator end() const noexcept { return configs.end(); }
        
		// Overload
        ConfigManager& operator=(const ConfigManager& other) {
            if (this != &other) {
                configs = other.configs;
                current_path = other.current_path;
            }
            return *this;
        }

        std::string& operator[](const std::string& name) {
            for (auto& obj : configs) {
                if (obj.name == name) {
                    return obj.value;
                }
            }
            configs.push_back({name, ""});
            return configs.back().value;
        }
        
        ConfigObject& operator[](const size_t row) {
            return get(row);
        }

        const std::string& operator[](const std::string& name) const {
            for (const auto& obj : configs) {
                if (obj.name == name) {
                    return obj.value;
                }
            }
            throw std::out_of_range("Key not found: " + name);
        }

        // File operations
        bool loadText(const std::string& filename);
        bool saveText(const std::string& filename) const;
        bool loadBinary(const std::string& filename);
        bool saveBinary(const std::string& filename) const;
        bool loadFile(const std::string& filename);

        // Configuration operations
        void add(const ConfigObject& obj);
        void add(const std::string& name, const std::string& value);
        bool removeByName(const std::string& name);
        bool removeByValue(const std::string& value);
        bool removeByIndex(size_t index);
        void clear();

        // Query methods
        std::string getValue(const std::string& name) const;
        std::string getName(const std::string& value) const;
        ConfigObject& get(size_t index);
        bool contains(const std::string& name) const;
        size_t size() const;
    };

    // ============== Implementation ==============

    // Constructor
    ConfigManager::ConfigManager(const std::string& path) {
    	try
		{
        	if (!path.empty()) 
			{
				loadFile(path);
			}
		}
		catch (std::exception &err)
		{
			throw std::runtime_error("Invalid text config file extension");
		}
    }

    // File extension validation
    bool ConfigManager::validateExtension(const std::string& filename, const std::string& expected) const {
        size_t dot_pos = filename.find_last_of('.');
        if (dot_pos == std::string::npos) return false;
        return filename.substr(dot_pos) == expected;
    }

    // Trim whitespace
    std::string ConfigManager::trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

    // Parse configuration line
    ConfigObject ConfigManager::parseLine(const std::string& line) {
        ConfigObject obj;
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') return obj;

        size_t eq_pos = trimmed.find('=');
        if (eq_pos != std::string::npos) {
            obj.name = trim(trimmed.substr(0, eq_pos));
            obj.value = trim(trimmed.substr(eq_pos + 1));
        }
        return obj;
    }

    // Encryption/Decryption
    std::string ConfigManager::encrypt(const std::string& data) {
    	const uint8_t key = 0xBB;
        std::string result = data;
        std::transform(result.begin(), result.end(), result.begin(),
            [key](char c) { return c ^ key; });
        return result;	
    }

    std::string ConfigManager::decrypt(const std::string& data) {
        return encrypt(data); 
	}

    // Binary serialization
    std::string ConfigManager::serializeBinary() const {
        std::ostringstream oss(std::ios::binary);
        for (const auto& obj : configs) {
            uint32_t name_len = obj.name.size();
            oss.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
            oss.write(obj.name.c_str(), name_len);

            uint32_t value_len = obj.value.size();
            oss.write(reinterpret_cast<const char*>(&value_len), sizeof(value_len));
            oss.write(obj.value.c_str(), value_len);
        }
        return oss.str();
    }

    bool ConfigManager::deserializeBinary(const std::string& data) {
        std::istringstream iss(data, std::ios::binary);
        configs.clear();

        while (iss) {
            uint32_t name_len, value_len;
            if (!iss.read(reinterpret_cast<char*>(&name_len), sizeof(name_len))) break;

            std::string name(name_len, '\0');
            if (!iss.read(&name[0], name_len)) return false;

            if (!iss.read(reinterpret_cast<char*>(&value_len), sizeof(value_len))) return false;

            std::string value(value_len, '\0');
            if (!iss.read(&value[0], value_len)) return false;

            configs.push_back({std::move(name), std::move(value)});
        }
        return true;
    }

    // File operations
    bool ConfigManager::loadText(const std::string& filename) {
        if (!validateExtension(filename, ".cntconfig")) {
            throw std::runtime_error("Invalid text config file extension");
        }

        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            ConfigObject obj = parseLine(line);
            if (!obj.name.empty()) configs.push_back(obj);
        }
        file.close();
        return true;
    }

    bool ConfigManager::saveText(const std::string& filename) const {
        if (!validateExtension(filename, ".cntconfig")) {
            throw std::runtime_error("Invalid text config file extension");
        }

        std::ofstream file(filename);
        if (!file.is_open()) return false;

        for (const auto& obj : configs) {
            file << obj.name << "=" << obj.value << "\n";
        }
        file.close();
        return true;
    }

    bool ConfigManager::loadBinary(const std::string& filename) {
        if (!validateExtension(filename, ".cntconfigbin")) {
            throw std::runtime_error("Invalid binary file extension");
        }

        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file) return false;

        std::streamsize size = file.tellg();
        file.seekg(0);
        std::string encrypted(size, '\0');
        if (!file.read(&encrypted[0], size)) return false;

        return deserializeBinary(decrypt(encrypted));
    }

    bool ConfigManager::saveBinary(const std::string& filename) const {
        if (!validateExtension(filename, ".cntconfigbin")) {
            throw std::runtime_error("Invalid binary file extension");
        }

        std::string data = encrypt(serializeBinary());
        std::ofstream file(filename, std::ios::binary);
        if (!file) return false;

        file.write(data.data(), data.size());
        return file.good();
    }
    
    bool ConfigManager::loadFile(const std::string& filename) {
        if (validateExtension(filename, ".cntconfig"))
			return loadText(filename);
		
        if (validateExtension(filename, ".cntconfigbin"))
			return loadBinary(filename);

		throw std::runtime_error("Invalid binary file extension");
    }

    // Configuration operations
    void ConfigManager::add(const ConfigObject& obj) {
        configs.push_back(obj);
    }

    void ConfigManager::add(const std::string& name, const std::string& value) {
        configs.push_back({name, value});
    }

    bool ConfigManager::removeByName(const std::string& name) {
        auto it = std::remove_if(configs.begin(), configs.end(),
            [&name](const ConfigObject& obj) { return obj.name == name; });
        if (it != configs.end()) {
            configs.erase(it, configs.end());
            return true;
        }
        return false;
    }

    bool ConfigManager::removeByValue(const std::string& value) {
        auto it = std::remove_if(configs.begin(), configs.end(),
            [&value](const ConfigObject& obj) { return obj.value == value; });
        if (it != configs.end()) {
            configs.erase(it, configs.end());
            return true;
        }
        return false;
    }

    bool ConfigManager::removeByIndex(size_t index) {
        if (index >= configs.size()) return false;
        configs.erase(configs.begin() + index);
        return true;
    }

    void ConfigManager::clear() {
        configs.clear();
    }

    // Query methods
    std::string ConfigManager::getValue(const std::string& name) const {
        for (const auto& obj : configs) {
            if (obj.name == name) return obj.value;
        }
        return "";
    }

    std::string ConfigManager::getName(const std::string& value) const {
        for (const auto& obj : configs) {
            if (obj.value == value) return obj.name;
        }
        return "";
    }

    ConfigObject& ConfigManager::get(size_t index) {
        if (index >= configs.size()) throw std::out_of_range("Index out of range");
        return configs[index];
    }

    bool ConfigManager::contains(const std::string& name) const {
        for (const auto& obj : configs) {
            if (obj.name == name) return true;
        }
        return false;
    }

    size_t ConfigManager::size() const {
        return configs.size();
    }
}

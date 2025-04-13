/**
 * @file cnt/lockskey.h
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
#ifndef CNT_LOCKSKEY_H
#define CNT_LOCKSKEY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>

namespace cnt {

class locks {
private:
    // 密钥元数据结构
    struct KeyMeta {
        std::string author;
        std::string license;
        std::string version;
        std::vector<uint8_t> key_data;
        time_t created_time;
    };

    std::unordered_map<std::string, KeyMeta> key_vault;  // 密钥存储库
    static constexpr size_t MIN_KEY_LENGTH = 32;         // 最小密钥长度

    // 基础加密算法
    std::vector<uint8_t> process_data(const std::vector<uint8_t>& data, 
                                     const std::vector<uint8_t>& key) const {
        if (key.empty()) throw std::invalid_argument("Empty encryption key");
        
        std::vector<uint8_t> result;
        result.reserve(data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            result.push_back(data[i] ^ key[i % key.size()]);
        }
        return result;
    }

public:
    // 密钥操作接口
    void create_key(const std::string& key_id,
                   const std::string& author,
                   const std::string& license,
                   const std::string& version,
                   const std::vector<uint8_t>& key_data) {
        if (key_data.size() < MIN_KEY_LENGTH) {
            throw std::length_error("Key length must be at least 32 bytes");
        }
        key_vault[key_id] = {author, license, version, key_data, time(nullptr)};
    }

    void delete_key(const std::string& key_id) {
        if (!key_vault.erase(key_id)) {
            throw std::out_of_range("Key not found: " + key_id);
        }
    }

    void update_key(const std::string& key_id,
                   const std::vector<uint8_t>& new_key_data) {
        auto it = key_vault.find(key_id);
        if (it == key_vault.end()) {
            throw std::out_of_range("Key not found: " + key_id);
        }
        if (new_key_data.size() < MIN_KEY_LENGTH) {
            throw std::length_error("New key length invalid");
        }
        it->second.key_data = new_key_data;
        it->second.created_time = time(nullptr);
    }

    // 数据加密/解密
    std::vector<uint8_t> encrypt(const std::string& key_id,
                                const std::vector<uint8_t>& plaintext) const {
        const auto& key = get_key(key_id).key_data;
        return process_data(plaintext, key);
    }

    std::vector<uint8_t> decrypt(const std::string& key_id,
                                const std::vector<uint8_t>& ciphertext) const {
        return encrypt(key_id, ciphertext);  // 异或加密解密相同
    }

    // 密钥查询
    const KeyMeta& get_key(const std::string& key_id) const {
        auto it = key_vault.find(key_id);
        if (it == key_vault.end()) {
            throw std::out_of_range("Key not found: " + key_id);
        }
        return it->second;
    }

    // 密钥列表
    std::vector<std::string> list_keys() const {
        std::vector<std::string> keys;
        keys.reserve(key_vault.size());
        for (const auto& [key, _] : key_vault) {
            keys.push_back(key);
        }
        return keys;
    }

    // 协议验证
    bool verify_license(const std::string& key_id,
                       const std::string& expected_license) const {
        return get_key(key_id).license == expected_license;
    }
};

} // namespace cnt

#endif // CNT_LOCKSKEY_H

#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <unordered_map>
#include <string>
#include <mutex>
#include "raylib.h"

class TextureCache {
private:
    std::unordered_map<std::string, Texture2D> cache;
    mutable std::mutex mtx;

    TextureCache() {}
    TextureCache(const TextureCache&) = delete;
    TextureCache& operator=(const TextureCache&) = delete;

public:
    static TextureCache& GetInstance() {
        static TextureCache instance;
        return instance;
    }

    Texture2D GetTexture(const std::string& path) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = cache.find(path);
        if (it != cache.end()) {
            return it->second;
        }
        Texture2D tex = LoadTexture(path.c_str());
        cache[path] = tex;
        return tex;
    }

    ~TextureCache() {
        for (auto& pair : cache) {
            UnloadTexture(pair.second);
        }
    }
};

#endif

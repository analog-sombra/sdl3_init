#ifndef __ASSETS_MANAGER_HPP__
#define __ASSETS_MANAGER_HPP__

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <filesystem>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

// Asset metadata - tracks info about loaded assets
struct AssetMetadata
{
    std::string path;
    size_t fileSize = 0;
    bool isLoaded = false;

    AssetMetadata() = default;
    AssetMetadata(const std::string &filePath, size_t size)
        : path(filePath), fileSize(size), isLoaded(true) {}
};

// Custom exception for asset operations
class AssetException : public std::exception
{
private:
    std::string message;

public:
    explicit AssetException(const std::string &msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

class AssetsManager
{
private:
    // Smart pointers + string IDs for type safety
    std::map<std::string, std::shared_ptr<TTF_Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL_Texture>> textures;

    // Metadata for each asset
    std::map<std::string, AssetMetadata> fontMetadata;
    std::map<std::string, AssetMetadata> textureMetadata;

    // Path -> ID mapping to prevent duplicate loads
    std::map<std::string, std::string> pathToFontId;
    std::map<std::string, std::string> pathToTextureId;

    SDL_Renderer *renderer;
    mutable std::mutex assetMutex; // Thread safety

    // Helper methods
    bool ValidatePath(const std::string &path);
    std::string NormalizePath(const std::string &path);
    size_t GetFileSize(const std::string &path);

public:
    AssetsManager(SDL_Renderer *renderer);
    ~AssetsManager();

    // Font operations - now with string IDs and size parameter
    bool LoadFont(const std::string &id, const std::string &path, int fontSize = 24);
    std::shared_ptr<TTF_Font> GetFont(const std::string &id);
    bool FontExists(const std::string &id) const;

    // Texture operations
    bool LoadTexture(const std::string &id, const std::string &path);
    std::shared_ptr<SDL_Texture> GetTexture(const std::string &id);
    bool TextureExists(const std::string &id) const;
    bool GetTextureDimensions(const std::string &id, int &width, int &height) const;

    // Unload operations
    bool UnloadFont(const std::string &id);
    bool UnloadTexture(const std::string &id);
    void UnloadAllAssets();

    // Query operations
    size_t GetFontCount() const;
    size_t GetTextureCount() const;
    const AssetMetadata *GetFontMetadata(const std::string &id) const;
    const AssetMetadata *GetTextureMetadata(const std::string &id) const;

    // Check if asset is already loaded from path (prevents duplicates)
    std::string GetFontIdByPath(const std::string &path) const;
    std::string GetTextureIdByPath(const std::string &path) const;
};

#endif // __ASSETS_MANAGER_HPP__
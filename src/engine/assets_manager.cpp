#include "engine/assets_manager.hpp"
#include <algorithm>

// ============================================================================
// Helper Methods Implementation
// ============================================================================

bool AssetsManager::ValidatePath(const std::string &path)
{
    try
    {
        return std::filesystem::exists(path);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Path validation error: %s", e.what());
        return false;
    }
}

std::string AssetsManager::NormalizePath(const std::string &path)
{
    try
    {
        return std::filesystem::absolute(path).string();
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to normalize path: %s", e.what());
        return path; // Return original if normalization fails
    }
}

size_t AssetsManager::GetFileSize(const std::string &path)
{
    try
    {
        return std::filesystem::file_size(path);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to get file size: %s", e.what());
        return 0;
    }
}

// ============================================================================
// Constructor & Destructor
// ============================================================================

AssetsManager::AssetsManager(SDL_Renderer *renderer) : renderer(renderer)
{
    if (!renderer)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "AssetsManager initialized with null renderer");
    }
}

AssetsManager::~AssetsManager()
{
    UnloadAllAssets();
}

// ============================================================================
// Font Operations
// ============================================================================

bool AssetsManager::LoadFont(const std::string &id, const std::string &path, int fontSize)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    // Validate input
    if (id.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Font ID cannot be empty");
        return false;
    }

    if (path.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Font path cannot be empty");
        return false;
    }

    // Check if already loaded with this ID
    if (fonts.find(id) != fonts.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Font with ID '%s' already loaded, skipping", id.c_str());
        return false;
    }

    // Check if same path already loaded with different ID (prevent duplicates)
    std::string normalizedPath = NormalizePath(path);
    auto pathIt = pathToFontId.find(normalizedPath);
    if (pathIt != pathToFontId.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Font path '%s' already loaded as '%s', reusing existing asset",
                    path.c_str(), pathIt->second.c_str());
        // Create an alias to existing font
        fonts[id] = fonts[pathIt->second];
        fontMetadata[id] = fontMetadata[pathIt->second];
        return true;
    }

    // Validate path exists
    if (!ValidatePath(normalizedPath))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Font file not found: %s", path.c_str());
        return false;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont(normalizedPath.c_str(), fontSize);
    if (!font)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to load font '%s': %s", path.c_str(), SDL_GetError());
        return false;
    }

    // Store with smart pointer (automatic cleanup)
    fonts[id] = std::shared_ptr<TTF_Font>(font, [](TTF_Font *f)
                                          { TTF_CloseFont(f); });
    fontMetadata[id] = AssetMetadata(normalizedPath, GetFileSize(normalizedPath));
    pathToFontId[normalizedPath] = id;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Font loaded successfully: ID='%s', Path='%s', Size=%dpt",
                id.c_str(), normalizedPath.c_str(), fontSize);

    return true;
}

std::shared_ptr<TTF_Font> AssetsManager::GetFont(const std::string &id)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = fonts.find(id);
    if (it == fonts.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Font not found: %s", id.c_str());
        return nullptr;
    }

    return it->second;
}

bool AssetsManager::FontExists(const std::string &id) const
{
    std::lock_guard<std::mutex> lock(assetMutex);
    return fonts.find(id) != fonts.end();
}

// ============================================================================
// Texture Operations
// ============================================================================

bool AssetsManager::LoadTexture(const std::string &id, const std::string &path)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    // Validate input
    if (id.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture ID cannot be empty");
        return false;
    }

    if (path.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture path cannot be empty");
        return false;
    }

    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Renderer not initialized, cannot load texture");
        return false;
    }

    // Check if already loaded with this ID
    if (textures.find(id) != textures.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Texture with ID '%s' already loaded, skipping", id.c_str());
        return false;
    }

    // Check if same path already loaded (prevent duplicates)
    std::string normalizedPath = NormalizePath(path);
    auto pathIt = pathToTextureId.find(normalizedPath);
    if (pathIt != pathToTextureId.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Texture path '%s' already loaded as '%s', reusing existing asset",
                    path.c_str(), pathIt->second.c_str());
        // Create an alias to existing texture
        textures[id] = textures[pathIt->second];
        textureMetadata[id] = textureMetadata[pathIt->second];
        return true;
    }

    // Validate path exists
    if (!ValidatePath(normalizedPath))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Texture file not found: %s", path.c_str());
        return false;
    }

    // Load texture
    SDL_Texture *texture = IMG_LoadTexture(renderer, normalizedPath.c_str());
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to load texture '%s': %s", path.c_str(), SDL_GetError());
        return false;
    }

    // Store with smart pointer (automatic cleanup)
    textures[id] = std::shared_ptr<SDL_Texture>(texture, [](SDL_Texture *t)
                                                { SDL_DestroyTexture(t); });
    textureMetadata[id] = AssetMetadata(normalizedPath, GetFileSize(normalizedPath));
    pathToTextureId[normalizedPath] = id;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "Texture loaded successfully: ID='%s', Path='%s'",
                id.c_str(), normalizedPath.c_str());

    return true;
}

std::shared_ptr<SDL_Texture> AssetsManager::GetTexture(const std::string &id)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = textures.find(id);
    if (it == textures.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Texture not found: %s", id.c_str());
        return nullptr;
    }

    return it->second;
}

bool AssetsManager::TextureExists(const std::string &id) const
{
    std::lock_guard<std::mutex> lock(assetMutex);
    return textures.find(id) != textures.end();
}

bool AssetsManager::GetTextureDimensions(const std::string &id, int &width, int &height) const
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = textures.find(id);
    if (it == textures.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Texture not found: %s", id.c_str());
        return false;
    }

    float w, h;
    if (!SDL_GetTextureSize(it->second.get(), &w, &h))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to query texture dimensions: %s", SDL_GetError());
        return false;
    }

    width = static_cast<int>(w);
    height = static_cast<int>(h);
    return true;
}

// ============================================================================
// Unload Operations
// ============================================================================

bool AssetsManager::UnloadFont(const std::string &id)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = fonts.find(id);
    if (it == fonts.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Font not found for unload: %s", id.c_str());
        return false;
    }

    // Get path before erasing
    auto metaIt = fontMetadata.find(id);
    if (metaIt != fontMetadata.end())
    {
        pathToFontId.erase(metaIt->second.path);
        fontMetadata.erase(metaIt);
    }

    fonts.erase(it);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Font unloaded: %s", id.c_str());
    return true;
}

bool AssetsManager::UnloadTexture(const std::string &id)
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = textures.find(id);
    if (it == textures.end())
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Texture not found for unload: %s", id.c_str());
        return false;
    }

    // Get path before erasing
    auto metaIt = textureMetadata.find(id);
    if (metaIt != textureMetadata.end())
    {
        pathToTextureId.erase(metaIt->second.path);
        textureMetadata.erase(metaIt);
    }

    textures.erase(it);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Texture unloaded: %s", id.c_str());
    return true;
}

void AssetsManager::UnloadAllAssets()
{
    std::lock_guard<std::mutex> lock(assetMutex);

    fonts.clear();
    textures.clear();
    fontMetadata.clear();
    textureMetadata.clear();
    pathToFontId.clear();
    pathToTextureId.clear();

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "All assets unloaded");
}

// ============================================================================
// Query Operations
// ============================================================================

size_t AssetsManager::GetFontCount() const
{
    std::lock_guard<std::mutex> lock(assetMutex);
    return fonts.size();
}

size_t AssetsManager::GetTextureCount() const
{
    std::lock_guard<std::mutex> lock(assetMutex);
    return textures.size();
}

const AssetMetadata *AssetsManager::GetFontMetadata(const std::string &id) const
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = fontMetadata.find(id);
    if (it == fontMetadata.end())
    {
        return nullptr;
    }

    return &it->second;
}

const AssetMetadata *AssetsManager::GetTextureMetadata(const std::string &id) const
{
    std::lock_guard<std::mutex> lock(assetMutex);

    auto it = textureMetadata.find(id);
    if (it == textureMetadata.end())
    {
        return nullptr;
    }

    return &it->second;
}

std::string AssetsManager::GetFontIdByPath(const std::string &path) const
{
    std::lock_guard<std::mutex> lock(assetMutex);

    std::string normalizedPath = const_cast<AssetsManager *>(this)->NormalizePath(path);
    auto it = pathToFontId.find(normalizedPath);

    return (it != pathToFontId.end()) ? it->second : "";
}

std::string AssetsManager::GetTextureIdByPath(const std::string &path) const
{
    std::lock_guard<std::mutex> lock(assetMutex);

    std::string normalizedPath = const_cast<AssetsManager *>(this)->NormalizePath(path);
    auto it = pathToTextureId.find(normalizedPath);

    return (it != pathToTextureId.end()) ? it->second : "";
}
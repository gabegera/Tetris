#pragma once

#include <filesystem>
#include <string>

#include "ColorPalettes.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

class Button;
struct Shape;
struct Block;
class Application;

enum class HorizontalAlignment : Uint8
{
    Left = 0,
    Center = 1,
    Right = 2
};

enum class VerticalAlignment : Uint8
{
    Top = 0,
    Center = 1,
    Bottom = 2
};

class Renderer
{
public:
    explicit Renderer(Application& app);
    ~Renderer();

protected:
    Application& m_application;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_TextEngine* m_textEngine = nullptr;
    TTF_Font* m_font = nullptr;

//TODO: Need to package the files into the exe instead of relying of external paths.
#if DEBUG_MODE
    const std::filesystem::path m_blockTexturePath = "../res/Tetris_Block.png";
    const std::filesystem::path m_transparentBlockTexturePath = "../res/Transparent_Tetris_Block.png";
    const std::filesystem::path m_defaultFontPath = "../res/Fonts/Roboto.ttf";
#else
    const std::filesystem::path m_blockPath = "./res/Tetris_Block.png";
    const std::filesystem::path m_transparentBlockPath = "./res/Transparent_Tetris_Block.png";
    const std::filesystem::path m_defaultFontPath = "./res/Fonts/Roboto.ttf";
#endif

    // Width and Height of the blocks in pixels.
    const unsigned int m_blockResolution = 16;

    SDL_Texture* m_blockTexture = nullptr;
    SDL_Texture* m_transparentBlockTexture = nullptr;

    ColorPalette m_colorPalette = ColorPalettes::Classic;

    void SetBlockTexture(const std::filesystem::path& texturePath);
    void SetTransparentBlockTexture(const std::filesystem::path& texturePath);

    void SetFont(const std::filesystem::path& fontPath);

    void SetColorPalette(const ColorPalette& newPalette);

    void ResetDrawColor() const;

public:
    void Init();
    void Stop() const;
    void Update(const float deltaTime) const;

    void SetRendererSizeToWindowSize() const;
    void SetRendererSizeInPixels(Uint16 width, Uint16 height) const;
    void SetRendererSizeInGameDimensions(Uint16 gameWidth, Uint16 gameHeight) const;

    void ClearRenderer() const;

    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, Uint16 colorID) const;
    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, Color color) const;

    void DrawShapeGuideAtPos(Uint8 xPos, Uint8 yPos, const Shape& shape, Uint16 colorID) const;
    void DrawShapeGuideAtPos(Uint8 xPos, Uint8 yPos, const Shape& shape, Color color) const;

    void DrawText(const std::string& inString, Uint32 xPos, Uint32 yPos, Color color,
        Uint32 size, HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
        VerticalAlignment verticalAlignment = VerticalAlignment::Top);
    void DrawButton(const Button* button);

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetSDLRenderer() const;

    const ColorPalette& GetColorPalette() const;

    Uint32 GetRenderWidth() const;
    Uint32 GetRenderHeight() const;

    TTF_Font* GetDefaultFont() const;
    Uint32 GetDefaultFontSize() const;

};

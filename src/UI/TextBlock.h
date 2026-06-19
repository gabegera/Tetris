#pragma once

#include "UIElement.h"
#include "../Theme.h"

class TextBlock : public UIElement
{
public:
    explicit TextBlock(Menu& menu);
    ~TextBlock() override;

protected:
    std::string m_text = "Empty Text Block";

    Uint32 m_fontSize = 32;

    Color m_textColor = {255, 255, 255};

    void Render() override;

public:
    void SetText(const std::string& inText);

    void SetFontSize(Uint32 inSize);

    void SetTextColor(const Color& inColor);

    const std::string& GetText() const;

    Uint32 GetFontSize() const;

    const Color& GetTextColor() const;};

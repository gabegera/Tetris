#include "TextBlock.h"

#include "../Renderer.h"

TextBlock::TextBlock(Menu& menu) : UIElement(menu, false)
{

}

TextBlock::~TextBlock()
{

}

void TextBlock::Render()
{
    GetRenderer()->DrawText(m_text, GetXPos(), GetYPos(), m_textColor, m_fontSize,
        m_horizontalAlignment, m_verticalAlignment);
}

void TextBlock::SetText(const std::string& inText)
{
    m_text = inText;
}

void TextBlock::SetFontSize(const Uint32 inSize)
{
    m_fontSize = inSize;
}

void TextBlock::SetTextColor(const Color& inColor)
{
    m_textColor = inColor;
}

const std::string& TextBlock::GetText() const
{
    return m_text;
}

Uint32 TextBlock::GetFontSize() const
{
    return m_fontSize;
}

const Color& TextBlock::GetTextColor() const
{
    return m_textColor;
}
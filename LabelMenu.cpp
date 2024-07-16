#include "SubMenu.hpp"

LabelMenu::LabelMenu() : SubMenu()
{
    m_str = nullptr;
    m_fmt = nullptr;
}

LabelMenu::LabelMenu(std::string str, std::string fmt, std::uint8_t segHeight, std::uint8_t segWidth,
		     std::uint8_t xOffset, std::uint8_t yOffset) : SubMenu(segHeight, segWidth, xOffset, yOffset)
{
	m_str = str;
	m_fmt = fmt;
}

float LabelMenu::callDisplayFunction()
{
    return NAN;
}
void LabelMenu::callExecuteFunction()
{
    ;
}
void LabelMenu::setHighlight()
{

}
void LabelMenu::unsetHighlight()
{

}

bool LabelMenu::updateValue()
{
    return false;
}


bool LabelMenu::draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    displayDevice->fillRectangle(x, y, x+m_segWidth, y+m_segHeight, m_bgColour);
    displayDevice->drawRectangle(x, y, x+m_segWidth, y+m_segHeight, m_borderColour);
    writeText(displayDevice, TextAlign::Centre, x, y);
    return true;
}

bool LabelMenu::refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    return true;
}

bool LabelMenu::writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y)
{
    std::uint8_t fontWidth = displayDevice->getFont().width;
    std::uint8_t fontHeight = displayDevice->getFont().height;

    std::uint16_t stringDisplayLength = m_str.size() * fontWidth;


    std::uint8_t startX;
    switch(textAlign)
    {
	case TextAlign::Centre:
	    startX = (m_segWidth - stringDisplayLength) / 2;
	    break;
	case TextAlign::Left:
	    startX = 2;
	    break;
	default:
	    break;
    }

    std::uint8_t startY = (m_segHeight - fontHeight) / 2;
    displayDevice->setCursorXY(x+startX, y+startY);
    if(stringDisplayLength > m_segWidth)
    {
	displayDevice->writeString("err", m_textColour, m_bgColour);
	return false;
    }
    displayDevice->writeString(m_str, m_textColour, m_bgColour);
    return true;
}

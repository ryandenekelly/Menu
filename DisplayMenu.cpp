#include "SubMenu.hpp"

DisplayMenu::DisplayMenu() : SubMenu()
{
    m_str = nullptr;
    m_fmt = nullptr;
    m_displayFunction = nullptr;
    m_value = NAN;
}

DisplayMenu::DisplayMenu(std::string str, std::string fmt, float (*displayFunction)(), float value,
			    std::uint8_t segHeigth, std::uint8_t segWidth, std::uint8_t xOffset,
			    std::uint8_t yOffset) : SubMenu(segHeigth, segWidth, xOffset, yOffset)
{
    m_str = str;
    m_fmt = fmt;
    m_displayFunction = displayFunction;
    m_value = value;
    lastUpdate = 0;
    updateInterval = 500;
}

bool DisplayMenu::writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y)
{
    char buffer[SubMenu::MAX_VALUE_LEN] = {0};
    snprintf(buffer, SubMenu::MAX_VALUE_LEN, m_fmt.c_str(), m_value);
    std::string strValue = !std::isnan(m_value) ? std::string(buffer) : "#";
    std::string combinedString = m_str + strValue;

    std::uint8_t fontWidth = displayDevice->getFont().width;
    std::uint8_t fontHeight = displayDevice->getFont().height;

    std::uint16_t stringDisplayLength = combinedString.size() * fontWidth;


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
    displayDevice->writeString(combinedString, m_textColour, m_bgColour);
    return true;
}

float DisplayMenu::callDisplayFunction()
{
    return NAN;
}

void DisplayMenu::callExecuteFunction(){;}
void DisplayMenu::setHighlight(){}
void DisplayMenu::unsetHighlight(){}

bool DisplayMenu::updateValue()
{
    if(HAL_GetTick() - lastUpdate < updateInterval)
    {
	return true;
    }
   float newValue = m_displayFunction();
   m_value = std::isnan(m_value) ? 0 : m_value;
   if(fabs(newValue-m_value) > 0.01)
   {
       m_value = m_displayFunction();
       lastUpdate = HAL_GetTick();
       return true;
   }
   return false;


}

bool DisplayMenu::draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    displayDevice->fillRectangle(x, y, x+m_segWidth, y+m_segHeight, m_bgColour);
    displayDevice->drawRectangle(x, y, x+m_segWidth, y+m_segHeight, m_borderColour);
    updateValue();
    writeText(displayDevice, TextAlign::Left, x, y);
    return true;
}

bool DisplayMenu::refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    if(updateValue())
    {
	displayDevice->fillRectangle(x, y, x+m_segWidth, y+m_segHeight, m_bgColour);
	displayDevice->drawRectangle(x, y, x+m_segWidth, y+m_segHeight, m_borderColour);
	writeText(displayDevice, TextAlign::Left, x, y);
	return true;
    }
    return false;
}

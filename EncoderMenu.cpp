#include "SubMenu.hpp"

EncoderMenu::EncoderMenu() : SubMenu()
{
    m_str = "";
    m_fmt = "";
    m_value = NAN;
    m_storedSign = 1;
    m_res = 0;
    m_encoder = nullptr;
    m_dirty = false;
}

EncoderMenu::EncoderMenu(std::string str, std::string fmt, TIM_HandleTypeDef* encoder, std::uint8_t segHeight, std::uint8_t segWidth)
    : SubMenu(segHeight, segWidth, 0, 0)
{
    m_str = str;
    m_fmt = fmt;
    m_value = NAN;
    m_storedSign = 1;
    m_encoder = encoder;
    m_res = 0.1;
    m_dirty= false;
    lastUpdate = 0;
    updateInterval = 500;
}

EncoderMenu::EncoderMenu(std::string str, std::string fmt, TIM_HandleTypeDef* encoder, void(*func)(float), std::uint8_t segHeight, std::uint8_t segWidth)
    : SubMenu(segHeight, segWidth, 0, 0)
{
    m_str = str;
    m_fmt = fmt;
    m_value = NAN;
    m_storedSign = 1;
    m_encoder = encoder;
    m_res = 0.1;
    m_executeFunction = func;
    m_dirty = false;
    lastUpdate = 0;
    updateInterval = 500;
}

void EncoderMenu::highlight()
{
    std::uint32_t unsigned_val = m_value < 0 ? (1 << 16) - (m_value / m_res) : (m_value / m_res);
    m_encoder->Instance->CNT = unsigned_val;

    m_highlighted = true;
}
void EncoderMenu::unhighlight()
{
    updateValue();
    m_highlighted = false;
}


void EncoderMenu::setExecuteFunction(void(*func)(float))
{
    m_executeFunction = func;
}

void EncoderMenu::callExecuteFunction()
{
    m_dirty = false;
    m_executeFunction(m_value);
}

bool EncoderMenu::writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y)
{
    char buffer[SubMenu::MAX_VALUE_LEN] = {0};
    snprintf(buffer, SubMenu::MAX_VALUE_LEN, m_fmt.c_str(), m_value);
    std::string strValue = !std::isnan(m_value) ? std::string(buffer) : "#";
    std::string combinedString = m_str + strValue;
    combinedString = m_dirty ? combinedString + "*" : combinedString;

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

bool EncoderMenu::updateValue()
{
    if(HAL_GetTick() - lastUpdate < updateInterval)
    {
	return true;
    }
    m_value = std::isnan(m_value) ? 0 : m_value;
    float newValue = (std::int16_t)(m_encoder->Instance->CNT) * m_res;
    if(fabs(m_value - newValue) > 0.01)
    {
	if(m_highlighted)
	{
	    m_value = newValue;
	    m_dirty = true;
	    lastUpdate = HAL_GetTick();
	    return true;
	}
    }
    return false;


}

bool EncoderMenu::draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{

    updateValue();
    displayDevice->fillRectangle(x, y, x+m_segWidth, y+m_segHeight, m_bgColour);
    displayDevice->drawRectangle(x, y, x+m_segWidth, y+m_segHeight, m_borderColour);
    writeText(displayDevice, TextAlign::Left, x, y);
    return true;

}

bool EncoderMenu::refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
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

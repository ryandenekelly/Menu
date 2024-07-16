#include "SubMenu.hpp"
#include "printf.h"

/** Constructors **/
SubMenu::SubMenu()
{
	m_segHeight = 0;
	m_segWidth = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_highlighted = false;
	m_highlightPending = false;
	m_borderColour = DisplayDevice::White;
	m_textColour = DisplayDevice::White;
	m_bgColour = DisplayDevice::Black;
}

SubMenu::SubMenu(std::uint8_t segHeight, std::uint8_t segWidth, std::uint8_t xOffset, std::uint8_t yOffset)
{
	m_segHeight = segHeight;
	m_segWidth = segWidth;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_highlighted = false;
	m_highlightPending = false;
	m_borderColour = DisplayDevice::White;
	m_textColour = DisplayDevice::White;
	m_bgColour = DisplayDevice::Black;
}

void SubMenu::setOffsets(std::uint8_t x, std::uint8_t y)
{
    m_xOffset = x;
    m_yOffset = y;
}
void SubMenu::setWidth(std::uint8_t width)
{
    m_segWidth = width;
}
void SubMenu::setValue(float value)
{
    m_value = value;
}
float SubMenu::getValue()
{
    return m_value;
}
std::uint8_t SubMenu::getSegWidth()
{
    return m_segWidth;
}
std::uint8_t SubMenu::getSegHeight()
{
    return m_segHeight;
}
bool SubMenu::unInitialised()
{
    return m_segWidth == 0 || m_segHeight == 0;
}
float SubMenu::callDisplayFunction()
{
    return NAN;
}
void SubMenu::callExecuteFunction()
{
    ;
}
void SubMenu::highlight()
{
    m_highlighted = true;
}
void SubMenu::unhighlight()
{
    m_highlighted = false;
}
bool SubMenu::updateValue()
{
    return false;
}
void SubMenu::invertSegment(DisplayDevice* displayDevice)
{
    m_borderColour = DisplayDevice::invertColour(m_borderColour);
    m_textColour = DisplayDevice::invertColour(m_textColour);
    m_bgColour = DisplayDevice::invertColour(m_bgColour);
}

bool SubMenu::refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    return false;
}

bool SubMenu::draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y)
{
    return false;
}
bool SubMenu::writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y)
{
    return false;
}



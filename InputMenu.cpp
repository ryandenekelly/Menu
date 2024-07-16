#include "SubMenu.hpp"

InputMenu::InputMenu() : DisplayMenu()
{
    m_executeFunction = nullptr;
}

InputMenu::InputMenu(std::string str, std::string fmt,  float (*displayFunction)(),
		     void(*executeFunction)(float) , float value, std::uint8_t segHeight,
		     std::uint8_t segWidth, std::uint8_t xOffset, std::uint8_t yOffset)
		     : DisplayMenu(str, fmt, displayFunction, value, segHeight, segWidth, xOffset, yOffset)
{
    m_executeFunction = executeFunction;
}

float InputMenu::callDisplayFunction()
{
    return NAN;
}
void InputMenu::callExecuteFunction()
{
    ;
}

void InputMenu::highlight()
{
    m_highlighted = true;
}
void InputMenu::unhighlight()
{
    m_highlighted = false;
}

bool InputMenu::updateValue()
{
       m_value = m_displayFunction();
       return true;
}

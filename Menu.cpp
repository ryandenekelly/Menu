#include "Menu.hpp"
#include "interrupts.hpp"
#include "tim.h"

#include <cstring>
#include <typeinfo>
#include <cstdarg>

Menu::Menu()
{
    m_displayDevice = nullptr;
    m_menuCursor = 0;
    m_highlightCount = 0;
    subMenuSize = 0;
}

Menu::Menu(DisplayDevice * device) : m_subMenus()
{
    m_displayDevice = device;
    m_menuCursor = 0;
    m_highlightCount = 0;
    subMenuSize = 0;
}

std::uint8_t Menu::height()
{
    return m_displayDevice->height();
}

std::uint8_t Menu::width()
{
    return m_displayDevice->width();
}

void Menu::drawInitialMenu()
{
    std::uint16_t x_pos = 0;
    std::uint16_t y_pos = 0;

    for(std::uint8_t i=0; i<V_MAX; i++)
    {
	x_pos = 0; // Reset x_pos for new row.

	for(std::uint8_t j=0; j<H_MAX; j++)
	{
	    if(m_subMenus[(i*H_MAX) + j])
	    {
		m_subMenus[(i*H_MAX) + j]->draw(m_displayDevice, x_pos, y_pos);
		x_pos += m_subMenus[(i*H_MAX) + j]->getSegWidth();
	    }
	}
	y_pos += m_subMenus[(i*H_MAX)]->getSegHeight();	// Increment height based on first of row.
    }

    m_displayDevice->refreshScreen();
}

void Menu::drawMenu()
{
    std::uint16_t x_pos = 0;
    std::uint16_t y_pos = 0;

    for(std::uint8_t i=0; i<V_MAX; i++)
    {
	x_pos = 0; // Reset x_pos for new row.

	for(std::uint8_t j=0; j<H_MAX; j++)
	{
	    if(m_subMenus[(i*H_MAX) + j])
	    {
		m_subMenus[(i*H_MAX) + j]->refresh(m_displayDevice, x_pos, y_pos);
		x_pos += m_subMenus[(i*H_MAX) + j]->getSegWidth();
	    }
	}
	y_pos += m_subMenus[(i*H_MAX)]->getSegHeight();	// Increment height based on first of row.
    }

    m_displayDevice->refreshScreen();
}

void Menu::drawSubMenu()
{
    m_subMenus[m_menuCursor]->updateValue();
    m_subMenus[m_menuCursor]->draw(m_displayDevice, 0, 0);
}


bool Menu::addSubMenu(SubMenu&& subMenu)
{
    if(subMenuSize == V_MAX)
    {
	return false;
    }
    std::uint8_t xOffset = 0;
    std::uint8_t yOffset = (subMenuSize*H_MAX)*16;
    subMenu.setOffsets(xOffset, yOffset);
    m_subMenus[subMenuSize++*H_MAX] = &subMenu;
    return true;
}

bool Menu::addSubMenu(SubMenu&& subMenuA, SubMenu&& subMenuB, SubMenu&& subMenuC, SubMenu&& subMenuD)
{
    if(subMenuSize == V_MAX)
    {
	return false;
    }
    bool subMenuCInit = subMenuC.unInitialised();
    bool subMenuDInit = subMenuD.unInitialised();
    std::uint8_t count = 2 + (std::uint8_t)!subMenuCInit + (std::uint8_t)!subMenuDInit;
    if(count < 3)
    {
	return false;
    }

    std::uint8_t subMenuWidth = width() / count;
    std::uint8_t segmentIndex = subMenuSize++*H_MAX;

    for(auto subMenu : {&subMenuA, &subMenuB, &subMenuC, &subMenuD})
    {
	if(!subMenu->unInitialised())
	{
	    subMenu->setWidth(subMenuWidth);
	    m_subMenus[segmentIndex++] = subMenu;
	}
    }

    return true;
}

// NB: Bad function!
bool Menu::addSubMenu(std::uint8_t count...)
{
    if(count > 3)
    {
	return false;
    }
    std::uint8_t subMenuWidth = width() / count;
    std::uint8_t segmentIndex = subMenuSize++*H_MAX;

    va_list args;
    va_start(args, count);
    for(std::uint8_t i=0; i<count; i++)
    {
	SubMenu* subMenu = va_arg(args, SubMenu*);
	subMenu->setWidth(subMenuWidth);
	m_subMenus[segmentIndex+i] = subMenu;
    }
    va_end(args);
    return true;
}



bool Menu::initMenuCursor()
{
    if(subMenuSize)
    {
	updateCursor(0);
	return true;
    }
    return false;
}

void Menu::updateCursor(std::uint8_t newIndex)
{
    SubMenu* oldCursor = m_subMenus[m_menuCursor];
    if(m_highlightCount > 0)
    {
	m_highlightCount--;
	oldCursor->unhighlight();
	oldCursor->invertSegment(m_displayDevice);
    }
    if(m_highlightCount == 0)
    {
	m_subMenus[newIndex]->invertSegment(m_displayDevice);
	m_subMenus[newIndex]->highlight();
	m_highlightCount++;
	m_menuCursor = newIndex;
    }
}

std::pair<std::uint8_t, std::uint8_t> Menu::indexOfCursor()
{
    std::uint8_t rowIndex = 0;
    std::uint8_t colIndex =  0;
    return {rowIndex, colIndex};
}
bool Menu::moveCursorDown()
{
    if(m_menuCursor == subMenuSize*4)
    {
	return false;
    }
    std::uint8_t next = ((m_menuCursor + 4) / 4) * 4;
    if(m_subMenus[next])
    {
	updateCursor(next);
	return true;
    }
    return false;
}

bool Menu::moveCursorUp()
{
    if(m_menuCursor == 0)
    {
	return false;
    }
    std::uint8_t next = ((m_menuCursor - 4) / 4) * 4;
    if(m_subMenus[next])
    {
	updateCursor(next);
	return true;
    }
    return false;
}

bool Menu::moveCursorLeft()
{
    if(m_menuCursor % 4 == 0)
    {
	return false;
    }
    if(m_subMenus[m_menuCursor-1])
    {
	updateCursor(m_menuCursor-1);
	return true;
    }
    return false;
}

bool Menu::moveCursorRight()
{
    if(m_menuCursor % 4 == 3)
    {
	return false;
    }
    if(m_subMenus[m_menuCursor+1])
    {
	updateCursor(m_menuCursor+1);
	return true;
    }
    return false;

}

bool Menu::enterValue(float value)
{

    // need to set the value on screen
    m_subMenus[m_menuCursor]->setValue(value);

    return false;
}


bool Menu::addEncoder(Encoder&& enc)
{
    if(m_encoderSize + 1 >= ENC_MAX)
    {
	return false;
    }
    if(m_encoders[m_encoderSize])
    {
	m_encoders[m_encoderSize++] = &enc;
	return true;
    }
    return false;
}

bool Menu::addPushButton(PushButton&& btn)
{
    if(m_buttonSize + 1 >= BTN_MAX)
    {
	return false;
    }
    if(m_buttons[m_buttonSize])
    {
	m_buttons[m_buttonSize++] = &btn;
	return true;
    }
    return false;
}

void Menu::processButtonInput()
{
    if(g_button_left_pending)
    {
	g_button_left_pending = false;
	moveCursorLeft();
    }
    if(g_button_up_pending)
    {
	g_button_up_pending = false;
	moveCursorUp();
    }
    if(g_button_down_pending)
    {
	g_button_down_pending  = false;
	moveCursorDown();
    }
    if(g_button_right_pending)
    {
	g_button_right_pending = false;
	moveCursorRight();
    }
    if(g_button_enc_push_pending)
    {
	g_button_enc_push_pending = false;
	m_subMenus[m_menuCursor]->callExecuteFunction();
    }
}

void Menu::runMenu()
{
    HAL_Delay(200);
    drawMenu();
    processButtonInput();
}

void Menu::startMenu()
{
    initMenuCursor();
    drawInitialMenu();

}

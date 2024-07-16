#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include <initializer_list>


#include "DisplayDevice.hpp"
#include "SubMenu.hpp"


class Menu
{
    public:
	Menu();
	Menu(DisplayDevice *device);

	static constexpr std::uint8_t V_MAX = 10;
	static constexpr std::uint8_t H_MAX = 4;
	static constexpr std::uint8_t ENC_MAX = 3;
	static constexpr std::uint8_t BTN_MAX = 6;

	std::uint8_t height();
	std::uint8_t width();

	void resetScreen();

	void drawInitialMenu();
	void drawMenu();
	void drawSubMenu();
	void updateCursor(std::uint8_t newIndex);
	bool initMenuCursor();

	std::pair<std::uint8_t, std::uint8_t> indexOfCursor();
	bool moveCursorDown();
	bool moveCursorUp();
	bool moveCursorLeft();
	bool moveCursorRight();
	bool enterValue(float value);
	void processButtonInput();
	void proccessEncoderInput();
	void startMenu();
	void runMenu();



	virtual bool addSubMenu(SubMenu&& subMenu);
	virtual bool addSubMenu(SubMenu&& subMenuA, SubMenu&& subMenuB, SubMenu&& subMenuC=SubMenu(), SubMenu&& subMenuD=SubMenu());
	virtual bool addSubMenu(std::uint8_t count...);

	bool addEncoder(Encoder&& enc);
	bool addPushButton(PushButton&& btn);


	std::uint8_t subMenuSize;
    private:
	DisplayDevice *m_displayDevice;
	SubMenu* m_subMenus[V_MAX*H_MAX];
	std::uint8_t m_menuCursor;
	std::uint8_t m_highlightCount;
	Encoder* m_encoders[ENC_MAX];
	std::uint8_t m_encoderSize;
	PushButton* m_buttons[BTN_MAX];
	std::uint8_t m_buttonSize;






};



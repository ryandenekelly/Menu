#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "DisplayDevice.hpp"
#include "MenuInput.hpp"


class SubMenu
{
    public:
	// Default
	SubMenu();
	SubMenu(std::uint8_t segHeigth, std::uint8_t segWidth, std::uint8_t xOffset, std::uint8_t yOffset);

	static constexpr std::uint8_t MAX_VALUE_LEN = 6;
	enum class TextAlign
	{
	    Centre,
	    Right,
	    Left
	};

	std::uint8_t getSegWidth();
	std::uint8_t getSegHeight();
	void setOffsets(std::uint8_t x, std::uint8_t y);
	void setWidth(std::uint8_t width);
	void setValue(float value);
	float getValue();
	void invertSegment(DisplayDevice* displayDevice);
	virtual bool unInitialised();
	virtual float callDisplayFunction();
	virtual void callExecuteFunction();
	virtual void highlight();
	virtual void unhighlight();
	virtual bool updateValue();

	virtual bool draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);
	virtual bool refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);

    protected:
	std::uint8_t m_segHeight;
	std::uint8_t m_segWidth;
	std::uint8_t m_xOffset;
	std::uint8_t m_yOffset;
	std::uint16_t m_borderColour;
	std::uint16_t m_textColour;
	std::uint16_t m_bgColour;
    	bool m_highlighted;
    	bool m_highlightPending;
    	float m_value;

    private:
	virtual bool writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y);
};

class LabelMenu : public SubMenu
{
    public:
	LabelMenu();
	LabelMenu(std::string str, std::string fmt = "%s", std::uint8_t segHeigth=16, std::uint8_t segWidth=127, std::uint8_t xOffset=0,
		  std::uint8_t yOffset=0);


	float callDisplayFunction();
	void callExecuteFunction();
	void setHighlight();
	void unsetHighlight();
	bool updateValue();
	bool draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);
	bool refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);

    private:
	std::string m_str;
	std::string m_fmt;

	bool writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y);
};

class DisplayMenu : public SubMenu
{
    public:
	DisplayMenu();
	DisplayMenu(std::string str, std::string fmt, float (*displayFunction)(), float value = NAN,
		    std::uint8_t segHeigth=16, std::uint8_t segWidth=127, std::uint8_t xOffset=0, std::uint8_t yOffset=0);



	float callDisplayFunction();
	void callExecuteFunction();
	void setHighlight();
	void unsetHighlight();
	bool updateValue();
	bool draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);
	bool refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);

    protected:
	float m_value;
	float (*m_displayFunction)();
	std::string m_str;
	std::string m_fmt;
	std::int8_t m_storedSign;
	std::uint32_t lastUpdate;
	std::uint32_t updateInterval;
    private:
	bool writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y);

};

class InputMenu : public DisplayMenu
{
    public:
	InputMenu();
	InputMenu(std::string str, std::string fmt, float (*displayFunction)(),
		  void(*executeFunction)(float), float value=NAN,
		  std::uint8_t segHeigth=16, std::uint8_t segWidth=127, std::uint8_t xOffset=0, std::uint8_t yOffset=0);
	InputMenu(std::string str, std::string fmt, TIM_HandleTypeDef* encoder, std::uint8_t segHeight=16, std::uint8_t segWidth=127);

	float callDisplayFunction();
	void callExecuteFunction();
	void highlight();
	void unhighlight();
	bool updateValue();

    private:
	MenuInput m_inputType;
	void (*m_executeFunction)(float);

};

class EncoderMenu : public SubMenu
{
    public:
	EncoderMenu();
	EncoderMenu(std::string str, std::string fmt, TIM_HandleTypeDef* encoder, std::uint8_t segHeight=16, std::uint8_t segWidth=127);
	EncoderMenu(std::string str, std::string fmt, TIM_HandleTypeDef* encoder, void (*executeFunction)(float), std::uint8_t segHeight=16, std::uint8_t segWidth=127);

	void highlight();
	void unhighlight();
	bool updateValue();
	void setExecuteFunction(void(*)(float));
	void callExecuteFunction();
	bool draw(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);
	bool refresh(DisplayDevice* displayDevice, std::uint16_t x, std::uint16_t y);

    private:
	float m_value;
	std::string m_str;
	std::string m_fmt;
	float m_res;
	TIM_HandleTypeDef* m_encoder;
	std::int8_t m_storedSign;
	std::uint32_t lastUpdate;
	std::uint32_t updateInterval;
	bool m_dirty;
	void (*m_executeFunction)(float);

	bool writeText(DisplayDevice* displayDevice, TextAlign textAlign, std::uint8_t x, std::uint8_t y);
};



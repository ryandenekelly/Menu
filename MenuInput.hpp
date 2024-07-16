#pragma once

#include "tim.h"
#include <cstdint>

class MenuInput
{

};

class PushButton : public MenuInput
{
    public:
	PushButton();
	PushButton(std::uint16_t pin, GPIO_TypeDef* port);
	void performAction();
    private:
	std::uint16_t m_pin;
	GPIO_TypeDef* m_port;
};

class Encoder : public MenuInput
{
    public:
	Encoder();
	Encoder(TIM_HandleTypeDef* timerHandler, float res=0.1);

	bool getEncValue(std::uint16_t&);
	bool getEncValue(float&);

	std::uint8_t m_access;
    private:
	TIM_HandleTypeDef* m_timerHandler;
	float m_res;

};

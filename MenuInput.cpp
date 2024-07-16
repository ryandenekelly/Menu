#include "MenuInput.hpp"
#include "math.h"

PushButton::PushButton()
{
    m_pin = 0;
    m_port = nullptr;
}

PushButton::PushButton(std::uint16_t pin, GPIO_TypeDef* port)
{
    m_pin = pin;
    m_port = port;
}

Encoder::Encoder()
{
    m_timerHandler = nullptr;
    m_res = 0.0;
    m_access = 0;
}

Encoder::Encoder(TIM_HandleTypeDef* timerHandler, float res)
{
    m_timerHandler = timerHandler;
    m_res = res;
    m_access = 0;
}

bool Encoder::getEncValue(std::uint16_t& value)
{
    if(m_access>0)
    {
	return false;
    }
    value = m_timerHandler->Instance->CNT;
    return true;
}

bool Encoder::getEncValue(float& value)
{
    if(m_access>0)
    {
	return false;
    }
    value = (std::int16_t)(m_timerHandler->Instance->CNT) * m_res;
    return true;
}



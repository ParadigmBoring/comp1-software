#include "Peripherals/inc/PWMInterface.h"

PWMInterface::PWMInterface(uint64_t clockSpeed,
                           uint32_t preScaler, 
                           uint32_t period, 
                           uint32_t pulse, 
                           TIM_HandleTypeDef* timer, 
                           uint32_t channel) 
    : m_clockSpeed(clockSpeed),
      m_preScaler(preScaler), 
      m_period(period),
      m_pulse(pulse), 
      m_timer(timer), 
      m_channel(channel)
      {}   

void PWMInterface::setPrescaler()
{
    __HAL_TIM_SET_PRESCALER(m_timer, m_preScaler);
}

void PWMInterface::setAutoreload()
{
    __HAL_TIM_SET_AUTORELOAD(m_timer, m_period);
}

void PWMInterface::setCompare()
{
    __HAL_TIM_SET_COMPARE(m_timer, m_channel, m_pulse);
}

void PWMInterface::PWMStart()
{
    HAL_TIM_PWM_Start(m_timer, m_channel);
    setPrescaler();
    setAutoreload();
    setCompare();
}

void PWMInterface::PWMStop()
{
    HAL_TIM_PWM_Stop(m_timer, m_channel);
}

void PWMInterface::setParameters(uint64_t clockspeed,
                                 uint32_t preScaler, 
                                 uint32_t period,
                                 uint32_t pulse, 
                                 TIM_HandleTypeDef* timer, 
                                 uint32_t channel)
{
    clockspeed = m_clockSpeed;
    preScaler = m_preScaler;
    period = m_period;
    pulse = m_pulse;
    timer = m_timer;
    channel = m_channel;
}

void PWMInterface::setPeriod(uint32_t period)
{
    m_period = period;
    setAutoreload();
}

void PWMInterface::setPulse(uint32_t pulse)
{
    m_pulse = pulse;
    setCompare();
}

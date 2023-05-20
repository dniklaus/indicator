/*
 * Indicator.cpp
 *
 *  Created on: 01.11.2019
 *      Author: nid
 */

#include <cstring>
#include <SpinTimer.h>
#include "Indicator.h"

//-----------------------------------------------------------------------------

AIndicatorAdapter::AIndicatorAdapter()
: m_indicator(0)
{ }

void AIndicatorAdapter::attachIndicator(Indicator* indicator)
{
  m_indicator = indicator;
}

Indicator* AIndicatorAdapter::indicator()
{
  return m_indicator;
}

//-----------------------------------------------------------------------------

class BlinkTimerAction : public SpinTimerAction
{
private:
  Indicator* m_indicator;

public:
  BlinkTimerAction(Indicator* indicator)
  : m_indicator(indicator)
  { }

  void timeExpired()
  {
    if (0 != m_indicator)
    {
      m_indicator->toggle();
    }
  }
};

//-----------------------------------------------------------------------------

class PulseTimerAction : public SpinTimerAction
{
private:
  Indicator* m_indicator;
public:
  PulseTimerAction(Indicator* indicator)
  : m_indicator(indicator)
  { }

  void timeExpired()
  {
    if (0 != m_indicator)
    {
      m_indicator->endOfPulse();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned long Indicator::c_blinkTimeMillis = 100;
const unsigned long Indicator::cDefaultPulseTimeMillis = 1000;

Indicator::Indicator(const char* name, const char* description, unsigned long pulseTimeMillis /* = cDefaultPulseTimeMillis */)
: m_adapter(0)
, m_blinkTimer(new SpinTimer(c_blinkTimeMillis, new BlinkTimerAction(this), SpinTimer::IS_RECURRING, SpinTimer::IS_AUTOSTART))
, m_pulseTimer(new SpinTimer(pulseTimeMillis, new PulseTimerAction(this), SpinTimer::IS_NON_RECURRING, SpinTimer::IS_NON_AUTOSTART))
, m_nameSize(strlen(name)+1)
, m_name(new char[m_nameSize])
, m_descriptionSize(strlen(description)+1)
, m_description(new char[m_descriptionSize])
, m_indicatorBit(false)
{ 
  strncpy(m_name, name, m_nameSize);
  strncpy(m_description, description, m_descriptionSize);
}

Indicator::~Indicator()
{
  delete m_blinkTimer->action();
  m_blinkTimer->attachAction(0);

  delete m_blinkTimer;
  m_blinkTimer = 0;
}

void Indicator::assignAdapter(AIndicatorAdapter* adapter)
{
  m_adapter = adapter;
  if (0 != adapter)
  {
    adapter->attachIndicator(this);
    adapter->notifyStatusChange(m_indicatorBit);
  }
}

AIndicatorAdapter* Indicator::adapter()
{
  return m_adapter;
}

const char* Indicator::name() const
{
  return m_name;
}

const char* Indicator::description() const
{
  return m_description;
}

SpinTimer* Indicator::blinkTimer()
{
  return m_blinkTimer;
}

SpinTimer* Indicator::pulseTimer()
{
  return m_pulseTimer;
}

void Indicator::toggle()
{
  m_indicatorBit = !m_indicatorBit;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::set()
{
  m_blinkTimer->cancel();
  m_indicatorBit = true;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::clear()
{
  m_blinkTimer->cancel();
  m_indicatorBit = false;
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChange(m_indicatorBit);
  }
}

void Indicator::blink()
{
  if (!m_blinkTimer->isRunning())
  {
    m_blinkTimer->start();
    toggle();
  }
}

bool Indicator::status()
{
  return m_indicatorBit;
}

void Indicator::startPulse(unsigned long pulseTimeMillis)
{
    if (0 != m_pulseTimer)
    {
      m_pulseTimer->start(pulseTimeMillis);
    }
    set();
}

void Indicator::startPulse()
{
    if (0 != m_pulseTimer)
    {
      m_pulseTimer->start();
    }
    set();
}

void Indicator::endOfPulse()
{
    clear();
}

Indicator::EIndState Indicator::getState()
{
  EIndState state = EIndState::off;
  state = m_blinkTimer->isRunning()  ? EIndState::blink  :
          m_indicatorBit             ? EIndState::on     :
                                       EIndState::off    ;
  return state;
}

const char* Indicator::getStateText(EIndState state)
{
  return  state == EIndState::blink  ? "blink" :
          state == EIndState::on     ? "on"    :
          state == EIndState::off    ? "off"   : "unknown";
}

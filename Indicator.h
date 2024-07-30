/*
 * Indicator.h
 *
 *  Created on: 01.11.2019
 *      Author: nid
 */

#ifndef LIB_INDICATOR_INDICATOR_H_
#define LIB_INDICATOR_INDICATOR_H_

//-----------------------------------------------------------------------------

class Indicator;

/**
 * Abstract Indicator Adapter.
 * Implement the notifyStatusChange(bool status) method to control any specific
 * output pin (or even a virtual object that might be "interested" in the status
 * of the indicator).
 * 
 * Example: To control the built in LED on an Arduino, implement the notifyStatusChange() method as follows:
 * @code{.cpp}
 *       #include <Arduino.h>
 * 
 *       class MyIndicatorAdapter : public AIndicatorAdapter
 *       {
 *         void notifyStatusChange(bool status)
 *         {
 *           digitalWrite(LED_BUILTIN, status);
 *         }
 *       }
 * @endcode
 *
 * In order to assign the adapter implementation to the indicator use
 * @code{.cpp}
 *       #include "MyIndicatorAdapter.h"
 *
 *       Indicator* myIndicator = new Indicator();
 *       myIndicator->assignAdapter(new MyIndicatorAdapter());
 * @endcode
 */
class AIndicatorAdapter
{
protected:
  AIndicatorAdapter();

public:
  virtual ~AIndicatorAdapter() { }

public:
  virtual void notifyStatusChange(bool status) = 0;
  void attachIndicator(Indicator* indicator);

protected:
  Indicator* indicator();

private:
  Indicator* m_indicator;

private:  // forbidden default methods
  AIndicatorAdapter(const AIndicatorAdapter &other) = delete;
  AIndicatorAdapter(AIndicatorAdapter &&other) = delete;
};

//-----------------------------------------------------------------------------

class SpinTimer;

/**
 * Indicator object.
 * This indicator provides a generic way to use output pins to indicate a binary
 * state.
 * A popular use could be controlling the built-in LED or just a pin which could
 * be probed by an oscilloscope for instance.
 *
 * The indicator is blinking by default (@ 2.5 Hz, on & off time is 200ms each).
 */
class Indicator
{
public:
  enum class EIndState
  {
    off   = 0,
    on    = 1,
    blink = 2
  };

public:
  Indicator(const char* name, const char* description, unsigned long pulseTimeMillis = cDefaultPulseTimeMillis);
  virtual ~Indicator();

  void assignAdapter(AIndicatorAdapter* adapter);
  AIndicatorAdapter* adapter();

  const char* name() const;
  const char* description() const;

  SpinTimer* blinkTimer();
  SpinTimer* pulseTimer();

  EIndState getState();
  static const char* getStateText(EIndState state);

  void toggle();
  void set();
  void clear();
  void blink();

  bool status();

  void startPulse(unsigned long pulseTimeMillis);
  void startPulse();
  void endOfPulse();

private:
  AIndicatorAdapter*  m_adapter;
 
  SpinTimer*          m_blinkTimer;
  SpinTimer* 		      m_pulseTimer;
 
  static const unsigned long c_blinkTimeMillis;
  static const unsigned long cDefaultPulseTimeMillis;
 
  const unsigned int m_nameSize;
  char* m_name;

  const unsigned int m_descriptionSize;
  char* m_description;
  bool  m_indicatorBit;

private:  // forbidden default methods
  Indicator() = delete;
  Indicator(const Indicator &other) = delete;
  Indicator(Indicator &&other) = delete;
};

#endif /* LIB_INDICATOR_INDICATOR_H_ */

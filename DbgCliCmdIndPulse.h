/*
 * DbgCliCmdLedSet.h
 *
 *  Created on: 01.11.2019
 *      Author: nid
 */

#ifndef LIB_INDICATOR_DBGCLICMDINDPULSE_H_
#define LIB_INDICATOR_DBGCLICMDINDPULSE_H_

#include <DbgCliCommand.h>

class Indicator;
class DbgTrace_Port;

class DbgCliCmd_IndPulse : public DbgCli_Command
{
public:
  DbgCliCmd_IndPulse(Indicator& indicator);
  virtual ~DbgCliCmd_IndPulse();
  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();
  void printReport();

private:
  DbgTrace_Port*  m_trPort;
  Indicator&      m_indicator;
};

#endif /* LIB_INDICATOR_DBGCLICMDINDPULSE_H_ */

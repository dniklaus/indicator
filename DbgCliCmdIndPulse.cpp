/*
 * DbgCliCmdIndPulse.cpp
 *
 *  Created on: 21.01.2021
 *      Author: Dieter Niklaus
 */

#include "DbgCliCmdIndPulse.h"

#include <string.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <SpinTimer.h>
#include "Indicator.h"

DbgCliCmd_IndPulse::DbgCliCmd_IndPulse(Indicator& indicator)
: DbgCli_Command(indicator.dbgCliTopic(), "pulse", "Send a pulse on indicator output")
, m_trPort(new DbgTrace_Port(this->getParentNode()->getNodeName(), DbgTrace_Level::notice))
, m_indicator(indicator)
{ }

DbgCliCmd_IndPulse::~DbgCliCmd_IndPulse()
{
  delete m_trPort;
  m_trPort = 0;
}

void DbgCliCmd_IndPulse::printUsage()
{
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "%s (default length: %u [ms])", getHelpText(), m_indicator.pulseTimer()->getInterval());
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "Usage: %s %s %s [pulseLengthMillis (>0)]", 
            DbgCli_Node::RootNode()->getNodeName(), this->getParentNode()->getNodeName(), this->getNodeName());
}

void DbgCliCmd_IndPulse::printReport()
{
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "%s : %u [ms]", getHelpText(), m_indicator.pulseTimer()->getInterval());
}

void DbgCliCmd_IndPulse::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle == 0)
  {
    m_indicator.startPulse();
    printReport();
  }
  else
  {
    long pulseLength = atol(args[idxToFirstArgToHandle]);

    if (pulseLength > 0)
    {
      m_indicator.startPulse(static_cast<unsigned long>(pulseLength));
      printReport();
    }
    else
    {
      printUsage();
    }
  }
}

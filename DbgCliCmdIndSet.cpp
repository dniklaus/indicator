/*
 * DbgCliCmdLedSet.cpp
 *
 *  Created on: 01.11.2019
 *      Author: nid
 */

#include "DbgCliCmdIndSet.h"

#include <string.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include "Indicator.h"

DbgCliCmd_IndSet::DbgCliCmd_IndSet(Indicator& indicator)
: DbgCli_Command(indicator.dbgCliTopic(), "set", "Set indicator state")
, m_trPort(new DbgTrace_Port(this->getParentNode()->getNodeName(), DbgTrace_Level::notice))
, m_indicator(indicator)
{ }

DbgCliCmd_IndSet::~DbgCliCmd_IndSet()
{
  delete m_trPort;
  m_trPort = 0;
}

void DbgCliCmd_IndSet::printUsage()
{
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "%s", getHelpText());
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "Usage: %s %s %s [blink|on|off].", 
            DbgCli_Node::RootNode()->getNodeName(), this->getParentNode()->getNodeName(),this->getNodeName());
}

void DbgCliCmd_IndSet::printReport()
{
  TR_PRINTF(m_trPort, DbgTrace_Level::alert, "%s : %s", getHelpText(), Indicator::getStateText(m_indicator.getState()));
}

void DbgCliCmd_IndSet::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle == 0)
  {
    printReport();
  }
  else
  {
    if (strncmp(args[idxToFirstArgToHandle], "on", strlen("on")) == 0)
    {
      m_indicator.set();
      printReport();
    }
    else if (strncmp(args[idxToFirstArgToHandle], "off", strlen("off")) == 0)
    {
      m_indicator.clear();
      printReport();
    }
    else if (strncmp(args[idxToFirstArgToHandle], "blink", strlen("blink")) == 0)
    {
      m_indicator.blink();
      printReport();
    }
    else
    {
      printUsage();
    }
  }
}

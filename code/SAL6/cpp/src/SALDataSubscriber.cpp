
/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2013 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE
 *
 *   for full copyright notice and license terms.
 *
 */

/*
 * This file contains the implementation for the 'SALDataSubscriber' executable.
 *
 ***/

// Control optional compilation 
#define USE_SAL_WRAPPER
#define USE_SAL_COMMAND

#include <string>
#include <sstream>
#include <iostream>
#include "SALDDS.h"
#include "ccpp_SALData.h"
#include "os.h"

#include "example_main.h"

using namespace DDS;
using namespace SALData;

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
  OS_API_EXPORT
  int SALDataSubscriber(int argc, char *argv[]);
}

int SALDataSubscriber(int argc, char *argv[])
{
  os_time delay_2ms = { 0, 2000000 };
  os_time delay_200ms = { 0, 200000000 };
  SALTopicSeq SALTopicList;
  SampleInfoSeq infoSeq;

  SALDDS mgr;

  // create domain participant
  mgr.createParticipant("SAL LSST");

  //create type
  SALTopicTypeSupport_var mt = new SALTopicTypeSupport();
  mgr.registerType(mt.in());

  //create Topic
  char topic_name[] = "SALData_SALTopic";
  mgr.createTopic(topic_name);

  //create Subscriber
  mgr.createSubscriber();

  // create DataReader
  mgr.createReader();

  DataReader_var dreader = mgr.getReader();
  SALTopicDataReader_var SALReader = SALTopicDataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), "SALTopicDataReader::_narrow");

  cout << "=== [Subscriber] Ready ..." << endl;

  bool closed = false;
  ReturnCode_t status =  - 1;
  int count = 0;
  while (!closed && count < 1500) // We dont want the example to run indefinitely
  {
    status = SALReader->take(SALTopicList, infoSeq, LENGTH_UNLIMITED,
      ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
    checkStatus(status, "SALTopicDataReader::take");
    for (DDS::ULong j = 0; j < SALTopicList.length(); j++)
    {
      cout << "=== [Subscriber] message received :" << endl;
      cout << "    revCode  : " << SALTopicList[j].private_revCode << endl;
    }
    status = SALReader->return_loan(SALTopicList, infoSeq);
    checkStatus(status, "SALTopicDataReader::return_loan");
    os_nanoSleep(delay_200ms);
    ++count;
  }

  os_nanoSleep(delay_2ms);

  //cleanup
  mgr.deleteReader();
  mgr.deleteSubscriber();
  mgr.deleteTopic();
  mgr.deleteParticipant();

  return 0;
}

int SALTelemetrySubscriber(char *topic_name)
{
  os_time delay_2ms = { 0, 2000000 };
  os_time delay_200ms = { 0, 200000000 };
  SALTopicSeq SALInstance;

  SALDDS mgr;

  mgr.salInitialize();
//  DataReader_var dreader = mgr.salTelemetrySub(topic_name);
  mgr.salTelemetrySub(topic_name);

  cout << "=== [Subscriber] Ready ..." << endl;

  bool closed = false;
  ReturnCode_t status =  - 1;
  int count = 0;
  while (!closed && count < 1500) // We dont want the example to run indefinitely
  {
    status = mgr.getSample(SALInstance);
    os_nanoSleep(delay_200ms);
    ++count;
  }

  os_nanoSleep(delay_2ms);

  //cleanup
  mgr.salShutdown();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
#ifdef USE_SAL_WRAPPER
  return SALTelemetrySubscriber("SALData_SALTopic");
#else
  return SALDataSubscriber (argc, argv);
#endif
}



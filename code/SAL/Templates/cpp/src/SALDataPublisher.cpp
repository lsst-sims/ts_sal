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
 * This file contains the implementation for the 'SALDataPublisher' executable.
 *
 ***/

// Control optional compilation 
#define USE_SAL_WRAPPER
//#define USE_SAL_COMMAND

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
  int SALDataPublisher(int argc, char *argv[]);
}

int SALTelemetryPublisher(char *topic_name)
{
  os_time delay_1s = { 1, 0 };
  SALDDS mgr;

  // Publish Events
  //create Topic
  mgr.salInitialize();
  DataWriter_var dwriter = mgr.salTelemetryPub(topic_name);
  SALTopic SALTopicInstance; /* Example on Stack */
  SALTopicInstance.private_revCode = DDS::string_dup("LSST TEST REVCODE");
  while (1) {
    mgr.putSample(SALTopicInstance);
    os_nanoSleep(delay_1s);
  }

  /* Remove the DataWriters etc */
  mgr.salShutdown();

  return 0;
}

int SALCommander(char *topic_name, char *device, char *property, char *action, char *value, char *modifiers)
{
  salReturn status = SAL__OK;
  salUINT cmdId;

  os_time delay_1s = { 1, 0 };
  SALDDS cmd;

  // Publish Events
  //create Topic
  cmd.salInitialize();
  cmd.salCommand(topic_name);

  SALCommand command; /* Example on Stack */
  command.private_revCode = DDS::string_dup("LSST TEST COMMAND");\
  command.device = device;
  command.property = property;
  command.action = action;
  command.value = value;
  command.modifiers = modifiers;
  cmdId = cmd.issueCommand(command);

  os_nanoSleep(delay_1s);
  status = cmd.waitForCompletion(cmdId, 10);

  /* Remove the DataWriters etc */
  cmd.salShutdown();

  return 0;
}


int SALDataPublisher(int argc, char *argv[])
{
  os_time delay_1s = { 1, 0 };
  SALDDS mgr;
  char topic_name[] = "SALData_SALTopic";

  // create Telemetry publisher
  mgr.createParticipant("SAL LSST");

  //create type
  SALTopicTypeSupport_var mt = new SALTopicTypeSupport();
  mgr.registerType(mt.in());

  //create Topic
  mgr.createTopic(topic_name);

  //create Publisher
  mgr.createPublisher();

  // create DataWriter :
  // If autodispose_unregistered_instances is set to true (default value),
  // you will have to start the subscriber before the publisher
  bool autodispose_unregistered_instances = false;
  mgr.createWriter(autodispose_unregistered_instances);

  // Publish Events
  DataWriter_var dwriter = mgr.getWriter();
  SALTopicDataWriter_var SALWriter = SALTopicDataWriter::_narrow(dwriter.in());

  SALTopic SALTopicInstance; /* Example on Stack */
  SALTopicInstance.private_revCode = DDS::string_dup("LSST TEST REVCODE");
  cout << "=== [Publisher] writing a message containing :" << endl;
  cout << "    revCode  : " << SALTopicInstance.private_revCode << endl;

  ReturnCode_t status = SALWriter->write(SALTopicInstance, DDS::HANDLE_NIL);
  checkStatus(status, "SALTopicDataWriter::write");
  while (1) {
    mgr.putSample(SALTopicInstance);
    os_nanoSleep(delay_1s);
  }

  /* Remove the DataWriters */
  mgr.deleteWriter();

  /* Remove the Publisher. */
  mgr.deletePublisher();

  /* Remove the Topics. */
  mgr.deleteTopic();

  /* Remove Participant. */
  mgr.deleteParticipant();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
#ifdef USE_SAL_WRAPPER
#ifdef USE_SAL_COMMAND
  return SALCommander ("SALData_SALCommand", argv[1], argv[2], argv[3], argv[4], argv[5]);
#else
  return SALTelemetryPublisher ("SALData_SALTopic");
#endif
#else
  return SALDataPublisher (argc, argv);
#endif
}



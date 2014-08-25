
//#include <chrono>
#include "SALDDS.h"
using namespace SALData;

void SALDDS::salInitialize()
{
   hasReader = false;
   hasWriter = false;
   debugLevel = 1;
}

// INSERT TYPE SUPPORT      
void SALDDS::salTypeSupport(char *topicName) 
{
    if (strncmp("SALData",topicName,7) == 0) {
       if ( strcmp("SALData_SALTopic",topicName) == 0) {
	  SALTopicTypeSupport_var mt = new SALTopicTypeSupport();
	  registerType(mt.in());
       }
    }
}
salReturn SALDDS::putSample(SALData::SALTopic data)
{
  DataWriter_var dwriter = getWriter();
  SALTopicDataWriter_var SALWriter = SALTopicDataWriter::_narrow(dwriter.in());
  data.private_revCode = DDS::string_dup("LSST TEST REVCODE");
  if (debugLevel > 0) {
    cout << "=== [putSample] writing a message containing :" << endl;
    cout << "    revCode  : " << data.private_revCode << endl;
  }
  ReturnCode_t status = SALWriter->write(data, DDS::HANDLE_NIL);
  checkStatus(status, "SALTopicDataWriter::write");  
  return status;
}

salReturn SALDDS::getSample(SALData::SALTopicSeq data)
{
  SampleInfoSeq infoSeq;
  ReturnCode_t status =  - 1;
  DataReader_var dreader = getReader();
  SALTopicDataReader_var SALReader = SALTopicDataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), "SALTopicDataReader::_narrow");
  status = SALReader->take(data, infoSeq, LENGTH_UNLIMITED, ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  checkStatus(status, "SALTopicDataReader::take");
  for (DDS::ULong j = 0; j < data.length(); j++)
  {
    cout << "=== [GetSample] message received :" << endl;
    cout << "    revCode  : " << data[j].private_revCode << endl;
  }
  status = SALReader->return_loan(data, infoSeq);
  checkStatus(status, "SALTopicDataReader::return_loan");
  return status;
}


// END OF INSERT TYPE SUPPORT      

DataWriter_var SALDDS::salTelemetryPub(char *topicName)
{
  // create domain participant
  createParticipant("SAL LSST");

  //create type
  salTypeSupport(topicName);

  //create Topic
  createTopic(topicName);

  //create Publisher
  createPublisher();

  // create DataWriter :
  // If autodispose_unregistered_instances is set to true (default value),
  // you will have to start the subscriber before the publisher
  bool autodispose_unregistered_instances = false;
  createWriter(autodispose_unregistered_instances);
  DataWriter_var dwriter = getWriter();
  hasWriter = true;
  return DataWriter::_duplicate(dwriter.in());

}

//DataReader_var  SALDDS::salTelemetrySub(char *topicName)
void SALDDS::salTelemetrySub(char *topicName)
{
  // create domain participant
  createParticipant("SAL LSST");

  //create type
  salTypeSupport(topicName);

  //create Topic
  createTopic(topicName);

  //create Subscriber
  createSubscriber();

 // create DataReader
  createReader();

//  DataReader_var dreader = getReader();
//  hasReader = true;
//  return DataReader::_duplicate(dreader.in());

}
       
void SALDDS::logError(salReturn status) 
{
   cout << "=== ERROR return value = " << status << endl; 
}

/*
using Clock = std::chrono::high_resolution_clock;
using Time_point = Clock::time_point;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

salTIME SALDDS::currentTime()
{
    Time_point tp = Clock::now();   
    return (salTIME duration_cast<milliseconds>tp.count());
}
*/

salUINT SALDDS::issueCommand( SALData::SALCommand data )
{
  // create DataWriter :
  // If autodispose_unregistered_instances is set to true (default value),
  // you will have to start the subscriber before the publisher
  bool autodispose_unregistered_instances = false;
  if (!hasWriter) {
     createWriter(autodispose_unregistered_instances);
     hasWriter = true;
     sndSeqNum = 1;
  }
  DataWriter_var dwriter = getWriter();
  SALCommandDataWriter_var SALWriter = SALCommandDataWriter::_narrow(dwriter.in());
  data.private_revCode = DDS::string_dup("LSST TEST COMMAND");
  data.private_seqNum = sndSeqNum;
  if (debugLevel > 0) {
    cout << "=== [issueCommand] writing a command containing :" << endl;
    cout << " " << data.device << "." << data.property << "." << data.action << " : " << data.value << endl;
  }
  ReturnCode_t status = SALWriter->write(data, DDS::HANDLE_NIL);
  sndSeqNum++;
  checkStatus(status, "SALCommandDataWriter::write");  
  if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
  }
  return (sndSeqNum-1);
}


salReturn SALDDS::checkCommand( salUINT cmdSeqNum )
{
   salReturn status = SAL__OK;
   SALResponse response;
   status = getResponse(response);
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salReturn SALDDS::getResponse(SALData::SALResponse data)
{
  SampleInfo info;
  ReturnCode_t status =  - 1;
  DataReader_var dreader = getReader();
  SALResponseDataReader_var SALReader = SALResponseDataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), "SALResponseDataReader::_narrow");
  status = SALReader->take_next_sample(data, info);
//, LENGTH_UNLIMITED,ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  checkStatus(status, "SALResponseDataReader::take");
  if (debugLevel > 0) {
    cout << "=== [getResponse] reading a message containing :" << endl;
    cout << "    revCode  : " << data.private_seqNum << endl;
  }
  status = data.ack;
  rcvSeqNum = data.private_seqNum;
//  status = SALReader->return_loan(data, info);
//  checkStatus(status, "SALResponseDataReader::return_loan");
  return status;
}


salReturn SALDDS::cancelCommand( salUINT cmdSeqNum )
{
   salReturn status = SAL__OK;
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salReturn SALDDS::abortCommand( salUINT cmdSeqNum )
{
   salReturn status = SAL__OK;
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salReturn SALDDS::waitForCompletion( salUINT cmdSeqNum , salINT timeout )
{
   salReturn status = SAL__OK;
   salUINT countdown = timeout;
   SALResponse response;

   while (status != SAL__CMD_COMPLETE && countdown > 0) {
      status = getResponse(response);
      if (response.private_seqNum != cmdSeqNum) { 
         status = SAL__CMD_NOACK;
      }
      usleep(SAL__SLOWPOLL);
      countdown--;
   }
   if (status != SAL__CMD_COMPLETE) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salReturn SALDDS::setDebugLevel( salUINT level )
{
   debugLevel = level;
   salReturn status = SAL__OK;
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salUINT SALDDS::getDebugLevel( salUINT level )
{
   return debugLevel;
}

salUINT SALDDS::getOrigin()
{
   salReturn status = SAL__OK;
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salUINT SALDDS::getProperty(salCHAR *property, salCHAR *value)
{
   salReturn status = SAL__OK;
   strcpy(value,"UNKNOWN");
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}

salReturn SALDDS::setProperty(salCHAR *property, salCHAR *value)
{
   if (status != SAL__OK) {
      if (debugLevel >= SAL__LOG_ROUTINES) {
          logError(status);
      }
   }
   return status;
}
 
/* 
template <class T1, class T2>
void SALDDS::putSample(T1 data, T2 SALWriter)
{
  T1 SALInstance = data;
  SALInstance.private_revCode = DDS::string_dup("LSST TEST REVCODE");
  cout << "=== [putSample] writing a message containing :" << endl;
  cout << "    revCode  : " << SALInstance.private_revCode << endl;

  ReturnCode_t status = SALWriter->write(SALInstance, DDS::HANDLE_NIL);
  checkStatus(status, "SALTopicDataWriter::write");  
}

template <class T1, class T2, class T3>
void SALDDS::getSample(T1 data, T2 info, T3 SALReader)
{
  ReturnCode_t status =  - 1;
  status = SALReader->take(data, info, LENGTH_UNLIMITED,ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  checkStatus(status, "SALTopicDataReader::take");
  cout << "=== [getSample] reading a message containing :" << endl;
  cout << "    revCode  : " << data.private_revCode << endl;
  status = SALReader->return_loan(data, info);
  checkStatus(status, "SALTopicDataReader::return_loan");
}
*/

salReturn SALDDS::getEvent(SALData::SALEvent data)
{
  SampleInfo info;
  ReturnCode_t status =  - 1;
  DataReader_var dreader = getReader();
  SALEventDataReader_var SALReader = SALEventDataReader::_narrow(dreader.in());
  checkHandle(SALReader.in(), "SALEventDataReader::_narrow");
  status = SALReader->take_next_sample(data, info);
//, LENGTH_UNLIMITED,ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
  checkStatus(status, "SALEventDataReader::take");
  if (debugLevel > 0) {
    cout << "=== [getEvent] reading a message containing :" << endl;
    cout << "    revCode  : " << data.private_revCode << endl;
  }
//  status = SALReader->return_loan(data, info);
//  checkStatus(status, "SALEventDataReader::return_loan");
  return status;
}


void SALDDS::salCommand(char *topicName)
{
  // create domain participant
  createParticipant("SAL LSST");

  //create type
  SALCommandTypeSupport_var mt = new SALCommandTypeSupport();
  registerType(mt.in());

  //create Topic
  createTopic(topicName);
}


void SALDDS::salEvent(char *topicName)
{
  // create domain participant
  createParticipant("SAL LSST");

  //create type
  SALEventTypeSupport_var mt = new SALEventTypeSupport();
  registerType(mt.in());

  //create Topic
  createTopic(topicName);
}

void SALDDS::salShutdown()
{
  if ( hasReader ) { 
     deleteReader();
     deleteSubscriber();     
  }
  if ( hasWriter ) {
     deleteWriter();
     deletePublisher();
  }
  deleteTopic();
  deleteParticipant();
}

void SALDDS::createParticipant(const char *partitiontName)
{
  domain = DOMAIN_ID_DEFAULT;
  dpf = DomainParticipantFactory::get_instance();
  checkHandle(dpf.in(), "DDS::DomainParticipantFactory::get_instance");
  participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT, NULL,
    STATUS_MASK_NONE);
  checkHandle(participant.in(),
    "DDS::DomainParticipantFactory::create_participant");
  partition = partitiontName;
}

void SALDDS::deleteParticipant()
{
    status = dpf->delete_participant(participant.in());
     checkStatus(status, "DDS::DomainParticipant::delete_participant ");
}

void SALDDS::registerType(TypeSupport *ts)
{
  typeName = ts->get_type_name();
  status = ts->register_type(participant.in(), typeName);
  checkStatus(status, "register_type");
}

void SALDDS::createTopic(char *topicName)
{
  status = participant->get_default_topic_qos(reliable_topic_qos);
  checkStatus(status, "DDS::DomainParticipant::get_default_topic_qos");
  reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  reliable_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;

  /* Make the tailored QoS the new default. */
  status = participant->set_default_topic_qos(reliable_topic_qos);
  checkStatus(status, "DDS::DomainParticipant::set_default_topic_qos");

  /* Use the changed policy when defining the HelloWorld topic */
  topic = participant->create_topic(topicName, typeName, reliable_topic_qos,
    NULL, STATUS_MASK_NONE);
  checkHandle(topic.in(), "DDS::DomainParticipant::create_topic ()");
}

void SALDDS::deleteTopic()
{
  status = participant->delete_topic(topic);
  checkStatus(status, "DDS.DomainParticipant.delete_topic");
}

void SALDDS::createPublisher()
{
  status = participant->get_default_publisher_qos(pub_qos);
  checkStatus(status, "DDS::DomainParticipant::get_default_publisher_qos");
  pub_qos.partition.name.length(1);
  pub_qos.partition.name[0] = partition;

  publisher = participant->create_publisher(pub_qos, NULL, STATUS_MASK_NONE);
  checkHandle(publisher.in(), "DDS::DomainParticipant::create_publisher");
}

void SALDDS::deletePublisher()
{
    status = participant->delete_publisher(publisher.in());
    checkStatus(status, "DDS::DomainParticipant::delete_publisher ");
}


void SALDDS::createWriter()
{
  writer = publisher->create_datawriter(topic.in(),
    DATAWRITER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);
  checkHandle(writer, "DDS::Publisher::create_datawriter");
}

void SALDDS::createWriter(bool autodispose_unregistered_instances)
{
  status = publisher->get_default_datawriter_qos(dw_qos);
  checkStatus(status, "DDS::DomainParticipant::get_default_publisher_qos");
  status = publisher->copy_from_topic_qos(dw_qos, reliable_topic_qos);
  checkStatus(status, "DDS::Publisher::copy_from_topic_qos");
  // Set autodispose to false so that you can start
  // the subscriber after the publisher
  dw_qos.writer_data_lifecycle.autodispose_unregistered_instances =
    autodispose_unregistered_instances;
  writer = publisher->create_datawriter(topic.in(), dw_qos, NULL,
    STATUS_MASK_NONE);
  checkHandle(writer, "DDS::Publisher::create_datawriter");
}

void SALDDS::deleteWriter()
{
    status = publisher->delete_datawriter(writer);
    checkStatus(status, "DDS::Publisher::delete_datawriter ");
}

void SALDDS::createSubscriber()
{
  int status = participant->get_default_subscriber_qos(sub_qos);
  checkStatus(status, "DDS::DomainParticipant::get_default_subscriber_qos");
  sub_qos.partition.name.length(1);
  sub_qos.partition.name[0] = partition;
  subscriber = participant->create_subscriber(sub_qos, NULL, STATUS_MASK_NONE);
  checkHandle(subscriber.in(), "DDS::DomainParticipant::create_subscriber");
}

void SALDDS::deleteSubscriber()
{
  status = participant->delete_subscriber(subscriber);
  checkStatus(status, "DDS::DomainParticipant::delete_subscriber ");
}

void SALDDS::createReader()
{
  reader = subscriber->create_datareader(topic.in(),
    DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);
  checkHandle(reader, "DDS::Subscriber::create_datareader ()");
}

void SALDDS::deleteReader()
{
    status = subscriber->delete_datareader(reader);
    checkStatus(status, "DDS::Subscriber::delete_datareader ");
}

DataReader_ptr SALDDS::getReader()
{
  return DataReader::_duplicate(reader.in());
}

DataWriter_ptr SALDDS::getWriter()
{
  return DataWriter::_duplicate(writer.in());
}

Publisher_ptr SALDDS::getPublisher()
{
  return Publisher::_duplicate(publisher.in());
}

Subscriber_ptr SALDDS::getSubscriber()
{
  return Subscriber::_duplicate(subscriber.in());
}

Topic_ptr SALDDS::getTopic()
{
  return Topic::_duplicate(topic.in());
}

DomainParticipant_ptr SALDDS::getParticipant()
{
  return DomainParticipant::_duplicate(participant.in());
}

SALDDS::~SALDDS(){

}

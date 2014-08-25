
#ifndef _SALDDSMGR_
  #define _SALDDSMGR_

  #include <string>
  #include "SAL_defines.h"
  #include "ccpp_dds_dcps.h"
  #include "ccpp_SALData.h"
  #include "CheckStatus.h"
  using namespace DDS;

  class SALDDS
  {

      /* Generic DDS entities */
      DomainParticipantFactory_var dpf;
      DomainParticipant_var participant;
      Topic_var topic;
      Publisher_var publisher;
      Subscriber_var subscriber;
      DataWriter_var writer;
      DataReader_var reader;

      /* QosPolicy holders */
      TopicQos reliable_topic_qos;
      TopicQos setting_topic_qos;
      PublisherQos pub_qos;
      DataWriterQos dw_qos;
      SubscriberQos sub_qos;

      DomainId_t domain;
      ReturnCode_t status;

      DDS::String_var partition;
      DDS::String_var typeName;
      bool hasReader;
      bool hasWriter;
      salUINT debugLevel;
      salUINT sndSeqNum;
      salUINT rcvSeqNum;

    public:
      void salTypeSupport(char *topicName);
      DataWriter_var salTelemetryPub(char *topicName);
//      DataReader_var salTelemetrySub(char *topicName);
      void salTelemetrySub(char *topicName);
      void salCommand(char *topicName);
      void salEvent(char *topicName);
//      template <class T1, class T2>
//      void putSample(T1 data, T2 writer);
//      template <class T1, class T2, class T3>
//      void getSample(T1 data, T2 info, T3 reader);
      salReturn putSample(SALData::SALTopic data);
      salReturn getSample(SALData::SALTopicSeq data);
      salReturn getResponse(SALData::SALResponse data);
      salReturn getEvent(SALData::SALEvent data);
      void salShutdown();
      void salInitialize();
      salUINT issueCommand( SALData::SALCommand data);
      salReturn checkCommand( salUINT cmdSeqNum );
      salReturn cancelCommand( salUINT cmdSeqNum );
      salReturn abortCommand( salUINT cmdSeqNum );
      salReturn waitForCompletion( salUINT cmdSeqNum , salINT timeout );
      salReturn setDebugLevel( salUINT level );
      salUINT getDebugLevel( salUINT level );
      salUINT getOrigin();
      salUINT getProperty(salCHAR *property, salCHAR *value);
      salReturn setProperty(salCHAR *property, salCHAR *value);
      void logError(salReturn status);
      salTIME currentTime();
      
      void createParticipant(const char *partitiontName);
      void deleteParticipant();
      void registerType(TypeSupport *ts);
      void createTopic(char *topicName);
      void deleteTopic();
      void createPublisher();
      void deletePublisher();
      void createWriter();
      void createWriter(bool autodispose_unregistered_instances);
      void deleteWriter();
      void createSubscriber();
      void deleteSubscriber();
      void createReader();
      void deleteReader();
      DataReader_ptr getReader();
      DataWriter_ptr getWriter();
      Publisher_ptr getPublisher();
      Subscriber_ptr getSubscriber();
      Topic_ptr getTopic();
      DomainParticipant_ptr getParticipant();
      ~SALDDS();
  };

#endif

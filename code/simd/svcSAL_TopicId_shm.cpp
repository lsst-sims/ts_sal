// Generic code for simd interface, replace TopicId
// with actual topic name, then process through INSERT_PERTOPIC
// to add data exchangers

// -- STD C++ Include
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
using namespace std;

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// -- SIMD Include
#include <dds/dds.hpp>

// -- TopicId Include
#include <gen/ccpp_TopicId.h>

REGISTER_TOPIC_TRAITS(TopicId);
std::string topic("TopicIdTopic");

// -- SAL Include
#include "svcSAL_TopicId.h"
#include "shmem_TopicId.h"

void salTopicId::GetObject((TopicId_cache *) pshm_TopicId) {
       *pshm_TopicId = (TopicId_cache*)shm_TopicId::m_shared_mem;
}


salTopicId::salTopicId()
{
     char *env;
     string PubOP("publish");
     
	currentInstance= NULL;
        newData = SAL__NO_UPDATES;
        env = (char *) (getenv("SAL_DEBUG_LEVEL"));
	if ( env != NULL ) {
           sscanf(env, "%d", &debugLevel);
	}
	debugLevel = 0;
        timeOfSnd = 0.0;
        timeOfRcv = 0.0;
        haveQos = 0;
	haveDur = 0;
	haveRdr = 0;
	haveWrt = 0;
	priority = 0;
	pid = 0;
	readCount = 0;
	writeCount = 0;
//	latency_budget = {1,0);
	deadline = {1,0};
	pid = getpid();
	dds::Runtime runtime();
	tqos.set_reliable();
	tqos.set_transient();
//	tqos.set_latency_budget(latency_budget);
	tqos.set_deadline(deadline);
	tqos.set_priority(priority);
        tqos.set_keep_last(SAL__DEFAULT_HISTORY_DEPTH);
        dds::Topic<TopicId> TopicIdTopic(topic);
        if (operation.compare(PubOP) == 0) {
           dds::DataWriter<TopicId> writer(TopicIdTopic,wQos);
           haveWrt = TRUE;
        } else {
           dds::DataReader<TopicId> reader;
           haveRdr = TRUE;
        }
}

 
salTopicId::~salTopicId()
{
}



svcRTN salTopicId::publisher ()
{
   if ( !haveWrt ) {
      dds::Topic<TopicId> TopicIdTopic(topic);
      dds::DataWriter<TopicId> writer(TopicIdTopic);
      haveWrt = TRUE;
   }
   return SAL__OK;
}

svcRTN salTopicId::subscriber ()
{
   if ( !haveRdr ) {
      dds::Topic<TopicId> TopicIdTopic(topic);
      dds::DataReader<TopicId> reader(TopicIdTopic);
      haveRdr = TRUE;
   }
   return SAL__OK;
}


svcRTN salTopicId::getSample (svcINT timeout)
{
    svcRTN result;
    svcINT remaining;
   
   remaining = timeout+1;
    while (remaining >= 0) {
      result = salTopicId::read(dataSeq,sinfoSeq,dum1,dum2,dum3,dum4);
      if ( result != DDS::RETCODE_OK ) {
         if ( result != DDS::RETCODE_TIMEOUT &&
	      result != DDS::RETCODE_NO_DATA ) {
            if (remaining < 0) {
	       return result;
	    }
         }
      } else {
         data.private_rcvStamp = currentTime();
	 timeOfRcv = data.private_rcvStamp;
	 readCount++;
	 if (timeout == SAL__WAIT_FOR_CHANGE) {
	 } else { 
	    return SAL__OK;
	 }
      }
      remaining = remaining - 1000;
      usleep(SAL__FASTPOLL);
    }
    return SAL__TIMEOUT;
}

svcRTN salTopicId::putSample () {
    svcRTN result;
    data.private_sndStamp = currentTime();
    timeOfSnd = data.private_rcvStamp;
    data.private_origin = pid;
    result = salTopicId::write(data,currentInstance);
    writeCount++;
    return SAL__OK;
}

svcRTN salTopicId::getProperty ( svcCHAR *propertyName , svcCHAR *textValue)
{
    return SAL__OK;
}
	
svcRTN salTopicId::setProperty ( svcCHAR *propertyName , svcCHAR *textValue)
{
    return SAL__OK;
}
	
DDS::Boolean salTopicId::_local_is_a (const char * _id)
{
   if (strcmp (_id, salTopicId::_local_id) == 0) return true;

   typedef salTopicId NestedBase_1;

   if (NestedBase_1::_local_is_a (_id)) return true;

   return false;
}


svcRTN TopicId::shmMapper()
{
       key_t key = SAL_SHMEM_KEY_TopicId;
       size_t TopicId_shmsize = sizeof(TopicId_cache);
       bool bCreated = false;

       m_TopicId_shmid = shmget(key, TopicId_shmsize, 0666);

       if (-1 == m_TopicId_shmid) {

           if (ENOENT != errno) {
               cerr << __FILE__ <<  ":" <<  __LINE__ <<  " Critical error in shmget"  << endl;
               return SAL_ERR;
           }

           m_TopicId_shmid = shmget(key, TopicId_shmsize, IPC_CREAT | 0666);

           if (-1 == m_TopicId_shmid) {
               cerr <<  __FILE__ <<  ":" <<  __LINE__ <<  " Critical error in shmget" << endl;
               return SAL__ERR;
           }

           cout <<   "Created the shared memory" <<  endl;
           bCreated = true;
       }

       m_shared_mem = shmat(m_TopicId_shmid, NULL, 0);

       if (-1 == (int)shm_TopicId::m_shared_mem) {
           cerr <<  __FILE__ <<  ":" <<  __LINE__ <<  " Critical error in shmat" << endl;
           return SAL__ERR;
       }

       if (bCreated) {
           // Construct objects on the shared memory
           shmdata = (TopicId_cache *)m_shared_mem;
           shmdata->syncO = 0;
       }

       return SAL__OK;
}

void salTopicId::shmInfo() {
       cout <<  "Object type: shm_TopicId" <<  endl;
}


// INSERT PERTOPIC_METHODS



#ifndef ORG_LSST_SAL_SALRESPONSE_READER_HPP_
#define ORG_LSST_SAL_SALRESPONSE_READER_HPP_

#include "response_util.hpp"
#include <unistd.h>
#include <vector>
#include <algorithm>

namespace sal {
   namespace salresponse {
      template<typename T>
      class Reader;
   }
}
/**
 * This class shows a sample use of the (forward) iterator-based read API.
 * Beware that a back-inserting iterator API also exist.
 *
 */
template<typename T>
class sal::salresponse::Reader: public sal::salresponse::Runner<T> {
public:
   virtual ~Reader() {
   }

public:
   virtual void run(const dds::domain::DomainParticipant& dp,
         const dds::topic::Topic<T>& topic, const Params& params)
   {
      dds::sub::qos::SubscriberQos sqos =
            dp.default_subscriber_qos() << Partition("salresponse");

      Subscriber sub(dp, sqos);

      dds::sub::qos::DataReaderQos drqos =
            sub.default_datareader_qos()
            << History::KeepLast(params.history_depth)
            << Durability::Transient();

      dds::sub::DataReader<T> dr(sub, topic, drqos);

      const uint32_t period = params.period;
      const uint32_t samples = params.samples;
      const uint32_t max_samples = 16;
      const uint32_t sleep_time = period * 1000;

      std::vector< Sample<SALTopic_responseType> > data(max_samples);


      // AnyTopic work fine
      AnyTopic at = topic;
      dds::topic::Topic<SALTopic_responseType> xt = at.get<SALTopic_responseType>();

      // QosProvider...
      QosProvider qos_provider(
            "http://www.opensplice.org/demo/config/qos.xml",
            "salresponse-profile");

      DataReader<SALTopic_responseType> dr(sub, topic, qos_provider.datareader_qos());

      // Query
      Query q(dr);

      // AnyDataReader
      AnyDataReader adr = q.data_reader();
      DataReader<SALTopic_responseType> xdr = adr.get<SALTopic_responseType>();

      // Conditions
      dds::sub::cond::ReadCondition rc(dr, params.data_state);

      Entity e = dr;
      StatusCondition sc(dr);
      sc.enabled_statuses(StatusMask::data_available());

      Subscriber xsub = dr.subscriber();

      WaitSet ws;
      ws
         .attach_condition(sc)
         .attach_condition(rc);

      LoanedSamples<SALTopic_responseType> loaned_samples = dr.read();

      for (uint32_t i = 0; i < samples; ++i) {
         int32_t rs =
               dr.selector()
                  .filter_state(params.data_state)
                  .read(data.begin(), max_samples);

         std::cout << "==== Read  ==== \n";
         std::for_each(data.begin(), data.begin() + rs, printSALTopicSample);
         std::cout << std::endl;
         usleep(sleep_time);

         LoanedSamples<SALTopic_responseType> loaned_samples =
               dr.selector()
                        .filter_state(params.data_state)
                        .read();

         std::cout << "==== Loaned Samples Read ==== \n";
         std::for_each(loaned_samples.begin(), loaned_samples.end(), printSALTopicSample);
         std::cout << std::endl;
         usleep(sleep_time);
      }
   }
};

#endif /* ORG_LSST_SAL_SALRESPONSE_READER_HPP_ */

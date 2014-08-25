#ifndef OMG_DDS_PUB_DATA_WRITER_HPP_
#define OMG_DDS_PUB_DATA_WRITER_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dds/core/corefwd.hpp>
#include <dds/core/InstanceHandle.hpp>
#include <dds/topic/topicfwd.hpp>
#include <dds/topic/BuiltinTopic.hpp>
#include <dds/pub/pubfwd.hpp>
#include <dds/pub/detail/DataWriter.hpp>
#include <dds/pub/qos/DataWriterQos.hpp>


namespace dds { namespace pub {

template <typename T, template <typename Q> class DELEGATE>
class DataWriter : public tdds::core::Entity< DELEGATE<T> >{
public:
    typedef dds::pub::DataWriterListener<T>              Listener;
    typedef dds::core::cond::StatusCondition<DataWriter> StatusCondition;

public:
    OMG_DDS_REF_TYPE(DataWriter, tdds::core::Entity, DELEGATE<T>)
public:
    ~DataWriter() { }

public:
    /**
     * This operation allows access to the StatusCondition
     * (Section 7.1.2.1.9, “StatusCondition Class) associated with the Entity.
     * The returned condition can then be added to a WaitSet (Section 7.1.2.1.6,
     * WaitSet Class) so that the application can wait for specific status changes
     * that affect the Entity.
     *
     * @return the status condition
     */
    StatusCondition status_condition() const{
        return this->delegate()->template status_condition<DataWriter>(*this);
    }

public:
    //==========================================================================
    //== Write API
    void write(const T& sample) {
        this->delegate()->write(sample);
    }

    void write(const T& sample, const dds::core::Time& timestamp) {
        this->delegate()->write(sample, timestamp);
    }

    void write(const T& data, const dds::core::InstanceHandle& instance) {
        this->delegate()->write(data, instance);
    }

    void write(const T& data,
               const dds::core::InstanceHandle& instance,
               const dds::core::Time& timestamp) {
        this->delegate()->write(data, instance, timestamp);
    }

    void write(const dds::topic::TopicInstance<T>& i) {
        this->delegate()->write(i);
    }

    void write(const dds::topic::TopicInstance<T>& i, const dds::core::Time& timestamp) {
        this->delegate()->write(i, timestamp);
    }

    /**
     * Write a series of samples or TopicInstances (determined by the template
     * specialization).
     */
    template <typename FWIterator>
    void write(const FWIterator& begin, const FWIterator& end) {
        while (begin != end) {
            this->delegate()->write(*begin);
            ++begin;
        }
    }

    template <typename FWIterator>
    void write(const FWIterator& begin, const FWIterator& end,
            const dds::core::Time& timestamp) {
        while (begin != end) {
            this->delegate()->write(*begin, timestamp);
            ++begin;
        }
    }

    /**
     * Write a series of samples and their parallel instance handles.
     */
    template <typename SamplesFWIterator, typename HandlesFWIterator>
    void write(const SamplesFWIterator& data_begin,
               const SamplesFWIterator& data_end,
               const HandlesFWIterator& handle_begin,
               const HandlesFWIterator& handle_end) {
        while (data_begin != data_end && handle_begin != handle_end) {
            this->delegate()->write(*data_begin, *handle_begin);
            ++data_begin; ++handle_begin;
        }
    }

    /**
     * Write a series of samples and their parallel instance handles.
     */
    template <typename SamplesFWIterator, typename HandlesFWIterator>
    void write(const SamplesFWIterator& data_begin,
               const SamplesFWIterator& data_end,
               const HandlesFWIterator& handle_begin,
               const HandlesFWIterator& handle_end,
               const dds::core::Time& timestamp) {
        while (data_begin != data_end && handle_begin != handle_end) {
            this->delegate()->write(*data_begin, *handle_begin, timestamp);
            ++data_begin; ++handle_begin;
        }
    }

    template <typename DATA>
    void operator << (const DATA& data) {
        this->write(data);
    }

    //==========================================================================
    //== Instance Management
    const dds::core::InstanceHandle register_instance(const T& key) {
        return this->delegate()->register_instance(key);
    }

    const dds::core::InstanceHandle register_instance(const T& key,
            const dds::core::Time& ts) {
        return this->delegate()->register_instance(key, ts);
    }

    void unregister_instance(const dds::core::InstanceHandle& i) {
        this->delegate()->unregister_instance(i);
    }

    void unregister_instance(const dds::core::InstanceHandle& i,
                             const dds::core::Time& ts) {
        this->delegate()->unregister_instance(i,ts);
    }

    void dispose_instance(const dds::core::InstanceHandle& i) {
        this->delegate()->dispose_instance(i);
    }

    void dispose_instance(const dds::core::InstanceHandle& i,
                          const dds::core::Time& ts) {
        this->delegate()->dispose_instance(i,ts);
    }

    /**
     * This operation can be used to retrieve the instance key that corresponds
     * to an instance_handle. The operation will only fill the fields that form
     * the key inside the key_holder instance.
     * This operation may raise a BadParameter exception if the InstanceHandle
     * does not correspond to an existing data-object known to the DataWriter.
     * If the implementation is not able to check invalid handles, then the
     * result in this situation is unspecified.
     */
    dds::topic::TopicInstance<T>& key_value(dds::topic::TopicInstance<T>& i,
                                            const dds::core::InstanceHandle& h) {
        return this->delegate()->key_value(i, h);
    }

    /**
     * This operation can be used to retrieve the instance key that corresponds
     * to an instance_handle. The operation will only fill the fields that form
     * the key inside the key_holder instance.
     * This operation may raise a BadParameter exception if the InstanceHandle
     * does not correspond to an existing data-object known to the DataWriter.
     * If the implementation is not able to check invalid handles, then the
     * result in this situation is unspecified.
     */
    T& key_value(T& sample, const dds::core::InstanceHandle& h) {
        return this->delegate()->key_value(sample, h);
    }

    /**
     * This operation takes as a parameter an instance and returns a handle
     * that can be used in subsequent operations that accept an instance handle
     * as an argument. The instance parameter is only used for the purpose
     * of examining the fields that define the key. This operation does not
     * register the instance in question. If the instance has not been
     * previously registered, or if for any other reason the Service is unable
     * to provide an instance handle, the Service will return a TopicInstance
     * whose handle will be set to the HANDLE_NIL value.
     */
    const dds::core::InstanceHandle lookup_instance(const T& key) {
        return this->delegate()->lookup_instance(key);
    }

    //==========================================================================
    //== QoS Management

    using tdds::core::Entity< DELEGATE<T> >::qos;

    const dds::pub::qos::DataWriterQos qos() const {
        return this->delegate()->qos();
    }

    void qos(const dds::pub::qos::DataWriterQos& the_qos) {
        this->delegate()->qos(the_qos);
    }

    DataWriter& operator <<(const dds::pub::qos::DataWriterQos& the_qos) {
        this->delegate()->qos(the_qos);
        return *this;
    }

    const DataWriter& operator >> (dds::pub::qos::DataWriterQos& the_qos) const {
        the_qos = this->delegate()->qos();
        return *this;
    }


    //==========================================================================
    //== Entity Navigation
    dds::topic::Topic<T> topic() const {
        return this->delegate()->topic();
    }


    //==========================================================================
    //== ACKs

    /**
     * This operation blocks the calling thread until either all data written
     * by the reliable DataWriter entities is acknowledged by all matched
     * reliable DataReader entities, or else the duration specified by the
     * max_wait parameter elapses, whichever happens first.
     * A normal return indicates that all the samples written have been
     * acknowledged by all reliable matched data readers; A TimeoutError
     * indicates that max_wait elapsed before all the data was acknowledged.
     */
    void wait_for_acknowledgments(const dds::core::Duration& timeout) {
        this->delegate()->wait_for_acknowledgments(timeout);
    }

    //==========================================================================
    //== Listeners Management

    /**
     * By virtue of extending Entity, a DataWriter can be attached to a Listener
     * at creation time or later by using the listener operation.
     * The attached Listener must be a DataWriterListener.
     *
     * @param listener the data writer listener
     */
    void listener(
            DataWriterListener<T>* the_listener,
            const dds::core::status::StatusMask& mask) {
        this->delegate()->listener(the_listener, mask);
    }

    /**
     * Return the listener currently associated with this <code>DataWriter</code>.
     *
     * @return the <code>DataWriterListener</code>.
     */
    DataWriterListener<T>* listener() const {
        return this->delegate()->listener();
    }

    //==========================================================================
    //== Status Management

    const dds::core::status::LivelinessLostStatus liveliness_lost_status() {
        return this->delegate()->liveliness_lost_status();
    }

    const dds::core::status::OfferedDeadlineMissedStatus offered_deadlined_missed_status() {
        return this->delegate()->offered_deadlined_missed_status();
    }

    const dds::core::status::OfferedIncompatibleQosStatus offered_incompatible_qos_staus() {
        return this->delegate()->offered_incompatible_qos_staus();
    }

    const dds::core::status::PublicationMatchedStatus publication_matched_status() {
        return this->delegate()->publication_matched_status();
    }


    //==========================================================================
    //== Liveliness Management
    void assert_liveliness() {
        return this->delegate()->assert_liveliness();
    }

    //==========================================================================
    //== Discovery Management

    /**
     * This operation retrieves the list of subscriptions currently "associated"
     * with the DataWriter; that is, subscriptions that have a matching Topic
     * and compatible QoS that the application has not indicated should be
     * "ignored" by means of the DomainParticipant ignore_subscription operation.
     * The handles returned in the "subscription_handles" list are the ones that
     * are used by the DDS implementation to locally identify the corresponding
     * matched DataReader entities. These handles match the ones that appear
     * in the "instance_handle" field of the SampleInfo when reading the
     * "DCPSSubscriptions" builtin topic. The operation may fail if the
     * infrastructure does not locally maintain the connectivity information.
     */
    const dds::core::InstanceHandleSeq matched_subscriptions() {
        return this->delegate()->matched_subscriptions();
    }

    /**
     * This operation retrieves the list of subscriptions currently "associated"
     * with the DataWriter; that is, subscriptions that have a matching Topic
     * and compatible QoS that the application has not indicated should be
     * "ignored" by means of the DomainParticipant ignore_subscription operation.
     * The handles returned in the "subscription_handles" list are the ones that
     * are used by the DDS implementation to locally identify the corresponding
     * matched DataReader entities. These handles match the ones that appear
     * in the "instance_handle" field of the SampleInfo when reading the
     * "DCPSSubscriptions" builtin topic. The operation may fail if the
     * infrastructure does not locally maintain the connectivity information.
     */
    dds::core::InstanceHandleSeq& matched_subscriptions(dds::core::InstanceHandleSeq& subs) {
        return this->delegate()->matched_subscriptions(subs);
    }

    /**
     * This operation retrieves information on a subscription that is currently
     * “associated” with the DataWriter; that is, a subscription with a matching
     * Topic and compatible QoS that the application has not indicated should be
     * “ignored” by means of the DomainParticipant ignore_subscription operation.
     * The subscription_handle must correspond to a subscription currently
     * associated with the DataWriter, otherwise the operation will fail and
     * throw a BadParameterError.
     * The operation matched_subscriptions can be used to find the subscriptions
     * that are currently matched with the DataWriter.
     *
     * The operation may also fail if the infrastructure does not hold the
     * information necessary to fill in the subscription_data.
     * In this case the operation will throw UnsupportedError.
     */
    const dds::topic::SubscriptionBuiltinTopicData
    matched_subscription_data(const dds::core::InstanceHandle& h) {
        return this->delegate()->matched_subscription_data(h);
    }
};

} }

#endif /* OMG_DDS_PUB_DATA_WRITER_HPP_ */

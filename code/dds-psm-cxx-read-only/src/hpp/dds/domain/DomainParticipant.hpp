#ifndef OMG_DDS_DOMAIN_DOMAINPARTICIPANT_HPP_
#define OMG_DDS_DOMAIN_DOMAINPARTICIPANT_HPP_

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

#include <dds/domain/domainfwd.hpp>
#include <dds/domain/detail/DomainParticipant.hpp>
#include <dds/domain/detail/DomainParticipantHolder.hpp>
#include <dds/core/ref_traits.hpp>

template <typename DELEGATE> template <typename T>
dds::topic::Topic<T>
tdds::domain::DomainParticipant<DELEGATE>::create_topic(
        const std::string& topic_name,
        const std::string& type_name, 
        const dds::topic::qos::TopicQos& topic_qos,
        dds::topic::TopicListener<T>* a_listener,
        const dds::core::status::StatusMask& mask) {
    return dds::topic::Topic<T>(
        new dds::topic::detail::Topic<T>(
            topic_name,
            type_name,
            topic_qos,
            dds::core::smart_ptr_traits<dds::domain::detail::DomainParticipantHolder>::ref_type(
            		new dds::domain::detail::DomainParticipantHolder(*this))));
}


template <typename DELEGATE> template <typename T>
dds::topic::Topic<T>
tdds::domain::DomainParticipant<DELEGATE>::create_topic(
        const std::string& topic_name) {
    return dds::topic::Topic<T>(
        new dds::topic::detail::Topic<T>(
            topic_name,
            topic_name, // use an appropriate type name
            default_topic_qos(),
            dds::core::smart_ptr_traits<dds::domain::detail::DomainParticipantHolder>::
                ref_type(
                    new dds::domain::detail::DomainParticipantHolder(*this))));
}

#endif /* OMG_DDS_DOMAIN_DOMAINPARTICIPANT_HPP_ */

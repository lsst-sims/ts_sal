#ifndef OMG_DDS_DOMAIN_DOMAIN_FWD_HPP_
#define OMG_DDS_DOMAIN_DOMAIN_FWD_HPP_

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
#include <dds/domain/detail/domainfwd.hpp>


namespace dds { namespace domain {

    typedef uint32_t DomainId;

    typedef ::dds::domain::detail::DomainParticipant DomainParticipant;

    typedef ::dds::domain::detail::DomainParticipantFactory
    DomainParticipantFactory;

    class DomainParticipantListener;

    class NoOpDomainParticipantListener;


    namespace qos {
        typedef ::dds::domain::qos::detail::DomainParticipantQos
        DomainParticipantQos;

        typedef ::dds::domain::qos::detail::DomainParticipantFactoryQos
        DomainParticipantFactoryQos;
    }
} }

#endif /* OMG_DDS_DOMAIN_DOMAIN_FWD_HPP_ */

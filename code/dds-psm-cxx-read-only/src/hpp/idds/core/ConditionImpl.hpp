#ifndef OMG_IDDS_CORE_CONDITION_IMPL_HPP_
#define OMG_IDDS_CORE_CONDITION_IMPL_HPP_

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

#include <idds/core/corefwd.hpp>


namespace idds { namespace core {

   class ConditionImpl {
   public:
      virtual ~ConditionImpl();

      virtual void dispatch() { }

      virtual bool trigger_value() {
          return false;
      }

   protected:
      ConditionImpl();
   };

} }

#endif /* OMG_IDDS_CORE_CONDITION_IMPL_HPP_ */


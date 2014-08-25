//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: SALDataDcps.h
//  Source: SALDataDcps.idl
//  Generated: Sun May 26 11:41:57 2013
//  OpenSplice V6.3.0
//  
//******************************************************************
#ifndef _SALDATADCPS_H_
#define _SALDATADCPS_H_

#include "sacpp_mapping.h"
#include "sacpp_DDS_DCPS.h"
#include "dds_dcps_interfaces.h"
#include "dds_builtinTopics.h"
#include "dds_dcps_builtintopics.h"
#include "SALData.h"

namespace SALData
{
   struct SALTopic;

   class SALTopicTypeSupportInterface;

   typedef SALTopicTypeSupportInterface * SALTopicTypeSupportInterface_ptr;
   typedef DDS_DCPSInterface_var < SALTopicTypeSupportInterface> SALTopicTypeSupportInterface_var;
   typedef DDS_DCPSInterface_out < SALTopicTypeSupportInterface> SALTopicTypeSupportInterface_out;


   class SALTopicDataWriter;

   typedef SALTopicDataWriter * SALTopicDataWriter_ptr;
   typedef DDS_DCPSInterface_var < SALTopicDataWriter> SALTopicDataWriter_var;
   typedef DDS_DCPSInterface_out < SALTopicDataWriter> SALTopicDataWriter_out;


   class SALTopicDataReader;

   typedef SALTopicDataReader * SALTopicDataReader_ptr;
   typedef DDS_DCPSInterface_var < SALTopicDataReader> SALTopicDataReader_var;
   typedef DDS_DCPSInterface_out < SALTopicDataReader> SALTopicDataReader_out;


   class SALTopicDataReaderView;

   typedef SALTopicDataReaderView * SALTopicDataReaderView_ptr;
   typedef DDS_DCPSInterface_var < SALTopicDataReaderView> SALTopicDataReaderView_var;
   typedef DDS_DCPSInterface_out < SALTopicDataReaderView> SALTopicDataReaderView_out;

   struct SALTopicSeq_uniq_ {};
   typedef DDS_DCPSUVLSeq < SALTopic, struct SALTopicSeq_uniq_> SALTopicSeq;
   typedef DDS_DCPSSequence_var < SALTopicSeq> SALTopicSeq_var;
   typedef DDS_DCPSSequence_out < SALTopicSeq> SALTopicSeq_out;
   class SALTopicTypeSupportInterface
   :
      virtual public DDS::TypeSupport
   { 
   public:
      typedef SALTopicTypeSupportInterface_ptr _ptr_type;
      typedef SALTopicTypeSupportInterface_var _var_type;

      static SALTopicTypeSupportInterface_ptr _duplicate (SALTopicTypeSupportInterface_ptr obj);
      DDS::Boolean _local_is_a (const char * id);

      static SALTopicTypeSupportInterface_ptr _narrow (DDS::Object_ptr obj);
      static SALTopicTypeSupportInterface_ptr _unchecked_narrow (DDS::Object_ptr obj);
      static SALTopicTypeSupportInterface_ptr _nil () { return 0; }
      static const char * _local_id;
      SALTopicTypeSupportInterface_ptr _this () { return this; }


   protected:
      SALTopicTypeSupportInterface () {};
      ~SALTopicTypeSupportInterface () {};
   private:
      SALTopicTypeSupportInterface (const SALTopicTypeSupportInterface &);
      SALTopicTypeSupportInterface & operator = (const SALTopicTypeSupportInterface &);
   };

   class SALTopicDataWriter
   :
      virtual public DDS::DataWriter
   { 
   public:
      typedef SALTopicDataWriter_ptr _ptr_type;
      typedef SALTopicDataWriter_var _var_type;

      static SALTopicDataWriter_ptr _duplicate (SALTopicDataWriter_ptr obj);
      DDS::Boolean _local_is_a (const char * id);

      static SALTopicDataWriter_ptr _narrow (DDS::Object_ptr obj);
      static SALTopicDataWriter_ptr _unchecked_narrow (DDS::Object_ptr obj);
      static SALTopicDataWriter_ptr _nil () { return 0; }
      static const char * _local_id;
      SALTopicDataWriter_ptr _this () { return this; }

      virtual DDS::InstanceHandle_t register_instance (const SALTopic& instance_data) = 0;
      virtual DDS::InstanceHandle_t register_instance_w_timestamp (const SALTopic& instance_data, const DDS::Time_t& source_timestamp) = 0;
      virtual DDS::ReturnCode_t unregister_instance (const SALTopic& instance_data, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::ReturnCode_t unregister_instance_w_timestamp (const SALTopic& instance_data, DDS::InstanceHandle_t handle, const DDS::Time_t& source_timestamp) = 0;
      virtual DDS::ReturnCode_t write (const SALTopic& instance_data, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::ReturnCode_t write_w_timestamp (const SALTopic& instance_data, DDS::InstanceHandle_t handle, const DDS::Time_t& source_timestamp) = 0;
      virtual DDS::ReturnCode_t dispose (const SALTopic& instance_data, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::ReturnCode_t dispose_w_timestamp (const SALTopic& instance_data, DDS::InstanceHandle_t handle, const DDS::Time_t& source_timestamp) = 0;
      virtual DDS::ReturnCode_t writedispose (const SALTopic& instance_data, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::ReturnCode_t writedispose_w_timestamp (const SALTopic& instance_data, DDS::InstanceHandle_t handle, const DDS::Time_t& source_timestamp) = 0;
      virtual DDS::ReturnCode_t get_key_value (SALTopic& key_holder, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::InstanceHandle_t lookup_instance (const SALTopic& instance_data) = 0;

   protected:
      SALTopicDataWriter () {};
      ~SALTopicDataWriter () {};
   private:
      SALTopicDataWriter (const SALTopicDataWriter &);
      SALTopicDataWriter & operator = (const SALTopicDataWriter &);
   };

   class SALTopicDataReader
   :
      virtual public DDS::DataReader
   { 
   public:
      typedef SALTopicDataReader_ptr _ptr_type;
      typedef SALTopicDataReader_var _var_type;

      static SALTopicDataReader_ptr _duplicate (SALTopicDataReader_ptr obj);
      DDS::Boolean _local_is_a (const char * id);

      static SALTopicDataReader_ptr _narrow (DDS::Object_ptr obj);
      static SALTopicDataReader_ptr _unchecked_narrow (DDS::Object_ptr obj);
      static SALTopicDataReader_ptr _nil () { return 0; }
      static const char * _local_id;
      SALTopicDataReader_ptr _this () { return this; }

      virtual DDS::ReturnCode_t read (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t take_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t read_next_sample (SALTopic& received_data, DDS::SampleInfo& sample_info) = 0;
      virtual DDS::ReturnCode_t take_next_sample (SALTopic& received_data, DDS::SampleInfo& sample_info) = 0;
      virtual DDS::ReturnCode_t read_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_next_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take_next_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_next_instance_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t take_next_instance_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t return_loan (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq) = 0;
      virtual DDS::ReturnCode_t get_key_value (SALTopic& key_holder, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::InstanceHandle_t lookup_instance (const SALTopic& instance) = 0;

   protected:
      SALTopicDataReader () {};
      ~SALTopicDataReader () {};
   private:
      SALTopicDataReader (const SALTopicDataReader &);
      SALTopicDataReader & operator = (const SALTopicDataReader &);
   };

   class SALTopicDataReaderView
   :
      virtual public DDS::DataReaderView
   { 
   public:
      typedef SALTopicDataReaderView_ptr _ptr_type;
      typedef SALTopicDataReaderView_var _var_type;

      static SALTopicDataReaderView_ptr _duplicate (SALTopicDataReaderView_ptr obj);
      DDS::Boolean _local_is_a (const char * id);

      static SALTopicDataReaderView_ptr _narrow (DDS::Object_ptr obj);
      static SALTopicDataReaderView_ptr _unchecked_narrow (DDS::Object_ptr obj);
      static SALTopicDataReaderView_ptr _nil () { return 0; }
      static const char * _local_id;
      SALTopicDataReaderView_ptr _this () { return this; }

      virtual DDS::ReturnCode_t read (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t take_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t read_next_sample (SALTopic& received_data, DDS::SampleInfo& sample_info) = 0;
      virtual DDS::ReturnCode_t take_next_sample (SALTopic& received_data, DDS::SampleInfo& sample_info) = 0;
      virtual DDS::ReturnCode_t read_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_next_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t take_next_instance (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::SampleStateMask sample_states, DDS::ViewStateMask view_states, DDS::InstanceStateMask instance_states) = 0;
      virtual DDS::ReturnCode_t read_next_instance_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t take_next_instance_w_condition (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::InstanceHandle_t a_handle, DDS::ReadCondition_ptr a_condition) = 0;
      virtual DDS::ReturnCode_t return_loan (SALTopicSeq& received_data, DDS::SampleInfoSeq& info_seq) = 0;
      virtual DDS::ReturnCode_t get_key_value (SALTopic& key_holder, DDS::InstanceHandle_t handle) = 0;
      virtual DDS::InstanceHandle_t lookup_instance (const SALTopic& instance) = 0;

   protected:
      SALTopicDataReaderView () {};
      ~SALTopicDataReaderView () {};
   private:
      SALTopicDataReaderView (const SALTopicDataReaderView &);
      SALTopicDataReaderView & operator = (const SALTopicDataReaderView &);
   };

}
template <>
DDS::BuiltinTopicKey_t_slice* DDS_DCPS_ArrayHelper < DDS::BuiltinTopicKey_t, DDS::BuiltinTopicKey_t_slice, DDS::BuiltinTopicKey_t_uniq_>::alloc ();
template <>
void DDS_DCPS_ArrayHelper < DDS::BuiltinTopicKey_t, DDS::BuiltinTopicKey_t_slice, DDS::BuiltinTopicKey_t_uniq_>::copy (DDS::BuiltinTopicKey_t_slice *to, const DDS::BuiltinTopicKey_t_slice* from);
template <>
void DDS_DCPS_ArrayHelper < DDS::BuiltinTopicKey_t, DDS::BuiltinTopicKey_t_slice, DDS::BuiltinTopicKey_t_uniq_>::free (DDS::BuiltinTopicKey_t_slice *ptr);




#endif 

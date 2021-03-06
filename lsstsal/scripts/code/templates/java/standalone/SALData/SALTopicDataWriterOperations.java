package SALData;

public interface SALTopicDataWriterOperations extends
    DDS.DataWriterOperations
{

    long register_instance(
            SALData.SALTopic instance_data);

    long register_instance_w_timestamp(
            SALData.SALTopic instance_data, 
            DDS.Time_t source_timestamp);

    int unregister_instance(
            SALData.SALTopic instance_data, 
            long handle);

    int unregister_instance_w_timestamp(
            SALData.SALTopic instance_data, 
            long handle, 
            DDS.Time_t source_timestamp);

    int write(
            SALData.SALTopic instance_data, 
            long handle);

    int write_w_timestamp(
            SALData.SALTopic instance_data, 
            long handle, 
            DDS.Time_t source_timestamp);

    int dispose(
            SALData.SALTopic instance_data, 
            long instance_handle);

    int dispose_w_timestamp(
            SALData.SALTopic instance_data, 
            long instance_handle, 
            DDS.Time_t source_timestamp);
    
    int writedispose(
            SALData.SALTopic instance_data, 
            long instance_handle);

    int writedispose_w_timestamp(
            SALData.SALTopic instance_data, 
            long instance_handle, 
            DDS.Time_t source_timestamp);

    int get_key_value(
            SALData.SALTopicHolder key_holder, 
            long handle);
    
    long lookup_instance(
            SALData.SALTopic instance_data);

}

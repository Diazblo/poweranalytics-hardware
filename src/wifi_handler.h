#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>


#define config_portal_timeout 200
#define MAX_VARS 10
#define powermonitor_sync_data_add(pinstance, pvar) pinstance.powermonitor_sync_data_name[pinstance.powermonitorno][10] = #pvar; pinstance.powermonitor_sync_ptr[pinstance.powermonitorno++] = & pvar;

/* functions */
bool wifi_init();
void start_config_portal();


class wifi_handler
{
    public:
        void pwanl_sync();

        int powermonitorno;
        const char* powermonitor_sync_data_name[MAX_VARS][10];
        double* powermonitor_sync_ptr[MAX_VARS];

    private:
        String serverName = "http://poweranalytics.loca.lt/endpoint/";

        const char* root_ca= \
        "-----BEGIN CERTIFICATE-----\n" \
        "MIICiTCCAg+gAwIBAgIQH0evqmIAcFBUTAGem2OZKjAKBggqhkjOPQQDAzCBhTEL\n" \
        "MAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n" \
        "BxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RPIENBIExpbWl0ZWQxKzApBgNVBAMT\n" \
        "IkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDgwMzA2MDAw\n" \
        "MDAwWhcNMzgwMTE4MjM1OTU5WjCBhTELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdy\n" \
        "ZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEaMBgGA1UEChMRQ09N\n" \
        "T0RPIENBIExpbWl0ZWQxKzApBgNVBAMTIkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlv\n" \
        "biBBdXRob3JpdHkwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQDR3svdcmCFYX7deSR\n" \
        "FtSrYpn1PlILBs5BAH+X4QokPB0BBO490o0JlwzgdeT6+3eKKvUDYEs2ixYjFq0J\n" \
        "cfRK9ChQtP6IHG4/bC8vCVlbpVsLM5niwz2J+Wos77LTBumjQjBAMB0GA1UdDgQW\n" \
        "BBR1cacZSBm8nZ3qQUfflMRId5nTeTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/\n" \
        "BAUwAwEB/zAKBggqhkjOPQQDAwNoADBlAjEA7wNbeqy3eApyt4jf/7VGFAkK+qDm\n" \
        "fQjGGoe9GKhzvSbKYAydzpmfz1wPMOG+FDHqAjAU9JM8SaczepBGR7NjfRObTrdv\n" \
        "GDeAU/7dIOA1mjbRxwG55tzd8/8dLDoWV9mSOdY=\n" \
        "-----END CERTIFICATE-----\n";

        
};


















#if 0 /* old method */

// powermonitor_sync_data(3, pwanl.instpower, pwanl.totalpower, pwanl.powerfactor);//, pwanl.avgpower, pwanl.vrms

#define powermonitor_sync_data(element_cnt, var_name, ...) int powermonitorno = element_cnt; 
const char* powermonitor_sync_data_name[] =  {var_name_arr1(#var_name, __VA_ARGS__)};
var_ptr_ ## element_cnt(var_name, __VA_ARGS__)

#define var_name_arr1(var_name1, ...) #var_name1, var_name_arr2(__VA_ARGS__)
#define var_name_arr2(var_name2, ...) #var_name2, var_name_arr3(__VA_ARGS__)
#define var_name_arr3(var_name3, ...) #var_name3, var_name_arr4(__VA_ARGS__)
#define var_name_arr4(var_name4, ...) #var_name4, var_name_arr5(__VA_ARGS__)
#define var_name_arr5(var_name5, ...) #var_name5, var_name_arr6(__VA_ARGS__)
#define var_name_arr6(var_name6, ...) #var_name6, var_name_arr7(__VA_ARGS__)
#define var_name_arr7(var_name7, ...) #var_name7, var_name_arr8(__VA_ARGS__)
#define var_name_arr8(var_name8, ...) #var_name8, var_name_arr9(__VA_ARGS__)
#define var_name_arr9(var_name9, ...) #var_name9, var_name_arr10(__VA_ARGS__)
#define var_name_arr10(var_name10, ...) #var_name10

#define var_ptr_1(ptr1) double* powermonitor_sync_ptr[] =  {& ptr1}
#define var_ptr_2(ptr1, ptr2) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2}
#define var_ptr_3(ptr1, ptr2, ptr3) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3}
#define var_ptr_4(ptr1, ptr2, ptr3, ptr4) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4}
#define var_ptr_5(ptr1, ptr2, ptr3, ptr4, ptr5) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5}
#define var_ptr_6(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5, & ptr6}
#define var_ptr_7(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, ptr7) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5, & ptr6, & ptr7}
#define var_ptr_8(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, ptr7, ptr8) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5, & ptr6, & ptr7, & ptr8}
#define var_ptr_9(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, ptr7, ptr8, ptr9) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5, & ptr6, & ptr7, & ptr8, & ptr9}
#define var_ptr_10(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, ptr7, ptr8, ptr9, ptr10) double* powermonitor_sync_ptr[] =  {& ptr1, & ptr2, & ptr3, & ptr4, & ptr5, & ptr6, & ptr7, & ptr8, & ptr9, & ptr10}

#endif



#endif /* WIFI_HANDLER_H */
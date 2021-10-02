#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#define config_portal_timeout 200

bool pwanl_wifiinit();
void start_config_portal();
void pwanl_sync();

uint8_t powermonitor_sync_data_no = 0;

#define powermonitor_sync_data(element_cnt, var_name, ...) powermonitor_sync_data_no = element_cnt; \
char* powermonitor_sync_data_name[] =  {#var_name, var_name_arr1(__VA_ARGS__)};\
var_ptr_ ## element_cnt(var_name, __VA_ARGS__)

// double* powermonitor_sync_ptr[] =  {& var_name, var_ptr_arr1(__VA_ARGS__)}

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

#endif /* WIFI_HANDLER_H */
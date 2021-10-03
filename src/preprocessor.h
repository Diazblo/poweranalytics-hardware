// #define powermonitor_sync_data(var_name, ...) char* powermonitor_sync_data_name[] =  {#var_name var_name_arr1(__VA_ARGS__)}
// #define var_name_arr1(var_name1, ...) #var_name1 __VA_OPT__(,) var_name_arr2(__VA_ARGS__)
// #define var_name_arr2(var_name2, ...) #var_name2 __VA_OPT__(,) var_name_arr3(__VA_ARGS__)
// #define var_name_arr3(var_name3, ...) #var_name3 __VA_OPT__(,) var_name_arr4(__VA_ARGS__)
// #define var_name_arr4(var_name4, ...) #var_name4 __VA_OPT__(,) var_name_arr5(__VA_ARGS__)
// #define var_name_arr5(var_name5, ...) #var_name5 __VA_OPT__(,) var_name_arr6(__VA_ARGS__)
// #define var_name_arr6(var_name6, ...) #var_name6 __VA_OPT__(,) var_name_arr7(__VA_ARGS__)
// #define var_name_arr7(var_name7, ...) #var_name7 __VA_OPT__(,) var_name_arr8(__VA_ARGS__)
// #define var_name_arr8(var_name8, ...) #var_name8 __VA_OPT__(,) var_name_arr9(__VA_ARGS__)
// #define var_name_arr9(var_name9, ...) #var_name9 __VA_OPT__(,) var_name_arr10(__VA_ARGS__)
// #define var_name_arr10(var_name10, ...) #var_name10




// #define var_ptr_arr1(var_ptr1, ...) & var_ptr1 , var_ptr_arr2(__VA_ARGS__)
// #define var_ptr_arr2(var_ptr2, ...) & var_ptr2 , var_ptr_arr3(__VA_ARGS__)
// #define var_ptr_arr3(var_ptr3, ...) & var_ptr3 , var_ptr_arr4(__VA_ARGS__)
// #define var_ptr_arr4(var_ptr4, ...) & var_ptr4 , var_ptr_arr5(__VA_ARGS__)
// #define var_ptr_arr5(var_ptr5, ...) & var_ptr5 , var_ptr_arr6(__VA_ARGS__)
// #define var_ptr_arr6(var_ptr6, ...) & var_ptr6 , var_ptr_arr7(__VA_ARGS__)
// #define var_ptr_arr7(var_ptr7, ...) & var_ptr7 , var_ptr_arr8(__VA_ARGS__)
// #define var_ptr_arr8(var_ptr8, ...) & var_ptr8 , var_ptr_arr9(__VA_ARGS__)
// #define var_ptr_arr9(var_ptr9, ...) & var_ptr9 , var_ptr_arr10(__VA_ARGS__)
// #define var_ptr_arr10(var_ptr10, ...) & var_ptr10\

// #define powermonitor_sync_dataa(element_cnt, ...) var_ptr_ ## element_cnt(__VA_ARGS__)
// powermonitor_sync_data_no = element_cnt; 


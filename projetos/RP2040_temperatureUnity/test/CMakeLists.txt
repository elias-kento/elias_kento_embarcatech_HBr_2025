enable_testing()

add_executable(test_adc test_adc.c)
target_link_libraries(test_adc adc_temp unity)
target_include_directories(test_adc PRIVATE ../include)

add_test(NAME adc_temp_test COMMAND test_adc)

add_executable(timestamp_test Timestamp_test.cc)
target_link_libraries(timestamp_test clownfish_base)

add_executable(logging_test Logging_test.cc)
target_link_libraries(logging_test clownfish_base)

add_executable(logfile_test LogFile_test.cc)
target_link_libraries(logfile_test clownfish_base)

add_executable(threadpool_test ThreadPool_test.cc)
target_link_libraries(threadpool_test clownfish_base)

add_executable(lockfreequeue_test LockFreeQueue_test.cc)
target_link_libraries(lockfreequeue_test clownfish_base)

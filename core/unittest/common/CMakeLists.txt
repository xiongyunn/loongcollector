# Copyright 2022 iLogtail Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.22)
project(common_unittest)

add_executable(common_simple_utils_unittest SimpleUtilsUnittest.cpp
        FileSystemUtilUnittest.h
        DevInodeUnittest.h
        TimeUtilUnittest.h)
target_link_libraries(common_simple_utils_unittest ${UT_BASE_TARGET})

add_executable(common_logfileoperator_unittest LogFileOperatorUnittest.cpp)
target_link_libraries(common_logfileoperator_unittest ${UT_BASE_TARGET})

add_executable(instance_identity_unittest InstanceIdentityUnittest.cpp)
target_link_libraries(instance_identity_unittest ${UT_BASE_TARGET})

add_executable(common_sliding_window_counter_unittest SlidingWindowCounterUnittest.cpp)
target_link_libraries(common_sliding_window_counter_unittest ${UT_BASE_TARGET})

# add_executable(common_string_piece_unittest StringPieceUnittest.cpp)
# target_link_libraries(common_string_piece_unittest ${UT_BASE_TARGET})

add_executable(common_string_tools_unittest StringToolsUnittest.cpp)
target_link_libraries(common_string_tools_unittest ${UT_BASE_TARGET})

add_executable(common_machine_info_util_unittest MachineInfoUtilUnittest.cpp)
target_link_libraries(common_machine_info_util_unittest ${UT_BASE_TARGET})

add_executable(encoding_converter_unittest EncodingConverterUnittest.cpp)
target_link_libraries(encoding_converter_unittest ${UT_BASE_TARGET})

add_executable(yaml_util_unittest YamlUtilUnittest.cpp)
target_link_libraries(yaml_util_unittest ${UT_BASE_TARGET})

add_executable(safe_queue_unittest SafeQueueUnittest.cpp)
target_link_libraries(safe_queue_unittest ${UT_BASE_TARGET})

add_executable(http_request_timer_event_unittest timer/HttpRequestTimerEventUnittest.cpp)
target_link_libraries(http_request_timer_event_unittest ${UT_BASE_TARGET})

add_executable(timer_unittest timer/TimerUnittest.cpp)
target_link_libraries(timer_unittest ${UT_BASE_TARGET})

add_executable(curl_unittest http/CurlUnittest.cpp)
target_link_libraries(curl_unittest ${UT_BASE_TARGET})

if (LINUX)
    add_executable(proc_parser_unittest ProcParserUnittest.cpp)
    target_link_libraries(proc_parser_unittest ${UT_BASE_TARGET})
endif()

add_executable(network_util_unittest NetworkUtilUnittest.cpp)
target_link_libraries(network_util_unittest ${UT_BASE_TARGET})

add_executable(lru_benchmark LRUBenchmark.cpp)
target_link_libraries(lru_benchmark ${UT_BASE_TARGET})

add_executable(timekeeper_benchmark TimeKeeperBenchmark.cpp)
target_link_libraries(timekeeper_benchmark ${UT_BASE_TARGET})

include(GoogleTest)
gtest_discover_tests(common_simple_utils_unittest)
gtest_discover_tests(common_logfileoperator_unittest)
gtest_discover_tests(common_sliding_window_counter_unittest)
gtest_discover_tests(common_string_tools_unittest)
gtest_discover_tests(common_machine_info_util_unittest)
gtest_discover_tests(encoding_converter_unittest)
gtest_discover_tests(yaml_util_unittest)
gtest_discover_tests(safe_queue_unittest)
gtest_discover_tests(http_request_timer_event_unittest)
gtest_discover_tests(timer_unittest)
gtest_discover_tests(curl_unittest)
if (LINUX)
    gtest_discover_tests(proc_parser_unittest)
endif()
gtest_discover_tests(network_util_unittest)
gtest_discover_tests(lru_benchmark)
gtest_discover_tests(timekeeper_benchmark)

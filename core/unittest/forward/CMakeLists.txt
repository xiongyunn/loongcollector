# Copyright 2023 iLogtail Authors
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
project(forward_unittest)

add_executable(grpc_runner_unittest GrpcRunnerUnittest.cpp)
target_link_libraries(grpc_runner_unittest ${UT_BASE_TARGET})

include(GoogleTest)
gtest_discover_tests(grpc_runner_unittest)
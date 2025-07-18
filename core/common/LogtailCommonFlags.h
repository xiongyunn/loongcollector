/*
 * Copyright 2022 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "common/Flags.h"

// app config
DECLARE_FLAG_STRING(ilogtail_config);
DECLARE_FLAG_BOOL(enable_full_drain_mode);
DECLARE_FLAG_INT32(cpu_limit_num);
DECLARE_FLAG_INT32(mem_limit_num);
DECLARE_FLAG_DOUBLE(cpu_usage_up_limit);
DECLARE_FLAG_INT64(memory_usage_up_limit);

// epoll
DECLARE_FLAG_INT32(ilogtail_epoll_time_out);
DECLARE_FLAG_INT32(ilogtail_epoll_wait_events);
DECLARE_FLAG_INT32(ilogtail_max_epoll_events);

// sls sender
DECLARE_FLAG_BOOL(sls_client_send_compress);
DECLARE_FLAG_STRING(default_region_name);

// profile
DECLARE_FLAG_STRING(logtail_profile_aliuid);

// monitor
DECLARE_FLAG_INT32(monitor_interval);

// process
DECLARE_FLAG_BOOL(ilogtail_discard_old_data);
DECLARE_FLAG_INT32(ilogtail_discard_interval);

// file source
DECLARE_FLAG_BOOL(enable_root_path_collection);
DECLARE_FLAG_INT32(timeout_interval);

DECLARE_FLAG_STRING(default_container_host_path);

DECLARE_FLAG_STRING(conf_dir);
DECLARE_FLAG_STRING(logs_dir);
DECLARE_FLAG_STRING(data_dir);
DECLARE_FLAG_STRING(run_dir);
DECLARE_FLAG_STRING(third_party_dir);

// ebpf event retry flags
DECLARE_FLAG_INT32(ebpf_event_retry_interval_sec);
DECLARE_FLAG_INT32(ebpf_event_retry_limit);

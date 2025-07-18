/*
 * Copyright 2023 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/Lock.h"
#include "models/PipelineEventGroup.h"
#include "monitor/metric_constants/MetricConstants.h"
#include "monitor/metric_models/MetricRecord.h"
#include "monitor/metric_models/SelfMonitorMetricEvent.h"

namespace logtail {

class WriteMetrics {
private:
    WriteMetrics() = default;
    std::mutex mMutex;
    MetricsRecord* mHead = nullptr;

    void Clear();
    MetricsRecord* GetHead();

public:
    ~WriteMetrics();
    static WriteMetrics* GetInstance() {
        static WriteMetrics* ptr = new WriteMetrics();
        return ptr;
    }

    // First, create a MetricsRecordRef using the CreateMetricsRecordRef method and Labels. Then add counter and gauge
    // metrics. Finally, commit the MetricsRecordRef through CommitMetricsRecordRef. This will successfully collect the
    // metric data.
    void CreateMetricsRecordRef(MetricsRecordRef& ref,
                                const std::string& category,
                                MetricLabels&& labels,
                                DynamicMetricLabels&& dynamicLabels = {});
    void CommitMetricsRecordRef(MetricsRecordRef& ref);
    MetricsRecord* DoSnapshot();


#ifdef APSARA_UNIT_TEST_MAIN
    friend class MetricManagerUnittest;
#endif
};

class ReadMetrics {
private:
    ReadMetrics() = default;
    mutable ReadWriteLock mReadWriteLock;
    MetricsRecord* mHead = nullptr;
    std::vector<std::map<std::string, std::string>> mGoMetrics;
    void Clear();
    MetricsRecord* GetHead();
    void UpdateGoCppProvidedMetrics(std::vector<std::map<std::string, std::string>>& metricsList);

public:
    ~ReadMetrics();
    static ReadMetrics* GetInstance() {
        static ReadMetrics* ptr = new ReadMetrics();
        return ptr;
    }
    void ReadAsSelfMonitorMetricEvents(std::vector<SelfMonitorMetricEvent>& metricEventList) const;
    void UpdateMetrics();

#ifdef APSARA_UNIT_TEST_MAIN
    friend class MetricManagerUnittest;
#endif
};

} // namespace logtail

// Copyright 2024 iLogtail Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "collection_pipeline/CollectionPipelineManager.h"

using namespace std;

namespace logtail {

class PipelineMock : public CollectionPipeline {
public:
    bool Init(CollectionConfig&& config) {
        mConfig = std::move(config.mDetail);
        WriteMetrics::GetInstance()->CreateMetricsRecordRef(
            mMetricsRecordRef,
            MetricCategory::METRIC_CATEGORY_PIPELINE,
            {{METRIC_LABEL_KEY_PROJECT, mContext.GetProjectName()}, {METRIC_LABEL_KEY_PIPELINE_NAME, mName}});
        mStartTime = mMetricsRecordRef.CreateIntGauge(METRIC_PIPELINE_START_TIME);
        WriteMetrics::GetInstance()->CommitMetricsRecordRef(mMetricsRecordRef);
        mSingletonInput = config.mSingletonInput;
        mContext.SetCreateTime(config.mCreateTime);
        return (*mConfig)["valid"].asBool();
    }
};

class PipelineManagerMock : public CollectionPipelineManager {
public:
    static PipelineManagerMock* GetInstance() {
        static PipelineManagerMock instance;
        return &instance;
    }

    void ClearEnvironment() {
        for (auto& it : mPipelineNameEntityMap) {
            it.second->Stop(true);
        }
        mPipelineNameEntityMap.clear();
    }

private:
    shared_ptr<CollectionPipeline> BuildPipeline(CollectionConfig&& config) override {
        // this should be synchronized with CollectionPipelineManager::BuildPipeline, except for the pointer type.
        shared_ptr<PipelineMock> p = make_shared<PipelineMock>();
        if (!p->Init(std::move(config))) {
            return nullptr;
        }
        return p;
    }
};
} // namespace logtail

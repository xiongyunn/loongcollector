// Copyright 2023 iLogtail Authors
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

#include "json/json.h"

#include "app_config/AppConfig.h"
#include "collection_pipeline/CollectionPipeline.h"
#include "collection_pipeline/CollectionPipelineContext.h"
#include "common/JsonUtil.h"
#include "common/http/AsynCurlRunner.h"
#include "common/timer/Timer.h"
#include "ebpf/Config.h"
#include "ebpf/EBPFServer.h"
#include "plugin/input/InputNetworkObserver.h"
#include "unittest/Unittest.h"

using namespace std;

namespace logtail {

class InputNetworkObserverUnittest : public testing::Test {
public:
    void TestName();
    void TestSupportAck();
    void OnSuccessfulInit();
    void OnFailedInit();
    void OnSuccessfulStart();
    void OnSuccessfulStop();
    // void OnPipelineUpdate();

protected:
    void SetUp() override {
        p.mName = "test_config";
        ctx.SetConfigName("test_config");
        ctx.SetPipeline(p);
        ebpf::EBPFServer::GetInstance()->Init();
    }

    void TearDown() override {
        ebpf::EBPFServer::GetInstance()->Stop();
        Timer::GetInstance()->Stop();
        AsynCurlRunner::GetInstance()->Stop();
    }

private:
    CollectionPipeline p;
    CollectionPipelineContext ctx;
};

void InputNetworkObserverUnittest::TestName() {
    InputNetworkObserver input;
    std::string name = input.Name();
    APSARA_TEST_EQUAL(name, "input_network_observer");
}

void InputNetworkObserverUnittest::TestSupportAck() {
    InputNetworkObserver input;
    bool supportAck = input.SupportAck();
    APSARA_TEST_TRUE(supportAck);
}

void InputNetworkObserverUnittest::OnSuccessfulInit() {
    unique_ptr<InputNetworkObserver> input;
    Json::Value configJson, optionalGoPipeline;
    string configStr, errorMsg;

    // valid optional param
    configStr = R"(
        {
            "Type": "input_network_observer",
            "ProbeConfig": 
            {
                "EnableProtocols": [
                    "http"
                ],
                "DisableProtocolParse": false,
                "DisableConnStats": false,
                "EnableConnTrackerDump": false
            }
        }
    )";
    APSARA_TEST_TRUE(ParseJsonTable(configStr, configJson, errorMsg));
    input.reset(new InputNetworkObserver());
    input->SetContext(ctx);
    input->CreateMetricsRecordRef("test", "1");
    APSARA_TEST_TRUE(input->Init(configJson, optionalGoPipeline));
    input->CommitMetricsRecordRef();
    APSARA_TEST_EQUAL(input->sName, "input_network_observer");
    logtail::ebpf::ObserverNetworkOption thisObserver = input->mNetworkOption;
    APSARA_TEST_EQUAL(thisObserver.mEnableProtocols.size(), 1UL);
    APSARA_TEST_EQUAL(thisObserver.mEnableProtocols[0], "http");
    APSARA_TEST_EQUAL(false, thisObserver.mDisableProtocolParse);
    APSARA_TEST_EQUAL(false, thisObserver.mDisableConnStats);
    APSARA_TEST_EQUAL(false, thisObserver.mEnableConnTrackerDump);
}

void InputNetworkObserverUnittest::OnFailedInit() {
    unique_ptr<InputNetworkObserver> input;
    Json::Value configJson, optionalGoPipeline;
    string configStr, errorMsg;

    // invalid optional param
    configStr = R"(
        {
            "Type": "input_network_observer",
            "ProbeConfig": 
            {
                "EnableProtocols": [
                    "http"
                ],
                "DisableProtocolParse": 1,
                "DisableConnStats": false,
                "EnableConnTrackerDump": false
            }
        }
    )";
    APSARA_TEST_TRUE(ParseJsonTable(configStr, configJson, errorMsg));
    input.reset(new InputNetworkObserver());
    input->SetContext(ctx);
    input->CreateMetricsRecordRef("test", "1");
    APSARA_TEST_TRUE(input->Init(configJson, optionalGoPipeline));
    input->CommitMetricsRecordRef();
    APSARA_TEST_EQUAL(input->sName, "input_network_observer");
    logtail::ebpf::ObserverNetworkOption thisObserver = input->mNetworkOption;
    APSARA_TEST_EQUAL(thisObserver.mEnableProtocols.size(), 1UL);
    APSARA_TEST_EQUAL(thisObserver.mEnableProtocols[0], "http");
    APSARA_TEST_EQUAL(false, thisObserver.mDisableProtocolParse);
    APSARA_TEST_EQUAL(false, thisObserver.mDisableConnStats);
    APSARA_TEST_EQUAL(false, thisObserver.mEnableConnTrackerDump);

    // lag of mandatory param + error param level
    configStr = R"(
        {
            "Type": "input_network_observer",
            "EnableProtocols": [
                "http"
            ],
            "DisableProtocolParse": false,
            "DisableConnStats": false,
            "EnableConnTrackerDump": false
        }
    )";
    APSARA_TEST_TRUE(ParseJsonTable(configStr, configJson, errorMsg));
    input.reset(new InputNetworkObserver());
    input->SetContext(ctx);
    input->CreateMetricsRecordRef("test", "1");
    APSARA_TEST_FALSE(input->Init(configJson, optionalGoPipeline));
    input->CommitMetricsRecordRef();
}

void InputNetworkObserverUnittest::OnSuccessfulStart() {
    unique_ptr<InputNetworkObserver> input;
    Json::Value configJson, optionalGoPipeline;
    string configStr, errorMsg;

    configStr = R"(
        {
            "Type": "input_network_observer",
            "ProbeConfig": 
            {
                "EnableProtocols": [
                    "http"
                ],
                "DisableProtocolParse": false,
                "DisableConnStats": false,
                "EnableConnTrackerDump": false
            }
        }
    )";
    APSARA_TEST_TRUE(ParseJsonTable(configStr, configJson, errorMsg));
    input.reset(new InputNetworkObserver());
    input->SetContext(ctx);
    input->CreateMetricsRecordRef("test", "1");
    APSARA_TEST_TRUE(input->Init(configJson, optionalGoPipeline));
    input->CommitMetricsRecordRef();
    APSARA_TEST_TRUE(input->Start());
    string serverPipelineName
        = ebpf::EBPFServer::GetInstance()->checkLoadedPipelineName(logtail::ebpf::PluginType::NETWORK_OBSERVE);
    string pipelineName = input->GetContext().GetConfigName();
    APSARA_TEST_TRUE(serverPipelineName.size() && serverPipelineName == pipelineName);
    APSARA_TEST_TRUE(input->Stop(true));
}

void InputNetworkObserverUnittest::OnSuccessfulStop() {
    unique_ptr<InputNetworkObserver> input;
    Json::Value configJson, optionalGoPipeline;
    string configStr, errorMsg;

    configStr = R"(
        {
            "Type": "input_network_observer",
            "ProbeConfig": 
            {
                "EnableProtocols": [
                    "http"
                ],
                "DisableProtocolParse": false,
                "DisableConnStats": false,
                "EnableConnTrackerDump": false
            }
        }
    )";
    APSARA_TEST_TRUE(ParseJsonTable(configStr, configJson, errorMsg));
    input.reset(new InputNetworkObserver());
    input->SetContext(ctx);
    input->CreateMetricsRecordRef("test", "1");
    APSARA_TEST_TRUE(input->Init(configJson, optionalGoPipeline));
    input->CommitMetricsRecordRef();
    APSARA_TEST_TRUE(input->Start());
    string serverPipelineName
        = ebpf::EBPFServer::GetInstance()->checkLoadedPipelineName(logtail::ebpf::PluginType::NETWORK_OBSERVE);
    string pipelineName = input->GetContext().GetConfigName();
    APSARA_TEST_TRUE(serverPipelineName.size() && serverPipelineName == pipelineName);
    // APSARA_TEST_TRUE(input->Stop(false));
    serverPipelineName
        = ebpf::EBPFServer::GetInstance()->checkLoadedPipelineName(logtail::ebpf::PluginType::NETWORK_OBSERVE);
    APSARA_TEST_TRUE(serverPipelineName.size() && serverPipelineName == pipelineName);
    APSARA_TEST_TRUE(input->Stop(true));
    serverPipelineName
        = ebpf::EBPFServer::GetInstance()->checkLoadedPipelineName(logtail::ebpf::PluginType::NETWORK_OBSERVE);
    APSARA_TEST_TRUE(serverPipelineName.empty());
}

UNIT_TEST_CASE(InputNetworkObserverUnittest, TestName)
UNIT_TEST_CASE(InputNetworkObserverUnittest, TestSupportAck)
UNIT_TEST_CASE(InputNetworkObserverUnittest, OnSuccessfulInit)
UNIT_TEST_CASE(InputNetworkObserverUnittest, OnFailedInit)
UNIT_TEST_CASE(InputNetworkObserverUnittest, OnSuccessfulStart)
UNIT_TEST_CASE(InputNetworkObserverUnittest, OnSuccessfulStop)
// UNIT_TEST_CASE(InputNetworkObserverUnittest, OnPipelineUpdate)

} // namespace logtail

UNIT_TEST_MAIN

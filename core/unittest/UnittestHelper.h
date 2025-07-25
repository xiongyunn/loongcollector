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

#include <stdlib.h>
#if defined(__linux__)
#include <sys/errno.h>
#include <sys/utsname.h>
#endif

#include <string>

#include "logger/Logger.h"
#include "protobuf/sls/sls_logs.pb.h"

namespace logtail {

class UnitTestHelper {
public:
    static std::pair<std::string, bool> GetLogTag(const sls_logs::LogGroup* logGroupPB, const std::string& key) {
        auto tags = logGroupPB->logtags();
        for (auto iter = tags.begin(); iter != tags.end(); ++iter) {
            if (iter->key() == key) {
                return std::make_pair(iter->value(), true);
            }
        }
        return std::make_pair(std::string(), false);
    }

    static bool LogTagExisted(const sls_logs::LogGroup* logGroupPB, const std::string& key) {
        auto findRst = GetLogTag(logGroupPB, key);
        return findRst.second;
    }

    static bool LogTagMatched(const sls_logs::LogGroup* logGroupPB, const std::string& key, const std::string& value) {
        auto findRst = GetLogTag(logGroupPB, key);
        return findRst.second && findRst.first == value;
    }

    static std::pair<std::string, bool> GetLogKey(const sls_logs::Log* logPB, const std::string& key) {
        const ::google::protobuf::RepeatedPtrField< ::sls_logs::Log_Content>& contents = logPB->contents();
        for (auto iter = contents.begin(); iter != contents.end(); ++iter) {
            if (iter->key() == key) {
                return std::make_pair(iter->value(), true);
            }
        }
        return std::make_pair(std::string(), false);
    }

    static bool LogKeyExisted(const sls_logs::Log* logPB, const std::string& key) {
        auto findRst = GetLogKey(logPB, key);
        return findRst.second;
    }

    static bool LogKeyMatched(const sls_logs::Log* logPB, const std::string& key, const std::string& value) {
        auto findRst = GetLogKey(logPB, key);
        return findRst.second && findRst.first == value;
    }

#if defined(__linux__)
    static bool GetKernelVersion(int& mainVersion, int& subVersion) {
        struct utsname buf;
        if (uname(&buf) != 0) {
            LOG_ERROR(sLogger, ("failed to get linux kernel version, errno", errno));
            return false;
        }

        std::string releaseVersion(buf.release);
        size_t pos = releaseVersion.find_first_of('-');
        if (pos == std::string::npos) {
            LOG_ERROR(sLogger, ("failed to get linux kernel version, release info", releaseVersion));
            return false;
        }

        size_t mainPos = releaseVersion.find_first_of('.');
        if (mainPos != std::string::npos) {
            mainVersion = atoi(releaseVersion.substr(0, mainPos).c_str());
        }
        size_t subPos = releaseVersion.find_first_of('.', mainPos + 1);
        if (subPos != std::string::npos) {
            subVersion = atoi(releaseVersion.substr(mainPos + 1, subPos - mainPos).c_str());
        }
        return true;
    }
#endif

    static std::string JsonEscapeDirPath(const std::string& path) {
#if defined(_MSC_VER)
        std::string jsonPath = "";
        for (auto& c : path) {
            if (c == '\\') {
                jsonPath += "\\\\";
            } else {
                jsonPath.push_back(c);
            }
        }
        return jsonPath;
#else
        return path;
#endif
    }
};

} // namespace logtail

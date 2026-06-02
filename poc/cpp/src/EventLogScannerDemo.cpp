#include <windows.h>
#include <winevt.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#pragma comment(lib, "wevtapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

namespace AVDetector {

    /// <summary>
    /// Represents a Windows Event Log entry
    /// </summary>
    struct EventLogEntry {
        std::chrono::system_clock::time_point timestamp;
        std::string channel;
        std::string eventType;
        std::string description;
        int severity; // 0=Info, 1=Warning, 2=Error, 3=Critical
        DWORD eventId;

        EventLogEntry() : severity(0), eventId(0) {}
    };

    /// <summary>
    /// Scans Windows Event Logs for security-related events
    /// </summary>
    class EventLogScanner {
    public:
        static std::vector<EventLogEntry> ScanEventLogs(int lastHoursToScan = 1) {
            std::vector<EventLogEntry> results;

            const char* channels[] = {
                "System",
                "Security",
                "Application",
                "Microsoft-Windows-Windows Defender/Operational"
            };

            std::cout << "Scanning " << sizeof(channels) / sizeof(channels[0]) 
                      << " event log channels...\n\n";

            for (const auto& channel : channels) {
                std::cout << "  Scanning: " << channel << "\n";
                auto entries = ScanChannel(channel, lastHoursToScan);
                results.insert(results.end(), entries.begin(), entries.end());
            }

            return results;
        }

    private:
        static std::vector<EventLogEntry> ScanChannel(const char* channelName, int lastHoursToScan) {
            std::vector<EventLogEntry> results;

            // Construct WQL query
            std::string query = "Event/System[EventRecordID <= 1000]"; // Limit to recent events
            
            // Open event log
            EVT_HANDLE hResults = EvtQuery(NULL, std::string(channelName).c_str(), 
                query.c_str(), EvtQueryChannelPath | EvtQueryReverseDirection);

            if (!hResults) {
                // Channel might not exist
                return results;
            }

            EVT_HANDLE hContext = EvtCreateRenderContext(0, NULL, EvtRenderContextSystem);
            if (!hContext) {
                EvtClose(hResults);
                return results;
            }

            DWORD dwReturned = 0;
            EVT_HANDLE hEvent;
            const DWORD EVENT_BATCH_SIZE = 10;
            EVT_HANDLE pEvents[EVENT_BATCH_SIZE];

            // Security keywords to search for
            const char* securityKeywords[] = {
                "threat", "virus", "malware", "suspicious", "antivirus",
                "quarantine", "blocked", "protection", "scan", "detected"
            };

            while (EvtNext(hResults, EVENT_BATCH_SIZE, pEvents, 1000, 0, &dwReturned)) {
                for (DWORD i = 0; i < dwReturned; i++) {
                    DWORD dwBufferSize = 0;
                    DWORD dwPropertyCount = 0;

                    if (!EvtRender(hContext, pEvents[i], EvtRenderEventXml, 0, NULL, &dwBufferSize)) {
                        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                            LPWSTR pRendered = new WCHAR[dwBufferSize];
                            if (pRendered) {
                                if (EvtRender(hContext, pEvents[i], EvtRenderEventXml, 
                                    dwBufferSize, pRendered, &dwBufferSize)) {
                                    
                                    // Convert to string and check for security keywords
                                    std::string xmlStr = WideStringToString(pRendered);
                                    
                                    bool hasSecurityKeyword = false;
                                    for (const auto& keyword : securityKeywords) {
                                        if (xmlStr.find(keyword) != std::string::npos) {
                                            hasSecurityKeyword = true;
                                            break;
                                        }
                                    }

                                    if (hasSecurityKeyword) {
                                        EventLogEntry entry;
                                        entry.channel = channelName;
                                        entry.eventType = "Security Event";
                                        entry.description = xmlStr.substr(0, 200); // First 200 chars
                                        entry.severity = 1; // Warning
                                        results.push_back(entry);
                                    }
                                }
                                delete[] pRendered;
                            }
                        }
                    }

                    EvtClose(pEvents[i]);
                }
            }

            EvtClose(hContext);
            EvtClose(hResults);
            return results;
        }

        static std::string WideStringToString(const wchar_t* wstr) {
            if (!wstr) return "";
            int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
            std::string result(size - 1, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size, NULL, NULL);
            return result;
        }
    };
}

/// <summary>
/// Main demonstration of event log scanning
/// </summary>
int main() {
    std::cout << "=== Event Log Scanner - C++ PoC ===\n\n";

    try {
        std::cout << "Requires Admin privileges to access Security log\n";
        std::cout << "Scanning last 1 hour of events...\n\n";

        auto events = AVDetector::EventLogScanner::ScanEventLogs(1);

        std::cout << "\n\n=== SCAN RESULTS ===\n\n";

        if (events.empty()) {
            std::cout << "No security-related events found in the last hour.\n";
            return 0;
        }

        std::cout << "Found " << events.size() << " security-related event(s):\n\n";

        for (size_t i = 0; i < events.size(); i++) {
            std::cout << (i + 1) << ". [" << events[i].channel << "] " 
                      << events[i].eventType << "\n";
            std::cout << "   Description: " << events[i].description << "\n";
            std::cout << "   Event ID: " << events[i].eventId << "\n\n";
        }

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "EXCEPTION: " << ex.what() << "\n";
        return 1;
    }
}

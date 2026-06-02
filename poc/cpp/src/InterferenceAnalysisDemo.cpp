#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>

namespace AVDetector {

    /// <summary>
    /// Represents an indicator of interference
    /// </summary>
    struct InterferenceIndicator {
        std::string type;
        std::string description;
        int weight; // 0-100
        double timingCorrelation; // 0.0-1.0 (how closely matched in time)
        
        InterferenceIndicator() : weight(0), timingCorrelation(0.0) {}
    };

    /// <summary>
    /// Analyzes correlation between AV activity and process behavior
    /// </summary>
    class InterferenceAnalyzer {
    public:
        struct AnalysisResult {
            bool interferenceDetected;
            int confidenceScore; // 0-100
            std::string primaryCause;
            std::vector<InterferenceIndicator> indicators;
            std::vector<std::string> recommendations;

            AnalysisResult() : interferenceDetected(false), confidenceScore(0) {}
        };

        static AnalysisResult AnalyzeInterference(
            const std::vector<std::string>& avEvents,
            const std::string& processStatus,
            double processCrashTime,
            double avActivityTime) {

            AnalysisResult result;
            std::vector<InterferenceIndicator> indicators;

            // Indicator 1: Process Crash
            if (processStatus == "CRASHED") {
                InterferenceIndicator indicator;
                indicator.type = "ProcessCrashed";
                indicator.description = "Target process terminated unexpectedly";
                indicator.weight = 40;
                indicator.timingCorrelation = 1.0;
                indicators.push_back(indicator);
            }

            // Indicator 2: AV Activity Correlation
            if (!avEvents.empty() && processCrashTime > 0 && avActivityTime > 0) {
                double timeDelta = std::abs(processCrashTime - avActivityTime);
                
                // Strong correlation if within 5 seconds
                double correlation = timeDelta < 5.0 ? (5.0 - timeDelta) / 5.0 : 0.0;
                
                if (correlation > 0) {
                    InterferenceIndicator indicator;
                    indicator.type = "AVActivityCorrelated";
                    indicator.description = "AV activity detected within " + 
                        std::to_string(static_cast<int>(timeDelta)) + " seconds of crash";
                    indicator.weight = 30;
                    indicator.timingCorrelation = correlation;
                    indicators.push_back(indicator);
                }
            }

            // Indicator 3: File Blocking
            for (const auto& event : avEvents) {
                if (event.find("blocked") != std::string::npos ||
                    event.find("quarantine") != std::string::npos) {
                    InterferenceIndicator indicator;
                    indicator.type = "FileBlocked";
                    indicator.description = "Antivirus blocked file access";
                    indicator.weight = 20;
                    indicator.timingCorrelation = 0.9;
                    indicators.push_back(indicator);
                    break;
                }
            }

            // Indicator 4: Performance Degradation
            InterferenceIndicator perfIndicator;
            perfIndicator.type = "PerformanceDegradation";
            perfIndicator.description = "Process performance degraded during AV activity";
            perfIndicator.weight = 15;
            perfIndicator.timingCorrelation = 0.7;
            indicators.push_back(perfIndicator);

            result.indicators = indicators;

            // Calculate confidence score
            int totalScore = 0;
            int maxScore = 0;

            for (const auto& indicator : indicators) {
                int score = static_cast<int>(indicator.weight * indicator.timingCorrelation);
                totalScore += score;
                maxScore += indicator.weight;
            }

            result.confidenceScore = maxScore > 0 ? (totalScore * 100) / maxScore : 0;
            result.interferenceDetected = result.confidenceScore >= 70;

            // Determine primary cause
            if (!indicators.empty()) {
                auto primaryIt = std::max_element(indicators.begin(), indicators.end(),
                    [](const InterferenceIndicator& a, const InterferenceIndicator& b) {
                        return (a.weight * a.timingCorrelation) < (b.weight * b.timingCorrelation);
                    });
                result.primaryCause = primaryIt->type;
            }

            // Generate recommendations
            if (result.interferenceDetected) {
                if (result.primaryCause == "ProcessBlocked") {
                    result.recommendations.push_back(
                        "Add target process to antivirus exclusion list");
                    result.recommendations.push_back(
                        "Review antivirus policy for this process type");
                }
                
                if (result.primaryCause == "FileBlocked") {
                    result.recommendations.push_back(
                        "Check if blocked file is legitimate application component");
                    result.recommendations.push_back(
                        "Contact application vendor for digital signature/certification");
                }

                result.recommendations.push_back(
                    "Update antivirus signatures and engine to latest version");
                result.recommendations.push_back(
                    "Consider temporarily disabling real-time protection for testing");
            }

            return result;
        }
    };
}

/// <summary>
/// Main demonstration of interference analysis
/// </summary>
int main() {
    std::cout << "=== Interference Analysis - C++ PoC ===\n\n";

    try {
        // Simulate collected data
        std::vector<std::string> avEvents = {
            "[10:30:21] Windows Defender - Scan started",
            "[10:30:23] Windows Defender - Threat detected: app.exe",
            "[10:30:24] Windows Defender - Action: Quarantine file"
        };

        std::string processStatus = "CRASHED";
        double processCrashTime = 10.025; // 10.025 seconds into monitoring
        double avActivityTime = 10.024;   // 10.024 seconds (1ms before crash)

        std::cout << "Input Data:\n";
        std::cout << "  AV Events: " << avEvents.size() << "\n";
        std::cout << "  Process Status: " << processStatus << "\n";
        std::cout << "  Crash Time: " << std::fixed << std::setprecision(3) 
                  << processCrashTime << "s\n";
        std::cout << "  AV Activity Time: " << avActivityTime << "s\n\n";

        auto result = AVDetector::InterferenceAnalyzer::AnalyzeInterference(
            avEvents, processStatus, processCrashTime, avActivityTime);

        std::cout << "\n=== ANALYSIS RESULTS ===\n\n";

        std::cout << "Interference Detected: " 
                  << (result.interferenceDetected ? "YES" : "NO") << "\n";
        std::cout << "Confidence Score: " << result.confidenceScore << "%\n";
        std::cout << "Primary Cause: " << result.primaryCause << "\n\n";

        std::cout << "Detected Indicators:\n";
        for (size_t i = 0; i < result.indicators.size(); i++) {
            const auto& ind = result.indicators[i];
            int score = static_cast<int>(ind.weight * ind.timingCorrelation);
            std::cout << "  " << (i + 1) << ". " << ind.type << "\n";
            std::cout << "     Description: " << ind.description << "\n";
            std::cout << "     Weight: " << ind.weight << " | "
                      << "Timing Correlation: " << std::fixed << std::setprecision(2) 
                      << ind.timingCorrelation << " | "
                      << "Score: " << score << "\n\n";
        }

        if (!result.recommendations.empty()) {
            std::cout << "Recommendations:\n";
            for (size_t i = 0; i < result.recommendations.size(); i++) {
                std::cout << "  " << (i + 1) << ". " << result.recommendations[i] << "\n";
            }
        }

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "EXCEPTION: " << ex.what() << "\n";
        return 1;
    }
}

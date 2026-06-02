#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace AVDetector {

    /// <summary>
    /// Represents a process in the system
    /// </summary>
    struct ProcessInfo {
        DWORD processId;
        std::string processName;
        DWORD parentProcessId;
        std::chrono::system_clock::time_point createTime;
        std::chrono::system_clock::time_point exitTime;
        int exitCode;
        bool isRunning;

        ProcessInfo();
        ~ProcessInfo() = default;
    };

    /// <summary>
    /// Monitors and tracks process hierarchies
    /// </summary>
    class ProcessMonitor {
    public:
        /// <summary>
        /// Find a process by name (case-insensitive)
        /// </summary>
        static std::unique_ptr<ProcessInfo> FindProcessByName(const std::string& processName);

        /// <summary>
        /// Get all child processes for a given parent PID
        /// </summary>
        static std::vector<ProcessInfo> GetChildProcesses(DWORD parentPid);

        /// <summary>
        /// Get all descendants (recursive) of a process
        /// </summary>
        static std::vector<ProcessInfo> GetAllDescendants(DWORD rootPid);

        /// <summary>
        /// Wait for a process to launch
        /// </summary>
        static std::unique_ptr<ProcessInfo> WaitForProcessByName(
            const std::string& processName, 
            int timeoutSeconds = 30);

        /// <summary>
        /// Check if a process is still running
        /// </summary>
        static bool IsProcessRunning(DWORD processId);

        /// <summary>
        /// Get exit code of a terminated process
        /// </summary>
        static int GetProcessExitCode(DWORD processId);
    };

} // namespace AVDetector

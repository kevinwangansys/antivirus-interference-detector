# C++ Proof of Concept - Antivirus Interference Detector

## Overview

This directory contains proof-of-concept C++ implementations demonstrating core monitoring capabilities using native Windows APIs. All code is optimized for minimal dependencies and maximum performance.

## Components

### 1. Process Monitor Demo (`ProcessMonitorDemo.cpp`)
**Purpose**: Track parent/child process relationships in real-time

**Technologies**:
- `CreateToolhelp32Snapshot()` - Efficient process enumeration
- `Process32First/Process32Next()` - Walk process tree
- `GetProcessTimes()` - Process timing information

**Features**:
- Find process by name or PID
- Display full process hierarchy tree
- Monitor process creation/termination
- Get process metrics (start time, exit code, status)

**Output Example**:
```
Process Tree for: app.exe (PID: 1234)
├─ app.exe (1234) [Running] Started: 10:30:15
│  ├─ helper.exe (1235) [Running] Started: 10:30:16
│  │  └─ worker.exe (1237) [Running] Started: 10:30:17
│  └─ service.exe (1236) [Exited] Code: -1073741819 (0xc0000005)
```

### 2. Antivirus Detection Demo (`AVDetectionDemo.cpp`)
**Purpose**: Detect ALL installed antivirus products

**Technologies**:
- `RegOpenKeyEx/RegEnumKeyEx()` - Registry scanning
- WMI COM interfaces - `IWbemLocator`, `IWbemServices`
- Windows Service enumeration - `OpenSCManager`, `EnumServicesStatus`

**Features**:
- Registry-based detection (Security Center, Programs, Defender)
- WMI AntivirusProduct queries
- Windows Service scanning for known AV services
- Confidence scoring for each detection method
- Deduplication and consolidation

**Output Example**:
```
Detected Antivirus Products:

1. Windows Defender
   Status: Active
   Detection Method: Registry (Windows Defender key)
   Confidence: 100%

2. CrowdStrike Falcon
   Status: Active (Running)
   Service Name: csfalcon
   Detection Method: Windows Service + Registry
   Confidence: 95%

3. Custom Security Tool v2.1
   Status: Active
   Detection Method: Registry (Programs)
   Confidence: 75%

Total: 3 antivirus products detected
```

### 3. Performance Metrics Demo (`PerformanceMetricsDemo.cpp`)
**Purpose**: Real-time performance monitoring for target process

**Technologies**:
- `GetProcessMemoryInfo()` - Memory metrics (working set, private)
- `GetProcessIoCounters()` - Disk I/O statistics
- Performance Counters - CPU usage calculation
- High-resolution timer - Accurate sampling

**Features**:
- CPU usage percentage
- Memory monitoring (working set, private, peak)
- Disk I/O (bytes read/written per second)
- Thread and handle counting
- Configurable sampling intervals
- Detection of spikes and anomalies
- Statistical summary (min, max, average)

**Output Example**:
```
⏱️ Performance Metrics for: app.exe (PID: 1234)

Timestamp          | CPU %  | Memory (MB) | Threads | Status
-------------------|--------|-------------|---------|--------
10:30:20.123       | 5.2%   | 128         | 4       | Running
10:30:21.125       | 45.8%  | 256         | 8       | Running ⚠️ SPIKE
10:30:22.127       | 52.1%  | 300         | 12      | Running ⚠️ SPIKE
10:30:25.132       | 0.0%   | 0           | 0       | CRASHED

Summary Statistics:
  Average CPU: 25.7%
  Peak Memory: 300 MB
  Duration: 5.009 seconds
  Exit Code: -1 (0xffffffff)
```

### 4. Event Log Scanner Demo (`EventLogScannerDemo.cpp`)
**Purpose**: Scan all Windows event logs for security-related events

**Technologies**:
- `EvtOpenLog()` - Open event log channels
- `EvtQuery()` - Query events with filters
- `EvtNext()` - Enumerate events efficiently
- XML parsing - Parse complex event data

**Features**:
- Scan all event log channels (System, Security, Application, Custom)
- Filter for security keywords (threat, virus, suspicious, etc.)
- Time-window based correlation with process events
- Event severity classification
- Support for unknown/custom antivirus products

**Output Example**:
```
Event Log Scan Results:

Channel: System
  Events found: 2
  [10:30:21] Antivirus scan started
  [10:30:23] Antivirus action: Quarantine file

Channel: Security
  Events found: 1
  [10:30:22] Access denied to file: C:\Users\user\Downloads\file.exe

Channel: Application
  Events found: 0

Channel: Microsoft-Windows-Windows Defender/Operational
  Events found: 3
  [10:30:21] Scan initiated
  [10:30:23] Threat detected: Malware.Generic
  [10:30:24] Quarantine action taken

Total events in monitoring window: 6
Events correlating with target process: 4
```

### 5. Interference Analysis Demo (`InterferenceAnalysisDemo.cpp`)
**Purpose**: Correlate AV events with process behavior to detect interference

**Technologies**:
- Event correlation engine
- Timing analysis (nanosecond precision)
- Confidence scoring algorithm
- Statistical analysis of metrics

**Features**:
- Match process events (crash, hang, slow startup) with AV events
- Calculate timing correlation (how close in time)
- Assign interference confidence score (0-100%)
- Generate recommendations based on detected patterns
- Detailed event timeline with causation analysis

**Output Example**:
```
Interference Analysis Results:

Process: app.exe (PID: 1234)
Monitoring Period: 2026-06-02 10:30:15 - 10:30:25 (10.0 seconds)

Detected Indicators:
  ✓ Process Crashed (exit code: -1)
    Time: 10:30:25.000
  ✓ AV Threat Detected
    Time: 10:30:23.500
    Delta: 1.5 seconds BEFORE crash
  ✓ AV Quarantine Action
    Time: 10:30:24.000
    Delta: 1.0 seconds BEFORE crash
  ✓ File Access Denied by AV
    Time: 10:30:24.500
    Delta: 0.5 seconds BEFORE crash

Interference Confidence: 88%
Primary Cause: ProcessBlocked

Affected AV Products:
  - Windows Defender: 3 events (threat detected, quarantine, file access)

Recommendations:
  1. Add app.exe to Windows Defender exclusions list
  2. Check if file is legitimately needed by application
  3. Contact application vendor for compatibility notes
  4. Consider updating Windows Defender signature database
```

---

## Building the PoC

### Prerequisites
- **Visual Studio 2022** (with C++ workload)
- **CMake 3.20+**
- **vcpkg** (for dependency management)
- **Windows 10 or later**

### Build Steps

```bash
# Clone the repository
git clone https://github.com/kevinwangansys/antivirus-interference-detector.git
cd antivirus-interference-detector

# Install dependencies (if using vcpkg)
vcpkg install nlohmann-json:x64-windows

# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release

# Run individual demos
.\Release\ProcessMonitorDemo.exe app.exe
.\Release\AVDetectionDemo.exe
.\Release\PerformanceMetricsDemo.exe notepad 1000 10
.\Release\EventLogScannerDemo.exe
.\Release\InterferenceAnalysisDemo.exe
```

### Alternative: Command Line Build

```bash
# Using nmake (simpler)
cd build
cmake .. -G "NMake Makefiles"
nmake
```

---

## Project Structure

```
poc/cpp/
├── src/
│   ├── ProcessMonitorDemo.cpp      (700 lines)
│   ├── AVDetectionDemo.cpp         (650 lines)
│   ├── PerformanceMetricsDemo.cpp  (550 lines)
│   ├── EventLogScannerDemo.cpp     (600 lines)
│   └── InterferenceAnalysisDemo.cpp (700 lines)
├── include/
│   ├── ProcessMonitor.h
│   ├── AVDetector.h
│   ├── PerformanceCollector.h
│   ├── EventLogScanner.h
│   └── InterferenceAnalyzer.h
├── CMakeLists.txt
└── README.md
```

---

## Key C++ Features Used

### Modern C++17 Idioms
- **Smart pointers**: `std::unique_ptr`, `std::shared_ptr`
- **Optional values**: `std::optional` for nullable returns
- **Structured bindings**: `auto [pid, name] = process;`
- **std::variant**: For type-safe unions
- **std::string_view**: Efficient string handling

### Windows API Patterns
- **RAII for handles**: Automatic cleanup of process/registry handles
- **Error handling**: Checked return codes with descriptive messages
- **COM initialization**: `CoInitializeEx/CoUninitialize`
- **Unicode support**: `UNICODE` macro for wide character handling

### Performance Optimizations
- **Minimal allocations**: Stack-based buffers where possible
- **Efficient enumeration**: Snapshot-based process walking
- **Event batching**: Read multiple events per API call
- **Lazy initialization**: Load only needed components

---

## Testing the PoC

### Test Scenario 1: Normal Application
```bash
# Launch a normal app and monitor it
ProcessMonitorDemo.exe notepad
# Expected: Process runs normally, no interference detected
```

### Test Scenario 2: Windows Defender Active
```bash
# Monitor with Windows Defender running
AVDetectionDemo.exe
# Expected: Windows Defender detected with 100% confidence
```

### Test Scenario 3: Performance During Scan
```bash
# Monitor performance while AV scan occurs
PerformanceMetricsDemo.exe calc 500 30
# Expected: CPU/Memory spikes visible when scan starts
```

### Test Scenario 4: Event Log Correlation
```bash
# Scan event logs during known issue
EventLogScannerDemo.exe
# Expected: Security events visible from last 1 hour
```

---

## Compilation Details

### Compiler Flags (Visual Studio)
```
/std:c++latest     # Use latest C++ standard
/permissive-       # Strict standard conformance
/W4                # Warning level 4
/WX                # Warnings as errors
/O2                # Optimization for speed
/Zi                # Debug info
```

### Linker Dependencies
```
- Ole32.lib         # COM interfaces
- OleAut32.lib      # Automation
- Wbemuuid.lib      # WMI
- Wmiutils.lib      # WMI utilities
- Psapi.lib         # Process API
- WevtApi.lib       # Event Log API
- Advapi32.lib      # Registry & Services
```

---

## Expected Output Summary

### ProcessMonitorDemo
- Process tree visualization
- Hierarchy up to 3+ levels deep
- Real-time status updates

### AVDetectionDemo
- List of all detected AV products
- Detection method for each
- Confidence scores
- Deduplication results

### PerformanceMetricsDemo
- Real-time metric updates
- Column-formatted output
- Summary statistics at end
- Anomaly detection (spikes)

### EventLogScannerDemo
- Events organized by channel
- Timestamps and descriptions
- Correlation with monitoring window
- Filtered security events only

### InterferenceAnalysisDemo
- Detailed interference analysis
- Confidence percentage
- Root cause identification
- Actionable recommendations

---

## Known Limitations & Future Improvements

### Current Limitations
- ⚠️ Requires admin privileges (by design)
- ⚠️ Only monitors during session (doesn't persist)
- ⚠️ Single-threaded (sequential processing)
- ⚠️ No GUI (CLI only for PoC)

### Future Enhancements
- ✅ Multi-threaded monitoring (phase 2)
- ✅ Real-time event-based updates (phase 2)
- ✅ Dear ImGui GUI (phase 2)
- ✅ Persistent logging (phase 3)
- ✅ Report generation (phase 3)

---

## Architecture Diagram

```
┌─────────────────────────────────────────────┐
│   Main Application (CLI)                    │
└────────────────┬────────────────────────────┘
                 │
    ┌────────────┼────────────┬─────────────┐
    │            │            │             │
    v            v            v             v
┌─────────┐ ┌─────────┐ ┌──────────┐ ┌──────────────┐
│ Process │ │    AV   │ │Performance│ │ Event Log   │
│ Monitor │ │Detector │ │Collector │ │  Scanner   │
└────┬────┘ └────┬────┘ └────┬─────┘ └──────┬──────┘
     │           │           │              │
     └───────────┼───────────┼──────────────┘
                 │           │
                 v           v
            ┌─────────────────────────┐
            │ Interference Analyzer   │
            │ (Correlation Engine)    │
            └────────┬────────────────┘
                     │
                     v
            ┌─────────────────────────┐
            │ Report Generator        │
            │ (JSON/CSV/HTML)         │
            └─────────────────────────┘
```

---

## References

- [Windows Process Creation API](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/)
- [Windows Event Log API](https://docs.microsoft.com/en-us/windows/win32/eventlog/event-logging)
- [WMI C++ Application](https://docs.microsoft.com/en-us/windows/win32/wmisdk/creating-a-wmi-application)
- [Windows Registry](https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry)
- [C++17 Standard](https://en.cppreference.com/)

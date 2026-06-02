# Proof of Concept - Antivirus Interference Detector

This directory contains proof-of-concept demonstrations for the core components of the AV Interference Detector using existing open-source libraries.

## Components Demonstrated

### 1. ETW (Event Tracing for Windows) - Real-time AV Event Capture
- Uses: `Microsoft.Diagnostics.Tracing.TraceEvent`
- Demonstrates: Capturing Windows security events in real-time
- File: `TraceEventDemo.cs`

### 2. WMI Process Hierarchy Tracking
- Uses: `System.Management` (built-in)
- Demonstrates: Finding child processes, monitoring process creation
- File: `ProcessHierarchyDemo.cs`

### 3. Event Log Scanning - Generic AV Detection
- Uses: `System.Diagnostics.EventLog` (built-in)
- Demonstrates: Scanning all event logs for security-related events
- File: `EventLogScannerDemo.cs`

### 4. Performance Metrics Collection
- Uses: `System.Diagnostics.Process` (built-in)
- Demonstrates: Real-time CPU, memory, disk I/O monitoring
- File: `PerformanceMetricsDemo.cs`

### 5. Antivirus Detection - Registry & Services
- Uses: `Microsoft.Win32.RegistryKey` (built-in)
- Demonstrates: Detecting all installed security software
- File: `AVDetectionDemo.cs`

### 6. Interference Detection Algorithm
- Demonstrates: Correlating AV events with process behavior
- File: `InterferenceAnalysisDemo.cs`

## Setup & Requirements

### Prerequisites
- Windows 10 or later
- .NET 6+ or .NET Framework 4.8+
- Visual Studio 2022 (or later) with C# support
- Administrator privileges (required for event log and ETW access)

### Required NuGet Packages
```bash
dotnet add package Microsoft.Diagnostics.Tracing.TraceEvent
```

### Building
```bash
dotnet build
```

### Running
```bash
# Run all demos
dotnet run

# Or run specific demo (after implementation)
dotnet run --project Demos/TraceEventDemo.csproj
```

**Important**: Must run as Administrator
```bash
# PowerShell
Start-Process powershell -Verb RunAs
cd path\to\project
dotnet run
```

## What Each Demo Shows

### 1. TraceEventDemo.cs
**Purpose**: Capture AV events in real-time using ETW

**Key Features**:
- Enables Windows Defender ETW provider
- Listens for security-related events
- Displays events as they occur
- Shows event details (severity, threat name, action taken)

**Output Example**:
```
[2026-06-02 10:30:23] Windows Defender - ThreatDetected
  Threat: Malware.Generic
  File: C:\Users\user\Downloads\file.exe
  Action: Quarantine

[2026-06-02 10:30:24] Windows Defender - ScanStarted
  Type: Full Scan
```

### 2. ProcessHierarchyDemo.cs
**Purpose**: Track parent/child process relationships using WMI

**Key Features**:
- Find process by name or pattern
- Get all child processes recursively
- Track process creation/termination events
- Build process tree visualization

**Output Example**:
```
📋 Process Tree for: app.exe (PID: 1234)
├─ app.exe (1234) - Running [10:30:15]
│  ├─ helper.exe (1235) - Running [10:30:16]
│  └─ service.exe (1236) - Crashed [10:30:25] Exit: -1
```

### 3. EventLogScannerDemo.cs
**Purpose**: Scan all event logs for security-related events (generic AV detection)

**Key Features**:
- Enumerate all event log channels
- Filter for security keywords (threat, virus, suspicious, etc.)
- Detect events from any AV product
- Map events to time windows

**Output Example**:
```
🔍 Event Log Scan Results:
Security log: 2 events
  [10:30:21] Antivirus scan started
  [10:30:23] Threat detected: app.exe

Application log: 1 event
  [10:30:24] CrowdStrike suspicious process behavior

Custom AV logs: 0 events
```

### 4. PerformanceMetricsDemo.cs
**Purpose**: Collect real-time performance metrics for monitored process

**Key Features**:
- Monitor CPU usage (%)
- Monitor memory (working set, private bytes, peak)
- Monitor disk I/O (bytes read/written)
- Sample at configurable intervals
- Detect spikes and anomalies

**Output Example**:
```
⏱️ Performance Metrics for: app.exe (PID: 1234)
Timestamp: 10:30:20
  CPU: 5% | Memory: 128 MB | Disk I/O: 0.5 MB/s

Timestamp: 10:30:21
  CPU: 45% | Memory: 256 MB | Disk I/O: 2.1 MB/s ⚠️ SPIKE

Timestamp: 10:30:25
  CPU: 0% | Memory: 0 MB | Status: CRASHED ❌
```

### 5. AVDetectionDemo.cs
**Purpose**: Detect ALL installed antivirus products

**Key Features**:
- Registry scanning for security software
- Windows Service enumeration
- WMI AntivirusProduct queries
- Registry detection for known products
- Fallback generic detection

**Output Example**:
```
🛡️ Detected Antivirus Products:
1. Windows Defender
   Status: Active
   Detection Method: Registry + WMI
   Confidence: 100%

2. CrowdStrike Falcon
   Status: Active
   Service: csfalcon
   Detection Method: Service + Registry
   Confidence: 95%

3. Custom Security Tool v2.1
   Status: Active
   Detection Method: Event Log
   Confidence: 75%

Total: 3 products detected
```

### 6. InterferenceAnalysisDemo.cs
**Purpose**: Correlate AV events with process behavior to detect interference

**Key Features**:
- Match process events (crash, hang, slow startup) with AV events
- Calculate timing correlation
- Assign interference confidence score
- Generate recommendations

**Output Example**:
```
📊 Interference Analysis Results:
Process: app.exe (PID: 1234)
Monitoring Period: 2026-06-02 10:30:15 - 10:30:25

Detected Indicators:
  ✓ Process Crashed (exit code: -1)
  ✓ AV Threat Detected 2 seconds before crash
  ✓ AV Action (Quarantine) taken 1 second before crash
  ✓ File Access Denied by AV

Interference Confidence: 88%
Primary Cause: ProcessBlocked

Affected AV Products:
  - Windows Defender: 2 blocking events

Recommendations:
  1. Add app.exe to Windows Defender exclusions
  2. Review process trust settings
  3. Check application vendor compatibility notes
```

## Architecture

```
PoC/
├── Demos/
│   ├── TraceEventDemo.cs          (ETW real-time monitoring)
│   ├── ProcessHierarchyDemo.cs    (Process tree tracking)
│   ├── EventLogScannerDemo.cs     (Generic event log scanning)
│   ├── PerformanceMetricsDemo.cs  (Performance monitoring)
│   ├── AVDetectionDemo.cs         (AV product detection)
│   └── InterferenceAnalysisDemo.cs (Correlation & analysis)
├── Models/
│   ├── AVEvent.cs                 (AV event data structure)
│   ├── ProcessEvent.cs            (Process event data structure)
│   ├── PerformanceMetrics.cs      (Metrics snapshot)
│   └── InterferenceScore.cs       (Analysis results)
├── Utilities/
│   ├── ETWHelper.cs               (TraceEvent wrapper)
│   ├── ProcessHelper.cs           (WMI process queries)
│   ├── EventLogHelper.cs          (Event log scanning)
│   └── CorrelationEngine.cs       (Timing correlation)
└── Program.cs                      (Main entry point)
```

## Key Learnings from PoC

### What Works Well
- ✅ TraceEvent captures real-time events efficiently
- ✅ WMI provides accurate parent/child process relationships
- ✅ Event log scanning is effective for generic AV detection
- ✅ Built-in APIs handle performance metrics well

### Challenges & Solutions
- ⚠️ ETW requires admin privileges → Already specified in requirements
- ⚠️ WMI queries have slight delay → Acceptable for monitoring use case
- ⚠️ Event log has retention limits → Capture events at monitoring time
- ⚠️ Unknown AV products need keyword matching → Generic event scanning handles this

## Next Steps

After PoC validation, we'll move to:
1. **Step 2**: Create project structure with dependencies integrated
2. **Step 3**: Build core engine modules that integrate all these components

## References

- [Microsoft TraceEvent Library](https://github.com/microsoft/perfview/tree/main/src/TraceEvent)
- [WMI Process Queries](https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-process)
- [Event Log Reading in C#](https://docs.microsoft.com/en-us/dotnet/api/system.diagnostics.eventlog)
- [Windows ETW Documentation](https://docs.microsoft.com/en-us/windows/win32/etw/event-tracing-portal)

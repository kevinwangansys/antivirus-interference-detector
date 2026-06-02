# Antivirus Interference Detector - Technical Specification

## 1. Executive Summary
A standalone application that allows IT admins and technical support engineers to detect whether application crashes or performance issues are caused by **any antivirus/endpoint protection software** by monitoring externally-launched processes and correlating application behavior with system-wide antivirus activity.

**Key Difference from traditional approach**: The tool monitors externally-launched processes rather than launching them internally. This allows real-world testing where applications are started normally through their usual entry points.

---

## 2. Use Case & Audience
- **Primary Users**: IT administrators, technical support engineers, system administrators
- **Problem**: Application crashes with unclear root cause — is it the antivirus?
- **Solution**: Start the tool in monitoring mode, then launch the application normally and observe if issues occur while monitoring all antivirus activity on the system
- **Workflow**:
  1. Start the AV Interference Detector tool
  2. Configure monitoring settings (which process to watch, what metrics to collect)
  3. Start capturing
  4. Launch the target application through normal means (shortcut, command line, installer, etc.)
  5. Reproduce the issue
  6. View live results or review logs after the issue occurs

---

## 3. Scope & Capabilities

### 3.1 Antivirus Support
- **Universal AV Detection**: Automatically detects ANY installed antivirus/endpoint protection software
- **Common Products** (out-of-box support):
  - Windows Defender
  - CrowdStrike Falcon
  - SentinelOne
  - Symantec (Norton)
  - McAfee
  - Kaspersky
  - AVG
  - Avast
  - Bitdefender
  - ESET
  - Trend Micro
  - Sophos
  - F-Secure
  - And many others...
- **Extensible Detection**: Designed to detect unknown/custom antivirus products through generic event log and registry scanning

### 3.2 Monitoring Capabilities
- Monitor externally-launched processes (by name or PID pattern)
- Detect all child processes created by target process
- Measure performance impact (CPU, memory, disk I/O, network)
- Detect interference (blocking, crashes, resource constraints)
- Real-time live view of metrics and AV activity from ANY installed antivirus
- Track interference for main process and all child processes
- Correlate application behavior with system-wide security software activity

### 3.3 What NOT to Do
- Do not launch or control the target application
- Do not modify antivirus settings
- Do not disable/enable antivirus (read-only monitoring only)
- Do not change quarantine policies
- Do not inject code into monitored processes

---

## 4. Architecture Overview

### 4.1 Application Structure
```
AV-Interference-Detector/
├── src/
│   ├── CLI/                          # Command-line interface
│   │   ├── Program.cs
│   │   └── CommandHandler.cs
│   ├── GUI/                          # Graphical user interface (WPF)
│   │   ├── App.xaml
│   │   ├── MainWindow.xaml
│   │   ├── Views/
│   │   │   ├── ConfigurationPanel.xaml
│   │   │   ├── MonitoringPanel.xaml
│   │   │   └── ResultsPanel.xaml
│   │   └── ViewModels/
│   │       ├── MainViewModel.cs
│   │       ├── ConfigurationViewModel.cs
│   │       ├── MonitoringViewModel.cs
│   │       └── ResultsViewModel.cs
│   ├── Core/
│   │   ├── ProcessMonitor.cs          # Monitor external process and children
│   │   ├── ProcessDiscovery.cs        # Find target process by name/pattern/PID
│   │   ├── PerformanceCollector.cs    # CPU, memory, disk I/O metrics
│   │   ├── AVDetector.cs              # Detect ANY installed AV products
│   │   ├── AVActivityMonitor.cs       # Monitor system-wide AV activity
│   │   ├── InterferenceAnalyzer.cs    # Analyze data & detect interference
│   │   ├── ReportGenerator.cs         # Generate reports (JSON, CSV, HTML)
│   │   └── MonitoringSession.cs       # Manage monitoring session lifecycle
│   ├── AVIntegration/
│   │   ├── GenericAVMonitor.cs        # Universal AV activity detector
│   │   ├── EventLogMonitor.cs         # Generic Windows Event Log scanner
│   │   ├── RegistryMonitor.cs         # Registry scanning for AV indicators
│   │   ├── PredefinedAVMonitors/      # Optimized monitors for known products
│   │   │   ├── WindowsDefenderMonitor.cs
│   │   │   ├── CrowdStrikeMonitor.cs
│   │   │   ├── SentinelOneMonitor.cs
│   │   │   ├── SymantecMonitor.cs
│   │   │   ├── McAfeeMonitor.cs
│   │   │   ├── KasperskyMonitor.cs
│   │   │   └── ... (other common products)
│   │   ├── UnknownAVHandler.cs        # Handle unknown antivirus products
│   │   └── IAVMonitor.cs              # Interface for all monitors
│   ├── Models/
│   │   ├── MonitoringSession.cs
│   │   ├── ProcessMetrics.cs
│   │   ├── AVActivity.cs
│   │   ├── InterferenceReport.cs
│   │   ├── AnalysisResult.cs
│   │   ├── ProcessSnapshot.cs
│   │   ├── MetricsSnapshot.cs
│   │   └── AVProduct.cs               # Generic AV product information
│   ├── Utils/
│   │   ├── EventLogReader.cs
│   │   ├── RegistryReader.cs
│   │   ├── WMIHelper.cs
│   │   ├── FileHelper.cs
│   │   ├── ProcessHelper.cs
│   │   └── SystemInfoHelper.cs        # Detect all installed security software
│   └── Services/
│       ├── RealTimeMonitoringService.cs
│       └── DataAggregationService.cs
├── tests/
│   ├── UnitTests/
│   │   ├── ProcessMonitorTests.cs
│   │   ├── AVDetectorTests.cs
│   │   ├── AVActivityMonitorTests.cs
│   │   └── InterferenceAnalyzerTests.cs
│   └── IntegrationTests/
│       └── EndToEndTests.cs
├── docs/
│   ├── SETUP.md
│   ├── USER_GUIDE.md
│   ├── CLI_REFERENCE.md
│   ├── ARCHITECTURE.md
│   └── AV_DETECTION_STRATEGY.md
├── AV-Interference-Detector.sln
├── README.md
├── SPECIFICATION.md
└── .gitignore
```

### 4.2 Technology Stack
- **Language**: C# (.NET 6+ or .NET Framework 4.8)
- **GUI Framework**: WPF (Windows Presentation Foundation)
- **CLI Framework**: System.CommandLine
- **Monitoring**: WMI, ETW (Event Tracing for Windows), Windows Event Log, Performance Counters, Registry
- **Process Management**: System.Diagnostics.Process, Job Objects
- **Report Generation**: JSON (System.Text.Json), CSV (CsvHelper), HTML (custom templates)
- **Async/Concurrency**: async/await, BackgroundWorker for GUI

---

## 5. Antivirus Detection Strategy

### 5.1 Generic AV Detection Methods

The tool uses multiple detection strategies to identify ANY antivirus software:

#### Method 1: Windows Registry Scanning
```
Common Registry Locations:
HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall\*
HKLM\Software\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*
HKLM\Software\Microsoft\Windows Defender\
HKLM\Software\Classes\CLSID\

Search keywords: "antivirus", "security", "protection", "defender", "endpoint", 
"threat", "malware", "virus", "AV", "security software"
```

#### Method 2: Windows Event Log Scanning
```
Common Event Log Channels:
- System
- Security
- Application
- Microsoft-Windows-Windows Defender/*
- Custom vendor channels (if present)

Search for events containing:
- "security", "threat", "antivirus", "malware", "quarantine", 
"virus", "suspicious", "protection", "scan", "blocked"
```

#### Method 3: Running Processes Analysis
```
Scan running processes for known AV-related service names:
- Services: csfalcon, sentinelctl, avast, kaspersky, mcafee, norton, etc.
- Process names: avgidsagent, cmdagent, cscript, etc.
- Executable paths: C:\Program Files\*Security*, C:\Program Files\*Protect*, etc.
```

#### Method 4: WMI Queries
```
Query: SELECT * FROM AntivirusProduct
Query: SELECT * FROM FirewallProduct
From namespace: root\SecurityCenter2
```

#### Method 5: Service Analysis
```
Enumerate all Windows services and identify security-related ones:
- Service display names containing security keywords
- Service start types
- Service status (running/stopped)
```

### 5.2 Predefined AV Monitors (Optimized Detection)

For well-known antivirus products, optimized monitors provide:
- Specific event log channel names
- Known registry locations
- Expected service names
- Specific event IDs for threats/blocks
- Vendor-specific API/file locations

**Benefit**: Faster and more accurate detection with less false positives

### 5.3 Unknown AV Handling

For unknown/custom antivirus products:
- Fall back to generic detection methods
- Enumerate all security-related processes and services
- Monitor all event log channels for security-related events
- Track registry changes in security-related keys
- Display detected AV product(s) with confidence level

### 5.4 AV Activity Detection - Universal Approach

#### Generic Event Log Monitoring
```
Monitor all event logs for security-related events:
- Event keyword: "threat", "suspicious", "malware", "virus", "quarantine", 
  "blocked", "detected", "protection", "scan", "remediated"
- Event severity: Warning, Error, Critical
- Event source: Any security-related source
```

#### Generic Registry Monitoring
```
Monitor for writes to security-related keys:
HKLM\Software\Microsoft\Windows\CurrentVersion\Run
HKLM\Software\Microsoft\Windows NT\CurrentVersion\Drivers
HKLM\Software\*Security*
HKLM\Software\*Antivirus*
HKLM\Software\*Protection*
```

#### Generic Process Activity Monitoring
```
Track when security software processes:
- Spawn child processes
- Access target process files
- Load DLLs
- Modify files in quarantine/security folders
```

---

## 5. Feature Specifications

## 5.1 Overall Workflow

### Phase 1: Configuration
1. User launches the tool
2. User specifies what to monitor:
   - By process name (e.g., "app.exe", "installer.exe")
   - By partial name pattern (e.g., "*app*" matches "MyApp.exe", "app-launcher.exe")
   - Alternatively, wait for first process matching pattern to appear
3. Tool auto-detects all installed antivirus products and displays them
4. User confirms or customizes AV monitoring (can monitor all or specific products)
5. User selects advanced options (performance metrics, tracking depth)
6. User clicks "Start Capturing"

### Phase 2: Monitoring (Active)
1. Tool waits for target process to appear
2. Tool simultaneously monitors system-wide antivirus activity
3. When target process is found:
   - User sees confirmation in UI: "Target process found: app.exe (PID: 1234)"
   - Focused monitoring begins immediately
4. User is free to reproduce the issue (let app run, interact with it, trigger crash)
5. Tool continuously:
   - Tracks main process + child processes
   - Collects performance metrics
   - Monitors antivirus activity from all detected products
   - Displays live metrics in UI

### Phase 3: Analysis & Reporting
1. After issue occurs or monitoring time expires:
   - User can stop capturing
   - Or let tool continue running
2. View live results in UI or download full report
3. Report shows correlation between AV activity and process behavior

---

## 5.2 Configuration Panel (GUI)

### Main Elements

**Section 1: Target Process Selection**
- **Option A**: Process name input
  - Text field: Enter executable name (e.g., "app.exe" or "*app*")
  - Description: "Exact name or wildcard pattern"
  - Help text: "Examples: calc.exe, *installer*, myapp*"

- **Option B**: Wait for process
  - Checkbox: "Wait for process matching pattern"
  - Auto-select when process appears

- **Option C**: Select by PID (advanced)
  - Text field: Enter process ID
  - "Refresh" button to get current running processes

**Section 2: Antivirus Selection**
- Auto-detect button
  - Shows ALL detected antivirus products on system
  - Example output:
    ```
    ✓ Windows Defender (Active)
    ✓ CrowdStrike Falcon (Active)
    ✓ Custom Security Tool v2.1 (Active, Unknown Product)
    ✓ McAfee Endpoint Security (Installed, Not Running)
    ```
  - Checkboxes to select which to monitor (all checked by default)
  - Confidence indicator for detection accuracy
  - Option to manually add custom AV names not auto-detected

**Section 3: Monitoring Options**
- Monitor child processes: Checkbox (default: checked)
- Track file access: Checkbox (default: unchecked)
- Track network access: Checkbox (default: unchecked)
- Track DLL/module loading: Checkbox (default: unchecked)
- Monitor system-wide AV events: Checkbox (default: checked)

**Section 4: Performance Monitoring**
- Sampling interval: Dropdown (100ms, 500ms, 1000ms, 5000ms) - default: 1000ms
- Metrics to collect: Checkboxes for CPU, Memory, Disk I/O, Network I/O, etc.

**Section 5: Output**
- Output directory: File browser (user-selected)
- Auto-save reports: Checkbox (default: checked)
- Report format: Checkboxes (JSON ✓, CSV, HTML ✓)

**Buttons**:
- "Start Capturing" - Begin monitoring
- "Cancel" - Close without starting
- "Show Advanced" - Expand advanced options
- "Refresh AV List" - Re-scan for installed antivirus

---

## 5.3 Monitoring Panel (Live View - GUI)

Displayed after "Start Capturing" is clicked.

### Real-Time Monitoring Display

**Section 1: Target Process Status**
- Status indicator: [● Waiting] or [● Monitoring] or [● Paused] or [⊙ Stopped]
- Process info (when found):
  - Process name: "app.exe"
  - PID: 1234
  - Start time: "2026-06-02 10:30:15"
  - Elapsed time: "0:02:45"
  - Status: "Running"

**Section 2: Detected Antivirus Products**
- List of all AV products being monitored:
  ```
  ✓ Windows Defender - Events: 2 | Last Event: 2min ago
  ✓ CrowdStrike Falcon - Events: 0 | Last Event: N/A
  ✓ Custom Security Tool - Events: 1 | Last Event: 1min ago
  ```

**Section 3: Live Metrics (Real-time updates)**
- Current CPU: 45.2% (sparkline showing trend)
- Current Memory: 256 MB (sparkline showing trend)
- Current Disk I/O: 1.2 MB/s (sparkline showing trend)
- Current Network I/O: 0.5 MB/s (sparkline showing trend)

**Section 4: Process Tree**
- Hierarchical view of main process + children
- Click on process to see detailed metrics

**Section 5: Antivirus Activity Log (Live)**
- Timeline of detected AV events from ANY product
- Auto-scroll to latest event
- Color coding: Yellow (warning), Red (threat/block), Green (safe)
- Shows product name, event type, description

**Section 6: Live Alerts**
- Display any detected interference in real-time
- Show correlation with AV events from any product

**Buttons**:
- "Pause Monitoring" - Temporarily stop collecting metrics
- "Resume" - Resume monitoring
- "Stop Capturing" - End session and generate report
- "Save Current Report" - Export intermediate results

---

## 5.4 Results Panel (GUI)

Displayed after monitoring session ends.

### Summary Section
- **Interference Detected**: YES / NO / INCONCLUSIVE
- **Confidence Level**: 0-100% (color indicator)
- **Primary Cause**: Process Blocked, Child Process Crashed, Performance Degradation, etc.
- **Affected Processes**: List of main and child processes with status
- **Antivirus Activity Summary**: 
  - Table of each detected AV product with event counts
  - Which product(s) had activity correlated with the issue

### Detailed Analysis Section
- Timeline graph: CPU, Memory, AV Activity over time
- Process details table
- AV events table with product name

### Recommendations Section
- Bullet points with suggested actions
- Identifies which AV product(s) may need configuration changes
- Based on detected interference patterns

---

## 5.5 CLI Interface

### Command Structure

```bash
# Start monitoring with process name
AV-Detector.exe monitor --process "app.exe"

# Start monitoring with pattern matching
AV-Detector.exe monitor --process "*app*"

# Start monitoring with PID
AV-Detector.exe monitor --pid 1234

# With advanced options
AV-Detector.exe monitor `
  --process "app.exe" `
  --antivirus "all" `
  --sample-interval 1000 `
  --track-children true `
  --output-dir "C:\Reports" `
  --output-format "json,html" `
  --timeout 300

# List ALL detected antivirus on system
AV-Detector.exe list-antivirus

# Get status of ALL detected antivirus
AV-Detector.exe av-status

# Show help
AV-Detector.exe monitor --help
```

### CLI Output Examples

```
$ AV-Detector.exe list-antivirus

[INFO] Scanning for installed antivirus software...

=== DETECTED ANTIVIRUS PRODUCTS ===

✓ Windows Defender
  Status: Active
  Location: Built-in
  Detection Method: Registry + Event Log

✓ CrowdStrike Falcon
  Status: Active
  Location: C:\Program Files\CrowdStrike
  Service Name: csfalcon
  Detection Method: Process + Registry

✓ Custom Security Software v2.1
  Status: Active
  Location: Unknown (Detected via Event Log)
  Service Name: CustomSecuritySvc
  Detection Method: Generic Event Log
  Confidence: 85%

Total: 3 antivirus products detected
All will be monitored during capture sessions.
```

---

## 5.6 Antivirus Activity Detection - Universal Approach

### Windows Event Log Scanning

The tool scans ALL event logs and filters for security-related events:

```csharp
// Pseudocode for universal event log scanning
foreach (EventLog log in EventLog.GetEventLogs())
{
    foreach (EventLogEntry entry in log.Entries)
    {
        if (entry.Source.Contains("Security") || 
            entry.Message.Contains("antivirus") || 
            entry.Message.Contains("threat") ||
            entry.Message.Contains("quarantine") ||
            ... other security keywords ...)
        {
            // Correlate with target process timeline
            if (entry.TimeGenerated is within monitoring window)
            {
                RecordAVEvent(entry);
            }
        }
    }
}
```

### Registry-Based Detection

Monitors registry for security software indicators:
- Installed software detection
- Service status changes
- Policy modifications
- Quarantine database changes

### Process-Based Detection

Tracks security software processes:
- Spawning child processes during target app execution
- File access patterns
- DLL injection attempts
- Resource consumption

### Correlation Analysis

When AV activity is detected during target process execution:
1. Record exact timestamp
2. Identify which AV product was active
3. Determine event type (scan, threat, block, etc.)
4. Correlate with target process behavior changes
5. Calculate confidence score for interference

---

## 6. Interference Detection Logic

### Interference Indicators

1. **Process Blocking/Prevention**: Process fails to start or exits immediately with AV activity evident
2. **Process Crashing**: Unexpected termination coincident with AV activity
3. **Child Process Interference**: Child process fails while parent succeeds, with AV activity
4. **Performance Degradation**: CPU/Memory spikes during AV activity windows
5. **Timeout/Hang**: Process unresponsive during AV activity
6. **File Access Denied**: Process file operations fail with AV activity

### Scoring Algorithm

```
confidence = (Σ(indicator_weight × timing_correlation × av_correlation)) / max_possible_weight

Where:
- indicator_weight: Base weight for each type of interference (40-95 points)
- timing_correlation: How closely AV event timing aligns with process event (0.5-1.0)
- av_correlation: Whether AV product was actively scanning/protecting (0.7-1.0)

Example:
  ProcessCrashed (40 pts) × 0.9 timing × 0.95 av_activity = 34.2 pts
  + CorrelatedWithAVEvent (30 pts) × 1.0 timing × 1.0 av_activity = 30 pts
  + FileBlocked (20 pts) × 0.8 timing × 0.9 av_activity = 14.4 pts
  = 78.6 / 100 = 78.6% confidence
```

---

## 7. Report Generation

### Output Formats

#### 1. JSON (Detailed, Machine-Readable)
Contains: session metadata, detected AV products, process tree, metrics timeline, AV activity timeline, analysis results, recommendations

#### 2. CSV (Spreadsheet-Friendly)
Contains: per-timestamp rows with CPU, memory, disk I/O, AV product activity, detected events

#### 3. HTML (User-Friendly Dashboard)
Contains: executive summary, graphs, AV product breakdown, process details, timeline, recommendations

---

## 8. System Requirements

### 8.1 Minimum Requirements
- **OS**: Windows 10 or later / Windows Server 2016 or later
- **.NET**: .NET 6+ or .NET Framework 4.8+
- **Admin Privileges**: Required (to read event logs and monitor processes)
- **Disk Space**: ~50 MB for application, ~100 MB per report
- **Memory**: 200 MB baseline

### 8.2 Antivirus Compatibility
- Works with ANY installed antivirus/endpoint protection software
- Read-only monitoring (no modifications to AV settings)
- No special agents or elevated permissions needed beyond admin

---

## 9. Success Criteria & Acceptance Tests

### 9.1 Functional Requirements
✓ Detect ANY installed antivirus/security software (not limited to predefined list)
✓ Monitor externally-launched processes (not start them)
✓ Monitor main process + all child processes
✓ Collect performance metrics in real-time
✓ Display live monitoring view in GUI/CLI
✓ Detect process blocking, crashes, performance degradation from ANY AV
✓ Generate JSON, CSV, HTML reports with AV product breakdown
✓ CLI and GUI interfaces both functional
✓ Support both exact name and wildcard pattern matching
✓ Gracefully handle unknown/custom antivirus products

### 9.2 Quality Criteria
✓ No false positives from non-AV processes (>95% accuracy)
✓ Minimal false negatives (<5%)
✓ Monitoring has <5% CPU overhead
✓ Monitoring has <50 MB memory overhead
✓ Reports generate in <5 seconds
✓ No memory leaks (sustained monitoring >1 hour)
✓ GUI remains responsive during monitoring

### 9.3 Test Scenarios
✓ Test with legitimate app (Word, VS Code)
✓ Test with different antivirus products installed
✓ Test with unknown/custom antivirus software
✓ Test with multiple AV products active simultaneously
✓ Test without any antivirus installed
✓ Test with app that crashes
✓ Test with long-running process (>10 minutes)
✓ Test with many child processes
✓ Test pattern matching: "*app*", "app*", "*app", exact name

---

## 10. Out of Scope (Future Enhancements)
- Pre-configured profiles for common applications
- Cloud-based result storage and analysis
- Scheduled/automated testing
- Integration with SIEM/logging systems
- Support for Mac/Linux
- Automatic remediation

---

## 11. Success Definition
**Delivered**: Standalone Windows application with GUI + CLI that allows IT admins to definitively determine if an application crash is caused by **ANY antivirus or endpoint protection software** by monitoring externally-launched processes. Tool automatically detects all installed security software, captures real-time metrics and AV activity, correlates events, and generates high-confidence reports without modifying or interfering with antivirus settings.

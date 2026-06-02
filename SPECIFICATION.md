# Antivirus Interference Detector - Technical Specification

## 1. Executive Summary
A standalone application that allows IT admins and technical support engineers to detect whether application crashes or performance issues are caused by Windows Defender, CrowdStrike, SentinelOne, Symantec, or other antivirus/endpoint protection software.

**Key Difference from traditional approach**: The tool monitors externally-launched processes rather than launching them internally. This allows real-world testing where applications are started normally through their usual entry points.

---

## 2. Use Case & Audience
- **Primary Users**: IT administrators, technical support engineers, system administrators
- **Problem**: Application crashes with unclear root cause — is it the antivirus?
- **Solution**: Start the tool in monitoring mode, then launch the application normally and observe if issues occur while monitoring antivirus activity
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
- **Windows Defender** (primary)
- **CrowdStrike Falcon**
- **SentinelOne**
- **Symantec (Norton)**
- **Extensible design** for other antivirus products

### 3.2 Monitoring Capabilities
- Monitor externally-launched processes (by name or PID pattern)
- Detect all child processes created by target process
- Measure performance impact (CPU, memory, disk I/O, network)
- Detect interference (blocking, crashes, resource constraints)
- Real-time live view of metrics and AV activity
- Track interference for main process and all child processes

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
│   │   ├── AVDetector.cs              # Detect installed AV products
│   │   ├── AVActivityMonitor.cs       # Monitor AV scanning/blocking activity
│   │   ├── InterferenceAnalyzer.cs    # Analyze data & detect interference
│   │   ├── ReportGenerator.cs         # Generate reports (JSON, CSV, HTML)
│   │   └── MonitoringSession.cs       # Manage monitoring session lifecycle
│   ├── AVIntegration/
│   │   ├── WindowsDefenderMonitor.cs
│   │   ├── CrowdStrikeMonitor.cs
│   │   ├── SentinelOneMonitor.cs
│   │   ├── SymantecMonitor.cs
│   │   └── IAVMonitor.cs              # Interface for extensibility
│   ├── Models/
│   │   ├── MonitoringSession.cs
│   │   ├── ProcessMetrics.cs
│   │   ├── AVActivity.cs
│   │   ├── InterferenceReport.cs
│   │   ├── AnalysisResult.cs
│   │   ├── ProcessSnapshot.cs
│   │   └── MetricsSnapshot.cs
│   ├── Utils/
│   │   ├── EventLogReader.cs
│   │   ├── RegistryReader.cs
│   │   ├── WMIHelper.cs
│   │   ├── FileHelper.cs
│   │   └── ProcessHelper.cs
│   └── Services/
│       ├── RealTimeMonitoringService.cs
│       └── DataAggregationService.cs
├── tests/
│   ├── UnitTests/
│   │   ├── ProcessMonitorTests.cs
│   │   ├── AVMonitorTests.cs
│   │   └── InterferenceAnalyzerTests.cs
│   └── IntegrationTests/
│       └── EndToEndTests.cs
├── docs/
│   ├── SETUP.md
│   ├── USER_GUIDE.md
│   ├── CLI_REFERENCE.md
│   └── ARCHITECTURE.md
├── AV-Interference-Detector.sln
├── README.md
├── SPECIFICATION.md
└── .gitignore
```

### 4.2 Technology Stack
- **Language**: C# (.NET 6+ or .NET Framework 4.8)
- **GUI Framework**: WPF (Windows Presentation Foundation)
- **CLI Framework**: System.CommandLine
- **Monitoring**: WMI, ETW (Event Tracing for Windows), Windows Event Log, Performance Counters
- **Process Management**: System.Diagnostics.Process, Job Objects
- **Report Generation**: JSON (System.Text.Json), CSV (CsvHelper), HTML (HtmlAgilityPack or custom templates)
- **Async/Concurrency**: async/await, BackgroundWorker for GUI

---

## 5. Feature Specifications

## 5.1 Overall Workflow

### Phase 1: Configuration
1. User launches the tool
2. User specifies what to monitor:
   - By process name (e.g., "app.exe", "installer.exe")
   - By partial name pattern (e.g., "*app*" matches "MyApp.exe", "app-launcher.exe")
   - Alternatively, wait for first process matching pattern to appear
3. User selects antivirus products to monitor
4. User selects advanced options (performance metrics, tracking depth)
5. User clicks "Start Capturing"

### Phase 2: Monitoring (Active)
1. Tool waits for target process to appear
2. When target process is found:
   - User sees confirmation in UI: "Target process found: app.exe (PID: 1234)"
   - Monitoring begins immediately
3. User is free to reproduce the issue (let app run, interact with it, trigger crash)
4. Tool continuously:
   - Tracks main process + child processes
   - Collects performance metrics
   - Monitors antivirus activity
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
  - Shows detected antivirus products
  - Checkboxes to select which to monitor (all checked by default)
  - Manual override option

**Section 3: Monitoring Options**
- Monitor child processes: Checkbox (default: checked)
- Track file access: Checkbox (default: unchecked)
- Track network access: Checkbox (default: unchecked)
- Track DLL/module loading: Checkbox (default: unchecked)

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

**Section 2: Live Metrics (Real-time updates)**
- Current CPU: 45.2% (sparkline showing trend)
- Current Memory: 256 MB (sparkline showing trend)
- Current Disk I/O: 1.2 MB/s (sparkline showing trend)
- Current Network I/O: 0.5 MB/s (sparkline showing trend)

**Section 3: Process Tree**
- Hierarchical view of main process + children
- Click on process to see detailed metrics

**Section 4: Antivirus Activity Log (Live)**
- Timeline of detected AV events
- Auto-scroll to latest event
- Color coding: Yellow (warning), Red (threat/block), Green (safe)

**Section 5: Live Alerts**
- Display any detected interference in real-time
- Show correlation with AV events

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
- **Antivirus Activity Summary**: Count of events per AV product

### Detailed Analysis Section
- Timeline graph: CPU, Memory, AV Activity over time
- Process details table
- AV events table

### Recommendations Section
- Bullet points with suggested actions
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
  --antivirus "Windows Defender,CrowdStrike" `
  --sample-interval 1000 `
  --track-children true `
  --output-dir "C:\Reports" `
  --output-format "json,html" `
  --timeout 300

# List detected antivirus
AV-Detector.exe list-antivirus

# Get antivirus status
AV-Detector.exe av-status

# Show help
AV-Detector.exe monitor --help
```

### CLI Output Examples

```
$ AV-Detector.exe monitor --process "app.exe"

[INFO] Antivirus Interference Detector v1.0
[INFO] Detected Antivirus: Windows Defender, CrowdStrike
[INFO] Waiting for process: app.exe
[INFO] Monitoring output: C:\Users\Admin\Desktop\Report_20260602_103000
[INFO] Type 'stop' and press Enter to end monitoring

... waiting for process to appear ...

[INFO] Target process found: app.exe (PID: 1234)
[INFO] Started monitoring at 2026-06-02 10:30:15
[INFO] Monitoring child processes: enabled

10:30:20 | CPU: 12% | Mem: 128 MB | Disk I/O: 0.5 MB/s
10:30:21 | [WinDefender] Scan started
10:30:22 | CPU: 45% | Mem: 256 MB | Disk I/O: 2.1 MB/s
10:30:23 | [WinDefender] Threat detected: app.exe (PID: 1234)
10:30:24 | [WinDefender] Action: Quarantine
10:30:25 | CPU: 5% | Mem: 0 MB | Status: CRASHED

[Alert] Process crashed - exit code: -1

stop

[INFO] Monitoring stopped
[INFO] Analysis in progress...

=== RESULTS ===
Interference Detected: YES
Confidence: 88%
Primary Cause: ProcessBlocked
Affected Processes: 1/1

Report saved to: C:\Users\Admin\Desktop\Report_20260602_103000\report.json
HTML report: C:\Users\Admin\Desktop\Report_20260602_103000\report.html
```

---

## 5.6 Process Discovery & Monitoring

### How It Works

1. **Initial Setup Phase**:
   - User specifies process to monitor (name, pattern, or PID)
   - Tool validates input and waits

2. **Process Discovery**:
   - Tool polls running processes every 500-1000ms
   - Searches for match (exact, pattern, or PID)
   - When found, logs discovery event and begins monitoring

3. **Child Process Tracking**:
   - Uses WMI or Job Objects to track all children
   - Maintains process tree structure
   - Captures child creation/termination events

4. **Continuous Monitoring**:
   - For each process in tree:
     - Collect metrics (CPU, memory, disk I/O, etc.)
     - Detect crashes/abnormal termination
     - Monitor file access (optional)
   - Correlate with AV activity events

---

## 5.7 Antivirus Activity Detection

### Windows Defender
- **Source**: Windows Event Log (Microsoft-Windows-Windows Defender)
- **Events to Monitor**: Scan started/completed, threat detected, action taken

### CrowdStrike Falcon
- **Source**: Windows Event Log (CrowdStrike) + Registry
- **Detection**: Service status check, event log monitoring

### SentinelOne
- **Source**: Windows Event Log, Registry, Local API
- **Detection**: Agent status check, threat event monitoring

### Symantec
- **Source**: Windows Event Log, Registry
- **Detection**: Service status check, threat event monitoring

---

## 5.8 Interference Detection Logic

### Interference Indicators

1. **Process Blocking/Prevention**: Process fails to start or exits immediately with AV blocking evident
2. **Process Crashing**: Unexpected termination coincident with AV activity
3. **Child Process Interference**: Child process fails while parent succeeds, with AV targeting child
4. **Performance Degradation**: CPU/Memory spikes during AV scan windows
5. **Timeout/Hang**: Process unresponsive during AV activity
6. **File Access Denied**: Process file operations fail with AV blocking evident

### Scoring Algorithm

```
confidence = (Σ(indicator_weight × timing_correlation)) / max_possible_weight

Example:
  ProcessCrashed (40 pts) + CorrelatedWithAVActivity (30 pts) = 70/100 = 70%
  ProcessCrashed (40 pts) + CorrelatedWithAVActivity (30 pts) + FileBlocked (20 pts) = 90/100 = 90%
```

---

## 5.9 Report Generation

### Output Formats

#### 1. JSON (Detailed, Machine-Readable)
Contains: session metadata, process tree, metrics timeline, AV activity timeline, analysis results, recommendations

#### 2. CSV (Spreadsheet-Friendly)
Contains: per-timestamp rows with CPU, memory, disk I/O, AV events

#### 3. HTML (User-Friendly Dashboard)
Contains: executive summary, graphs, process details, AV timeline, recommendations

---

## 6. System Requirements

### 6.1 Minimum Requirements
- **OS**: Windows 10 or Windows Server 2016+
- **.NET**: .NET 6+ or .NET Framework 4.8
- **Admin Privileges**: Required
- **Disk Space**: 50 MB application, 100 MB per report
- **Memory**: 200 MB baseline, +100 MB per monitored process

### 6.2 Antivirus Compatibility
- Works with any installed antivirus (read-only monitoring)
- No special agents or elevated permissions needed beyond admin

---

## 7. Success Criteria & Acceptance Tests

### 7.1 Functional Requirements
✓ Monitor externally-launched processes (not start them)
✓ Detect Windows Defender, CrowdStrike, SentinelOne, Symantec
✓ Monitor main process + all child processes
✓ Collect performance metrics in real-time
✓ Display live monitoring view in GUI/CLI
✓ Detect process blocking, crashes, performance degradation
✓ Generate JSON, CSV, HTML reports
✓ CLI and GUI interfaces both functional
✓ Support both exact name and wildcard pattern matching

### 7.2 Quality Criteria
✓ No false positives (>90% accuracy)
✓ Minimal false negatives (<10%)
✓ Monitoring has <5% CPU overhead
✓ Monitoring has <50 MB memory overhead
✓ Reports generate in <5 seconds
✓ No memory leaks (sustained monitoring >1 hour)
✓ GUI remains responsive during monitoring

### 7.3 Test Scenarios
✓ Test with legitimate app (Word, VS Code)
✓ Test with known malware (intentionally quarantined)
✓ Test with long-running process (>10 minutes)
✓ Test with many child processes (>50 children)
✓ Test with app that crashes
✓ Test with app that hangs
✓ Test pattern matching: "*app*", "app*", "*app", exact name
✓ Test with multiple AV products active simultaneously
✓ Test without any antivirus installed

---

## 8. Out of Scope (Future Enhancements)
- Pre-configured profiles for common applications
- Cloud-based result storage and analysis
- Scheduled/automated testing
- Comparison against golden baseline from clean system
- Integration with SIEM/logging systems
- Support for Mac/Linux
- Automatic remediation

---

## 9. Success Definition
**Delivered**: Standalone Windows application with GUI + CLI that allows IT admins to definitively determine if an application crash is caused by Windows Defender, CrowdStrike, SentinelOne, Symantec, or other antivirus by monitoring externally-launched processes. Tool captures real-time metrics and AV activity, correlates events, and generates high-confidence reports without modifying or interfering with antivirus settings.

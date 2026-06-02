# Antivirus Interference Detector

A standalone Windows application that helps IT administrators and technical support engineers determine if application crashes or performance issues are caused by antivirus interference (Windows Defender, CrowdStrike, SentinelOne, Symantec, etc.).

## Key Features

- **Non-intrusive Monitoring**: Monitor externally-launched applications without controlling them
- **Multi-AV Support**: Detect interference from Windows Defender, CrowdStrike, SentinelOne, Symantec
- **Child Process Tracking**: Automatically monitor all child processes created by the target application
- **Real-time Metrics**: Live CPU, memory, disk I/O, and network monitoring with visual graphs
- **AV Activity Correlation**: Real-time antivirus event log monitoring with correlation to application behavior
- **Live Alerts**: Immediate notifications when interference is detected
- **Comprehensive Reports**: Generate JSON, CSV, and HTML reports with detailed analysis and recommendations
- **Dual Interface**: Both CLI and GUI for different use cases

## How It Works

1. **Configure**: Specify the application to monitor (by name, pattern, or PID)
2. **Start Capturing**: Tool waits for the process to launch
3. **Launch App**: User launches the application normally through any means
4. **Reproduce Issue**: Interact with the app and reproduce the crash or problem
5. **View Results**: Real-time monitoring shows live metrics and AV activity; detailed report after completion

## Quick Start

### GUI Mode
```bash
AV-Interference-Detector.exe
```

### CLI Mode
```bash
# Monitor a specific process by name
AV-Interference-Detector.exe monitor --process "app.exe"

# Monitor using wildcard pattern
AV-Interference-Detector.exe monitor --process "*installer*"

# List detected antivirus products
AV-Interference-Detector.exe list-antivirus
```

## System Requirements

- **OS**: Windows 10 or later / Windows Server 2016 or later
- **Runtime**: .NET 6+ or .NET Framework 4.8+
- **Privileges**: Administrator
- **Disk Space**: ~50 MB for application, ~100 MB per report
- **Memory**: 200 MB baseline

## Documentation

- **[Setup Guide](docs/SETUP.md)** - Installation and configuration
- **[User Guide](docs/USER_GUIDE.md)** - Detailed usage instructions
- **[CLI Reference](docs/CLI_REFERENCE.md)** - Command-line options
- **[Architecture](docs/ARCHITECTURE.md)** - Technical architecture
- **[Specification](SPECIFICATION.md)** - Full technical specification

## Installation

### From Release
Download the latest release from the [Releases](../../releases) page.

### From Source
1. Clone the repository
2. Open `AV-Interference-Detector.sln` in Visual Studio 2022 or later
3. Build the solution (Release configuration)
4. Output: `bin/Release/AV-Interference-Detector.exe`

## Usage Examples

### GUI Workflow
1. Launch the application
2. Go to Configuration tab
3. Enter target process name (e.g., "setup.exe")
4. Select antivirus products to monitor
5. Click "Start Capturing"
6. Launch your application normally
7. Reproduce the issue
8. View live metrics and alerts in the Monitoring tab
9. Click "Stop Capturing" and review the Results tab

### CLI Workflow
```bash
# Start monitoring
AV-Interference-Detector.exe monitor --process "myapp.exe" --output-dir "C:\Reports"

# Wait for process to appear (shown in console)
# Launch your application
# When done, type "stop" and press Enter

# Reports will be saved to C:\Reports
```

## Output Reports

The tool generates three types of reports:

1. **JSON** - Detailed metrics and analysis (programmatic use)
2. **CSV** - Spreadsheet-compatible format (Excel/Sheets)
3. **HTML** - Interactive dashboard with graphs and analysis

Each report includes:
- Process execution timeline
- Performance metrics (CPU, memory, disk I/O)
- Antivirus activity events
- Interference confidence score
- Actionable recommendations

## Supported Antivirus Products

- ✅ Windows Defender
- ✅ CrowdStrike Falcon
- ✅ SentinelOne
- ✅ Symantec/Norton
- 🔄 Other products (extensible design)

## Project Structure

```
src/
├── CLI/                    # Command-line interface
├── GUI/                    # WPF graphical interface
├── Core/                   # Core monitoring engine
├── AVIntegration/          # Antivirus integrations
├── Models/                 # Data models
├── Utils/                  # Utility functions
└── Services/               # Background services

tests/                      # Unit and integration tests
docs/                       # Documentation
```

## Development

### Prerequisites
- Visual Studio 2022 or later
- .NET 6 SDK or .NET Framework 4.8+
- Administrator command prompt for building

### Building
```bash
# Clone repository
git clone https://github.com/kevinwangansys/antivirus-interference-detector.git
cd antivirus-interference-detector

# Build solution
msbuild AV-Interference-Detector.sln /p:Configuration=Release

# Or use Visual Studio
# Open AV-Interference-Detector.sln and build
```

### Running Tests
```bash
dotnet test
```

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit changes (`git commit -am 'Add feature'`)
4. Push to branch (`git push origin feature/your-feature`)
5. Create a Pull Request

## License

MIT License - see LICENSE file for details

## Support

For issues, questions, or suggestions:
1. Check existing [Issues](../../issues)
2. Review [Documentation](docs/)
3. Create a new Issue with details about your problem

## Roadmap

- [ ] Phase 1: Core engine (process monitoring, AV detection, basic analysis)
- [ ] Phase 2: GUI implementation
- [ ] Phase 3: CLI implementation
- [ ] Phase 4: Polish and optimization
- [ ] v1.0 Release

## Authors

- **Kevin Wang** - Project lead

## Acknowledgments

- Windows Defender, CrowdStrike, SentinelOne, and Symantec teams for documentation
- Community feedback and contributions

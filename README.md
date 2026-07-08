# gas-unit-pacemaker-simulation
An interactive browser-based simulation of a gas-integrated, MRI-compatible pacemaker prototype, built to validate the device's decision logic before any physical hardware existed.

Why this project exists

Conventional pacemakers carry two under-addressed risks: pneumothorax (gas/air buildup around the lungs during implantation or lead migration) and MRI incompatibility (ferromagnetic components heating or dislodging under a scanner's magnetic field). This project explores a pacemaker design that integrates a gas drainage unit to manage the first risk, alongside a non-ferromagnetic material approach aimed at the second.

Why a simulation

I designed this system without access to a lab or the physical sensors (AD8232 heart sensor, drainage sensor, buzzer) described in the accompanying circuit schematic and research report. Rather than wait for equipment I didn't have, I encoded the full decision logic — heart rate monitoring, ECG threshold checks, pneumothorax detection, automatic gas drainage, and combined-emergency handling — into an interactive HTML/CSS/JavaScript simulation. This let me test and refine the system's behavior end-to-end before ever touching hardware.

What it does


Simulates real-time heart rate and ECG monitoring with adjustable thresholds
Detects simulated pneumothorax via gas pressure readings and automatically triggers a drainage sequence
Distinguishes between normal, warning, and emergency system states
Logs every event with a timestamp, mirroring how the physical device would report to a clinician
Includes five test scenarios (normal operation, high/low heart rate, pneumothorax, combined emergency) to stress-test the logic


How to run it

No installation needed — it's a single self-contained HTML file with no external dependencies.


Download gas_unit_pacemaker_simulation.html
Open it in any browser
Click Start System, then try the buttons under Simulation Scenarios


Or view it live via GitHub Pages: (add your Pages link here once enabled)

Background

This was my first engineering project and the starting point for a broader interest in resource-constrained, hands-on design — later extended into an eye-accommodation teaching model, a rocket telemetry system, and a bionic hand prototype.

Author

Elif Ceren Okur — Hatice Cemil Ercan Science High School

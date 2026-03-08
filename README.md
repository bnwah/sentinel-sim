# Sentinel-Sim: C++ Sensor Fusion Simulator

A C++ command-line application simulating core data ingestion and 
processing workflows analogous to C5ISR mission management systems.

## What It Does
- Ingests raw GPS sensor pings from a flat file (simulating radar/AIS/camera feeds)
- Parses and models sensor data using C++ structs
- Calculates geodesic distance between coordinates using the Haversine formula
- Maintains running track state across multiple sensor sources
- Outputs a formatted Common Operating Picture (COP) to the console

## Build & Run
```bash
g++ -std=c++17 -o sentinel src/main.cpp
./sentinel sensors.txt
```

## Tech Stack
- Language: C++17
- Libraries: `<iostream>`, `<fstream>`, `<sstream>`, `<cmath>`, `<vector>`, `<map>`, `<string>`
- Concepts: Structs, file stream ingestion, geodesic algorithms, track state management

## Background
Built to demonstrate C++ proficiency and familiarity with sensor data 
workflows relevant to defense and C5ISR software engineering roles.
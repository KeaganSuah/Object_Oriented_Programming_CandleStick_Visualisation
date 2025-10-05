# 📈 Object-Oriented Candlestick Visualisation (C++)

A console-based, object-oriented application that reads timeseries data from CSV files and renders **candlestick-style summaries** with filtering, chunked display, and simple linear-regression forecasts. Developed as an academic (school) project to demonstrate C++ OOP design, file I/O, and basic analytics.

**YouTube demo:** https://www.youtube.com/watch?v=NY9SvBlgJ4c

## Overview

This project models candlestick data using C++ classes and provides a menu-driven interface to:
- Load and validate input CSVs (e.g., min/max/mean series)
- Filter by user-defined criteria (date ranges, etc.)
- Display candlesticks in readable chunks in the terminal
- Run a simple linear-regression–style prediction to extend the series

The codebase is organized into focused classes (CSV reading, data filtering, display/visualisation, and prediction) to illustrate clean OOP principles.

## Objectives

- Apply **C++ OOP** for a modular, maintainable CLI data tool.  
- Parse and validate CSV inputs; normalise data for analysis.  
- Render candlestick summaries and timestamps in terminal-friendly chunks.  
- Provide a lightweight forecasting routine for “next-period” values.

## Data

Place the following files in the project root (same folder as the executable):
- `min.csv`  
- `max.csv`  
- `mean.csv`  

> Note: The application checks for required files and may trigger generation/cleaning logic if expected inputs are missing.

## Build Instructions

Requires a modern **C++17** compiler (e.g., `g++`, `clang++`) and the C++ Standard Library. No external dependencies.

1. Compile:

    ```bash
    g++ -std=gnu++17 \
      main.cpp \
      weatherMain.cpp \
      CSVReader.cpp \
      candleStick.cpp \
      filterCandlestick.cpp \
      visualisation.cpp \
      prediction.cpp \
      -o candlesticks
    ```

2. Ensure the CSV files (`min.csv`, `max.csv`, `mean.csv`) are next to the executable.

3. Run:

    ```bash
    ./candlesticks
    ```

## Usage

- Follow the **menu prompts** to load data, set filters, and display candlestick chunks.  
- Use the **prediction** option to generate simple forward estimates based on historical averages and period detection.  
- Displays include headers (reflecting your filter), candles for each period, and aligned timestamps.

## Tools and Libraries

- **C++17** and **C++ Standard Library** (file I/O, containers, algorithms)  
- **No external libraries required**

## Key Learnings

- Designing a small CLI analytics tool with **encapsulated classes** and clear responsibilities.  
- Implementing robust **CSV parsing/validation** and data preparation.  
- Rendering informative **terminal visualisations** for timeseries.  
- Adding a straightforward **forecasting** step to extend insights beyond the observed data.

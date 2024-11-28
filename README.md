
# QuantEdge

QuantEdge is a modern C++ framework for implementing and testing quantitative trading strategies, including pairs trading, mean-reversion, and momentum strategies. It focuses on efficiency, scalability, and modularity, making it suitable for quantitative finance projects.

---

# Features

1. Pairs Trading: Market-neutral strategy based on the relative movement of two correlated assets.
2. Mean-Reversion Strategy: Identifies deviations from the historical mean and trades on the assumption of reversion.
3. Momentum Strategy: Trades on the assumption that trends will continue.
4. Cross-Platform Support: Uses CMake for cross-platform builds and tests.
5. Unit Testing: Includes Google Test (gtest) for structured, comprehensive testing.
6. Dockerized Development: Easily run and test the project in a containerized environment.

---

## Project Structure

QuantEdge/

│

├── include/                # Header files

│   ├── PairsTrader.hpp     # Pairs trading implementation

│   ├── Statistics.hpp      # Utility functions for statistical calculations

│   ├── StrategyBase.hpp    # Abstract base class for trading strategies

│   ├── MeanReversionStrategy.hpp  # Mean-reversion strategy

│   └── MomentumStrategy.hpp       # Momentum strategy

│

├── src/                    # Source files

│   ├── PairsTrader.cpp     # Pairs trading implementation

│   ├── Statistics.cpp      # Utility implementations

│   ├── MeanReversionStrategy.cpp  # Mean-reversion strategy

│   └── MomentumStrategy.cpp       # Momentum strategy

│

├── test/                   # Test files

│   └── TestPairsTrader.cpp # Unit tests for trading strategies

│

├── CMakeLists.txt          # CMake build configuration


├── Dockerfile              # Dockerfile for containerized builds and tests

├── docker-compose.yml      # Docker Compose for managing Docker builds and tests

└── README.txt              # Project documentation

---

## Prerequisites

- CMake: Version 3.16 or higher.
- C++ Compiler: A modern compiler supporting C++17.
- Docker: Optional, for containerized builds and tests.
- Google Test (gtest): Automatically fetched and built via CMake.

---

## Build and Run

### Using CMake

1. Configure the Project:
   cmake -S . -B build -G Ninja  # Or replace "Ninja" with your generator

2. Build the Project:
   cmake --build build

3. Run the Tests:
   ctest --test-dir build --output-on-failure

4. Run the Test Executable Directly:
   ./build/TestPairsTrader

---

### Using Docker

1. Build and Run Tests:
   docker-compose up --build

2. Re-run Tests:
   docker-compose run quantedge

---

### Cleanup Docker Environment

To clean up the Docker environment and free up space, follow these steps:

1. Stop and Remove All Running Containers:
   docker-compose down

2. Remove Unused Docker Resources:
   - To remove unused containers, networks, and dangling images:
     docker system prune -f

3. Optional: Remove Unused Volumes:
   - To free up space by removing unused volumes:
     docker volume prune -f

4. Full Reset:
   - To remove all unused containers, images, volumes, and networks:
     docker system prune -a --volumes -f

Note: Be cautious when using the -a and --volumes flags, as they remove all unused resources, not just dangling ones.

---

## Strategies Implemented

1. Pairs Trading
A market-neutral strategy that trades the spread between two correlated assets.

- Input: Price data for two assets (e.g., from AAPL.txt and MSFT.txt).
- Signal:
  - Long the spread when it is below -2 standard deviations.
  - Short the spread when it is above +2 standard deviations.
- Output: Final capital after backtesting.

2. Mean-Reversion Strategy
A single-asset strategy that trades on the assumption that prices revert to their mean.

- Input: Price data for one asset.
- Signal:
  - Long when the price is below the short-term mean.
  - Short when the price is above the long-term mean.

3. Momentum Strategy
A single-asset strategy that trades on price trends.

- Input: Price data for one asset.
- Signal:
  - Long when momentum is positive.
  - Short when momentum is negative.

---

## Testing with Google Test

Google Test (gtest) is integrated into the project to enable structured and comprehensive unit testing.

1. Run Tests with CTest:
   ctest --test-dir build --output-on-failure

2. Run the Test Executable Directly:
   ./build/TestPairsTrader

---
## Running the Docker Management Script

The `quantedge_docker.sh` script is a utility to manage Docker operations for the **QuantEdge** project. It provides commands to build the Docker image, run the application, execute tests, and clean up resources.

#### **Prerequisites**

1. Ensure Docker is installed on your system.
2. Make the script executable:
   ```bash
   chmod +x quantedge_docker.sh

## Usage

Run the script with one of the following parameters:
```
./quantedge_docker.sh [parameter]
```

## Parameters:
Parameter - Description
build - Builds the Docker image for the project.
run - Cleans up existing containers and starts the QuantEdge application.
test - Runs the tests inside a Docker container.
clean - Stops and removes running containers, deletes unused networks and volumes.

## Troubleshooting

Common Issues
1. Build Errors:
   - Ensure you’ve installed CMake and a C++17-compatible compiler.
   - If errors persist, clean up the build directory:
     rm -rf build
   Then reconfigure and rebuild:
     cmake -S . -B build -G Ninja
     cmake --build build

2. Docker Errors:
   - Ensure Docker is running.
   - Use docker-compose down to stop all services and clean up:
     docker-compose down
     docker system prune -f

# Use Ubuntu as the base image
FROM ubuntu:20.04

# Set non-interactive mode for installing packages
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    g++ \
    git \
    && apt-get clean

# Set working directory
WORKDIR /app

# Copy the source files
COPY . .

# Configure and build the project
RUN cmake -S . -B build -G Ninja && cmake --build build

# Run tests as the default command
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]


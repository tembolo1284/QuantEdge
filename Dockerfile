# Base image
FROM ubuntu:20.04

# Set up environment
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y build-essential cmake git ninja-build && \
    apt-get clean

# Set working directory
WORKDIR /QuantEdge

# Copy source files
COPY . .

# Build the project
RUN cmake -S . -B build -G Ninja && cmake --build build

# Default command
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]


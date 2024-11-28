#!/bin/bash

# Function to stop and remove existing containers
cleanup() {
    echo "Cleaning up existing containers..."
    docker-compose down
    docker network rm quantedge-net 2>/dev/null || true
    docker system prune -f
}

# Function to ensure network exists
ensure_network() {
    if ! docker network inspect quantedge-net >/dev/null 2>&1; then
        echo "Creating quantedge-net network..."
        docker network create quantedge-net
    fi
}

# Function to build the Docker image
build_image() {
    echo "Building Docker image..."
    ensure_network
    docker-compose build
}

# Function to run the application
run_application() {
    echo "Starting QuantEdge application..."
    ensure_network
    docker-compose up
}

# Function to run tests
run_tests() {
    echo "Running tests..."
    ensure_network
    docker-compose run --rm quantedge
}

# Main script
case "$1" in
    "build")
        build_image
        ;;
    "run")
        cleanup
        run_application
        ;;
    "test")
        run_tests
        ;;
    "clean")
        cleanup
        ;;
    *)
        echo "Usage: $0 {build|run|test|clean}"
        echo "  build - Build the Docker image"
        echo "  run   - Run the QuantEdge application"
        echo "  test  - Run the tests"
        echo "  clean - Clean up containers"
        exit 1
        ;;
esac

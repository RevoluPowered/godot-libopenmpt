#!/bin/bash
# Build documentation using Docker

set -e

echo "Building documentation with Docker..."

# Build and run the container
docker-compose up --build

echo ""
echo "Documentation built successfully!"
echo "Open docs/build/html/index.html in your browser"

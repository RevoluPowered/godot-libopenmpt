FROM barichello/godot-ci:4.3

WORKDIR /build

# Install build dependencies
RUN apt-get update && \
    apt-get install -y \
        build-essential scons git && \
    rm -rf /var/lib/apt/lists/*

# Copy entire project (run from repo root: docker build -f tests/Dockerfile .)
COPY . /build/

# Build with tests enabled
RUN scons target=template_debug arch=x86_64 tests=yes -j$(nproc)

# Copy extension to tests directory
RUN mkdir -p tests/bin && \
    cp demo/bin/libopenmpt.gdextension tests/bin/ && \
    cp demo/bin/*.so tests/bin/ 2>/dev/null || true

# Run tests in headless mode (no display server needed)
CMD echo "=== Starting test execution ===" && \
    echo "Checking test files..." && \
    ls -la tests/bin/ && \
    echo "Running Godot with tests..." && \
    godot --headless --path tests --script test_runner.gd --verbose

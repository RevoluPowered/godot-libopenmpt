FROM barichello/godot-ci:4.3

WORKDIR /build

# Install build dependencies and X11/audio dependencies for Godot
RUN apt-get update && \
    apt-get install -y \
        build-essential scons pkg-config libflac-dev git \
        libx11-6 libxcursor1 libxinerama1 libxi6 libxrandr2 \
        libasound2 libpulse0 libudev1 libgl1 libvulkan1 \
        xvfb && \
    rm -rf /var/lib/apt/lists/*

# Copy entire project (run from repo root: docker build -f tests/Dockerfile .)
COPY . /build/

# Build with tests enabled
RUN scons target=template_debug arch=x86_64 tests=yes -j$(nproc)

# Copy extension to tests directory
RUN mkdir -p tests/bin && \
    cp demo/bin/libopenmpt.gdextension tests/bin/ && \
    cp demo/bin/*.so tests/bin/ 2>/dev/null || true

# Run tests with virtual framebuffer
CMD xvfb-run -a godot --headless --path tests --script test_runner.gd

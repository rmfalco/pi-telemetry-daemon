# Start with a clean Ubuntu Linux container
FROM ubuntu:22.04

# Prevent interactive text pop-ups during installation
ENV DEBIAN_FRONTEND=noninteractive

# Run the package installer INSIDE the Linux container
RUN apt-get update && apt-get install -y \
    build-essential \
    crossbuild-essential-arm64 \
    cmake \
    git \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Set the workspace directory inside the container
WORKDIR /workspace


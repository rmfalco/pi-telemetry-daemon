# Start with a clean Ubuntu Linux container matching your modern Pi environment
FROM ubuntu:24.04

# Prevent interactive text pop-ups during installation
ENV DEBIAN_FRONTEND=noninteractive

# 1. Enable multi-architecture support for arm64 packages
RUN dpkg --add-architecture arm64

# 2. Overwrite the new Ubuntu 24.04 deb822 style repo file to map amd64 and arm64 channels properly
RUN echo 'Types: deb' > /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'URIs: http://archive.ubuntu.com/ubuntu/ http://security.ubuntu.com/ubuntu/' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Suites: noble noble-updates noble-security' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Components: main restricted universe multiverse' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Architectures: amd64 i386' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' >> /etc/apt/sources.list.d/ubuntu.sources && \
    \
    echo '' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Types: deb' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'URIs: http://ports.ubuntu.com/ubuntu-ports/' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Suites: noble noble-updates noble-security' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Components: main restricted universe multiverse' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Architectures: arm64' >> /etc/apt/sources.list.d/ubuntu.sources && \
    echo 'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' >> /etc/apt/sources.list.d/ubuntu.sources

# 3. Clean and run the package installer safely
RUN apt-get update && apt-get install -y \
    build-essential \
    crossbuild-essential-arm64 \
    libgpiod-dev:arm64 \
    cmake \
    git \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Set the workspace directory inside the container
WORKDIR /workspace

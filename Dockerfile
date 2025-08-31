FROM ubuntu:25.04

LABEL org.opencontainers.image.source="https://github.com/ZofikarDev/waffle"
LABEL org.opencontainers.image.description="Slim interactive dev/build environment for Waffle (Clang 20, CMake, Clang-Format)"

ENV DEBIAN_FRONTEND=noninteractive

# Install only the essentials
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    clang-20 \
    clang-format-20 \
    clang-tools-20 \
    lld-20 \
    ninja-build \
    libc6-dev \
    make \
    cmake \
    python3-minimal \
    git \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN ln -s /usr/bin/clang-20 /usr/bin/clang && ln -s /usr/bin/clang++-20 /usr/bin/clang++ && ln -s /usr/bin/clang-format-20 /usr/bin/clang-format

# Default to clang
ENV CC=clang-20
ENV CXX=clang++-20

WORKDIR /workspace

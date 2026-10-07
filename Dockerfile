# for M1/M2 mac users, use this FROM instruction instead
# FROM --platform=linux/amd64 ubuntu:22.04
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN groupdel dialout

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    libssl-dev \
    zlib1g-dev \
    dos2unix \
    rsync \
    doxygen \
    graphviz \
    libc6-dbg \
    libcap-dev \
    valgrind \
    git \
    cmake \
    wget \
    zip

# 1 - make a directory in the container to copy project files to (typical convention is /app)

# 2 - copy project folder into newly created directory

# 3 - CD into new directory

# 4 - make clean

# 4 - compile code

# 5 - set command to run when Docker container is started
FROM ubuntu:jammy

ARG USER_ID
ARG GROUP_ID
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y \
    git \
    cmake \
    make \
    ruby \
    gcc \
    python3 \
    python3-pip \
    wget \
    bzip2

RUN pip install pyyaml

# Download and install GCC 10.2.1 ARM toolchain
RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.2-2020q4/gcc-arm-none-eabi-10.2-2020q4-x86_64-linux.tar.bz2 && \
    tar -xjf gcc-arm-none-eabi-10.2-2020q4-x86_64-linux.tar.bz2 -C /opt && \
    rm gcc-arm-none-eabi-10.2-2020q4-x86_64-linux.tar.bz2 && \
    ln -s /opt/gcc-arm-none-eabi-10.2-2020q4/bin/* /usr/local/bin/

# if either of these are already set the same as the user's machine, leave them be and ignore the error
RUN addgroup --gid $GROUP_ID inav; exit 0;
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID inav; exit 0;

USER inav
RUN git config --global --add safe.directory /src

VOLUME /src

WORKDIR /src/build
ENTRYPOINT ["/src/cmake/docker.sh"]

FROM ubuntu:jammy

ARG USER_ID
ARG GROUP_ID
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    cmake \
    make \
    ruby \
    gcc \
    python3 \
    python3-pip \
    wget \
    bzip2 \
    ninja-build

RUN pip install pyyaml

# Install GCC ARM toolchain from local file
COPY downloads/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 /tmp/
RUN tar -xjf /tmp/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 -C /opt && \
    rm /tmp/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 && \
    ln -sf /opt/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
    ln -sf /opt/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ && \
    ln -sf /opt/gcc-arm-none-eabi-10-2020-q4-major/bin/arm-none-eabi-* /usr/bin/

# Create home directory for inav user
RUN mkdir -p /home/inav

# if either of these are already set the same as the user's machine, leave them be and ignore the error
RUN addgroup --gid $GROUP_ID inav; exit 0;
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID inav; exit 0;

# Set permissions after user is created
RUN chown -R inav:inav /home/inav

# Add GCC ARM toolchain to PATH
ENV PATH="/opt/gcc-arm-none-eabi-10-2020-q4-major/bin:${PATH}"

VOLUME /src

# Switch to inav user and configure git
USER inav
ENV HOME=/home/inav
RUN git config --global --add safe.directory /src

WORKDIR /src/build
ENTRYPOINT ["/src/cmake/docker.sh"]

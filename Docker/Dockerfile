from rootproject/root:6.20.08-ubuntu20.04

WORKDIR /workspace

ENV LANG=C.UTF-8
RUN ln -sf /usr/share/zoneinfo/UTC /etc/localtime

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && \
    apt-get -yq install \
    gnupg2 \
    software-properties-common \
    git \
    apt-transport-https \
    ca-certificates \
    && \
    apt-get clean && \
    rm -rf /var/cache/apt/archives/* && \
    rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/carlomt/cmos.git /workspace

ENTRYPOINT  ["/usr/bin/bash", "-l"]
    
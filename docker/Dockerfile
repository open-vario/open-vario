# FROM debian:12
FROM debian:bookworm-slim as openvario-builder
SHELL ["/bin/bash", "-c"]

ARG user=default
ARG group=default
ARG uid=1000
ARG gid=1000


# Make sure apt is happy
ENV DEBIAN_FRONTEND=noninteractive

# -y by default for apt-get
RUN echo "APT::Get::Assume-Yes \"true\";" >> /etc/apt/apt.conf.d/aptconf

# Install build tools
RUN apt-get update && apt full-upgrade -y \
	&& apt-get update && apt-get install -qy --no-install-recommends make cmake \
	&&  apt-get autoremove && apt-get clean && rm -rf "/var/lib/apt/lists/*"

# Install toolchain
ADD gcc-arm-none-eabi.tar.bz2 /opt/

# Ensure user exist within container with same uid/gid
RUN groupadd -g "${gid}" "${group}" && useradd -l -m -u "${uid}" -g "${gid}" "${user}"

ENTRYPOINT ["/bin/bash"]

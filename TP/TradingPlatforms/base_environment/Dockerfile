FROM ubuntu:14.04

ENV HIREDIS_VERSION=0.13.3
ENV LIBUV_VERSION=1.11.0
RUN apt-get update && apt-get -y upgrade && apt-get -y install software-properties-common && add-apt-repository ppa:webupd8team/java -y && apt-get update

RUN (echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections) && apt-get install -y oracle-java8-installer oracle-java8-set-default

ENV JAVA_HOME /usr/lib/jvm/java-8-oracle
ENV PATH $JAVA_HOME/bin:$PATH


RUN apt-get update && apt-get install -y \
        wget \
        automake \
        libtool \
        libcurl3-gnutls \
        libev-dev \
        libgoogle-glog-dev \
        libmysqlclient-dev \
        build-essential \
        gdb \
        sqlite3 \
    && rm -rf /var/lib/apt/lists/*

RUN cd /tmp \
    && wget -O "hiredis-${HIREDIS_VERSION}.tar.gz" -c https://github.com/redis/hiredis/archive/v${HIREDIS_VERSION}.tar.gz \
    && tar -xf hiredis-${HIREDIS_VERSION}.tar.gz && cd hiredis-${HIREDIS_VERSION} && make -j && make install && ldconfig && cd / && rm -rf /tmp/hiredis*

RUN cd /tmp \
    && wget -O "libuv-${LIBUV_VERSION}.tar.gz" -c https://github.com/libuv/libuv/archive/v${LIBUV_VERSION}.tar.gz \
    && tar -xf libuv-${LIBUV_VERSION}.tar.gz && cd libuv-${LIBUV_VERSION} && sh autogen.sh && ./configure && make -j && make install && ldconfig && cd / && rm -rf /tmp/libuv*

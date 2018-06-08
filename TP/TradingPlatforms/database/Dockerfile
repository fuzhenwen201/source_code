# Pull base image 
FROM oraclelinux:7-slim  
# update source  
ADD redis-3.2.10.tar.gz /home/
COPY mysql-community-server-minimal-5.6.37-2.el7.x86_64.rpm /home/
COPY mysql-shell-1.0.9-1.el7.x86_64.rpm /home/
RUN rpmkeys --import https://repo.mysql.com/RPM-GPG-KEY-mysql \
  && yum install -y /home/mysql-community-server-minimal-5.6.37-2.el7.x86_64.rpm /home/mysql-shell-1.0.9-1.el7.x86_64.rpm libpwquality make gcc \
  && yum clean all \
  && mkdir /docker-entrypoint-initdb.d

VOLUME /var/lib/mysql

RUN cd /home/redis-3.2.10 && make MALLOC=libc && make install

FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install wget build-essential -y
WORKDIR /opt/
RUN wget https://github.com/peercoin/peercoin/releases/download/v0.15.1ppc/peercoin-1af7509666a7-x86_64-linux-gnu.tar.gz
RUN tar zxvf peercoin-1af7509666a7-x86_64-linux-gnu.tar.gz
RUN mv peercoin-1af7509666a7/bin/peercoin* /usr/bin/
#RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
#RUN gcc blocknotify.c -o /usr/bin/blocknotify
CMD /usr/bin/peercoind -printtoconsole

FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install wget build-essential -y
WORKDIR /opt/
RUN wget https://github.com/peercoin/peercoin/releases/download/v0.14.10ppc/peercoin-43e7b96c50bc-x86_64-linux-gnu.tar.gz
RUN tar zxvf peercoin-43e7b96c50bc-x86_64-linux-gnu.tar.gz
RUN mv peercoin-43e7b96c50bc/bin/peercoin* /usr/bin/
RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
RUN gcc blocknotify.c -o /usr/bin/blocknotify
CMD /usr/bin/peercoind -daemon;tail -f /root/.peercoin/debug.log

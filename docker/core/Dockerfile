FROM wiiulegacy/core:0.1

MAINTAINER Maschell <maschell@gmx.de>

RUN git clone https://github.com/Maschell/WiiUPluginSystem.git
RUN cd WiiUPluginSystem && make && make install
RUN rm -rf WiiUPluginSystem

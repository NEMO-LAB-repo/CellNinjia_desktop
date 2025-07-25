FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y libserial-dev
RUN apt-get install -y build-essential cmake

WORKDIR /CellNinjia_desktop
COPY . .
RUN rm -rf build
WORKDIR build
RUN cmake ../
RUN make clean
RUN make all 
WORKDIR /CellNinjia_desktop

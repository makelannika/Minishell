FROM ubuntu:22.04

RUN apt update
RUN apt-get install -y pkg-config
RUN apt install -y make build-essential
RUN apt-get install -y gcc 
RUN apt-get install -y libreadline8 libreadline-dev
RUN apt-get install -y libncurses5-dev libncursesw5-dev
RUN mkdir /minishell

COPY . /minishell/

WORKDIR /minishell

RUN make

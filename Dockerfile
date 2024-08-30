FROM ubuntu:22.04

RUN apt update
RUN apt install -y make build-essential
RUN apt install -y libreadline8 libreadline-dev
RUN apt install -y libncurses5-dev libncursesw5-dev
RUN mkdir /minishell

COPY . /minishell/

WORKDIR /minishell

RUN make
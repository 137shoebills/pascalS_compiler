# 用于编译原理课程设计的dockerfile
# 虚拟机版本   ubuntu 20.04 
# 开发软件版本
# gcc 9.4.0
# g++ 9.4.0 
# make 4.2.1
# flex 2.6.4 bison 3.5.1 
# clang 10.0 llvm 10.0 

FROM ubuntu:20.04
LABEL maintainer="YangLan 1356298972@qq.com"

# 更新apt-get安装系统管理指令
RUN apt-get update 
RUN apt-get install sudo -y

# 更新apt
RUN sudo apt update -y

# 安装wget
RUN sudo apt install wget -y

# 安装 gcc g++ make
# ubuntu 20.04的gcc和g++版本为9.4.0 
# ubuntu 20.04的make版本为4.2.1
RUN sudo apt install build-essential -y


# 安装flex，bison
RUN sudo apt install flex bison -y


# 安装 clang 和 llvm 
# ubuntu 20.04 默认版本 10.0
RUN sudo apt install clang -y
RUN sudo apt install llvm -y

# 安装 locate
RUN sudo apt-get install mlocate -y
RUN sudo updatedb

# 安装 LLVM 后要在 PATH 中添加 /usr/include/llvm-10/ /usr/include/llvm-c-10
# 添加 PATH 后，VSC 依然会报错，
# 所以还需要 ctrl+alt+P 输入  C/C++: Edit Configurations(JSON) 打开json文件
# 在 "includePath" 中添加 "/usr/include/llvm-10" 和 "/usr/include/llvm-c-10" 两项。
# 修改json文件时不要忘了分隔符逗号
ENV PATH "$PATH:/usr/include/llvm-10"
ENV PATH "$PATH:/usr/include/llvm-c-10"

# 使用官方预编译二进制安装llvm
# 大约20分钟，版本 13.0.0
# 这里应该不需要所以全部注释掉了，不用管
# sudo mkdir -p /usr/local
# cd /usr/local
# sudo wget https://github.com/llvm/llvm-# project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-# x86_64-linux-gnu-ubuntu-20.04.tar.xz
# sudo tar xvf clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-# 20.04.tar.xz
# sudo mv clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04 llvm
# export PATH="$PATH:/usr/local/llvm/bin"
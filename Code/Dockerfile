# 选择一个镜像作为基础,centos8停止更新了，所以用centos7。
# 之前试过ubuntu，但是ubuntu需要更新apt-get，如果不换源就更新速度奇慢，而且换源比较麻烦，懒得搞了。
FROM centos:7

# 维护者的信息
LABEL maintainer="YangLan 1356298972@qq.com"

# 启动镜像后进行的操作，这里是更新yum，并依次安装软件。
RUN yum update -y
# 下面两行是更新版本，centos:7 install默认下载的gcc版本是 4.8.5 只支持到c++11，但是应该够用了，因此这个镜像不更新gcc版本
# RUN yum install centos-release-scl -y
# RUN yum install devtoolset-9-gcc devtoolset-9-gcc-c++
# 下载gcc、g++
RUN yum install -y gcc
RUN yum install -y gcc-c++
# 下载clang
RUN yum install -y epel-release
RUN yum install -y clang
#安装Make
RUN yum -y install gcc automake autoconf libtool make
# 安装gdb
RUN yum install -y gdb
# 安装flex bison
RUN yum install -y flex
RUN yum install -y flex-devel
RUN yum install -y bison
# 安装 LLVM/Clang 二进制文件
RUN yum install llvm clang -y
# 安装系统管理指令
RUN yum install sudo -y
RUN yum install wget -y
# 安装llvm11版本
RUN wget http://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
RUN yum localinstall epel-release-latest-7.noarch.rpm -y
RUN yum install llvm11 llvm11-devel clang -y
RUN cp /usr/lib64/llvm11/lib/libLLVM.so /usr/local/lib/   #将库的链接放置在local/bin目录下


# 安装CMake，centos7因为版本比较老所以要下载源码并编译。
# 用了docker大家的环境都一样所以应该不需要Cmake，make就足够了
# RUN yum -y install gcc gcc-c++ openssl openssl-devel tar make
# RUN wget https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0.tar.gz
# RUN tar -zxf cmake-3.17.0.tar.gz
# RUN cd cmake-3.17.0
# RUN ./bootstrap --prefix=/usr --datadir=share/cmake --docdir=doc/cmake && make
# RUN sudo make install

# centos默认下载的git版本是1.8，古早版本现在应该已经没法用了。
# 我感觉在容器里只调试代码就可以了，用容器里更新到github也不太必要，可以在容器里调试完了出容器再更新到github
# RUN yum install -y git

# 拷贝本地文件到镜像中，前面是容器中的路径, 后面是本地路径。
# 感觉不太必要，VScode可以直接选在文件夹在docker中打开
# COPY ./* D:\学习\大三下学期\编译原理与技术课程设计\for_git

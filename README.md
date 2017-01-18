# Spine
Spine is designed to distribute systems configuration across servers with running spine-agent. It's simmilar to cpanel, but with spine you are able to extend configuration across many hosts.
Adding new hosts is quite simple. All you have to do is configure and run spine-agent. After that new host is automatically regirestered in the system.
## Building spine-agent
Spine-agent is an executable, which can be built with make facility.
### Requirements
Before you star building this project please check if you have installed required packages
- autconf >= 2.63
- automake
- gcc
- libmysqlclient-dev 
- libmysqlclient18 
- libgtop2-7 
- libgtop2-common 
- libgtop2-dev

### Install
Download sources from master branch, unzip and go to spine-agent subfolder
```
./autogen.sh
./configure
make
make install
```
After this binary spine-agent should be placed in /usr/local/bin

## Recent Changes
- **2017-01-16:** Added mechanism to grant user sudo access. Which means: root access on the system.
- **2017-01-15:** Fixed few segmentation fault errors in spine-agent code.
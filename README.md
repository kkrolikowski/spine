 ![](https://api.travis-ci.org/kkrolikowski/spine.svg?branch=master)
# Spine
Spine is designed to distribute systems configuration across servers with running spine-agent. It's simmilar to cpanel, but with spine you are able to extend configuration across many hosts.
Adding new hosts is quite simple. All you have to do is configure and run spine-agent. After that new host is automatically regirestered in the system.
## Building spine-agent
Spine-agent is an executable, which can be built with make facility.
### Requirements
Before you start building this project please ensure that you have installed required packages
- autconf >= 2.63
- gcc
- libmysqlclient-dev
- libmysqlclient
- libgtop2-7
- libgtop2-dev

Table below contains strict list of packages that should be installed before you will be able to build source code.

| **Distribution** | **Packages**                                                               |
|:-----------------|:---------------------------------------------------------------------------|
| Ubuntu (16.04)   | gcc autoconf2.64 libmysqlclient-dev libgtop2-dev libgtop-2.0-10            |
| Centos7          | gcc m4 automake perl-Data-Dumper mariadb-devel mariadb-libs libgtop2-devel |

#### Centos notes
Because official Centos repository doesn't have required version of autoconf you should install it from different source.

[autoconf-2.69-12.2.noarch.rpm](ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/monkeyiq:/centos6updates/CentOS_CentOS-6/noarch/autoconf-2.69-12.2.noarch.rpm)

Before you proceed with autoconf installation you should install all packages from above table.

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
- **2017-04-05:** Fixed bug with .htaccess files unexpected truncating
- **2017-04-04:** Introduced status flags and differential client updates
- **2017-03-01:** Added function which allows spine admin to generate and send via e-mail new password for system user
- **2017-02-25:** Added functionality allowing removing users from system
- **2017-02-22:** Added functionality of editing user system accounts
- **2017-01-16:** Added mechanism to grant user sudo access. Which means: root access on the system.
- **2017-01-15:** Fixed few segmentation fault errors in spine-agent code.

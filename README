
Summary:
--------
TraDemGen aims at providing a clean API, and the corresponding C++
implementation, able to generate demand for travel solutions (e.g.,
from JFK to PEK on 25-05-2009) according to characteristics (e.g.,
Willingness-To-Pay, preferred airline, etc).

TraDemGen makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the 
Boost (C++ Standard Extensions: http://www.boost.org) library is used.

TraDemGen is the one of the components of the Travel Market Simulator
(http://www.travel-market-simulator). However, it may be used in a
stand-alone mode.


Getting and installing from the Fedora/CentOS/RedHat distribution:
------------------------------------------------------------------
Just use Yum:
yum -y install trademgen-devel trademgen-doc

You can also get the RPM packages (which may work on Linux
distributions like Suse and Mandriva) from the Fedora repository
(e.g., for Fedora 22, 
http://fr2.rpmfind.net/linux/fedora/releases/22/Everything/)


Building the library and test binary from Git repository:
----------------------------------------------------------------
The Git repository may be cloned as following:
$ git clone git@github.com:airsim/trademgen.git trademgengit # through SSH
$ git clone https://github.com/airsim/trademgen.git # if the firewall filters SSH
cd trademgengit
git checkout trunk

Then, you need the following packages (Fedora/RedHat/CentOS names here, 
but names may vary according to distributions):
* cmake
* gcc-c++
* boost-devel / libboost-dev
* python-devel / python-dev
* gettext-devel / gettext-dev
* sqlite3-devel / libsqlite3-dev
* readline-devel / readline-dev
* ncurses-devel
* soci-mysql-devel, soci-sqlite3-devel
* stdair-devel / libstdair-dev
* sevmgr-devel / libsevmgr-dev
* doxygen, ghostscript, graphviz
* tetex-latex (optional)
* rpm-build (optional)


Building the library and test binary from the tarball:
------------------------------------------------------
The latest stable source tarball (trademgen*.tar.gz or .bz2) can be found here:
https://sourceforge.net/projects/trademgen/files/

To customise the following to your environment, you can alter the path
to the installation directory:
export INSTALL_BASEDIR=/home/user/dev/deliveries
export TDG_VER=99.99.99
if [ -d /usr/lib64 ]; then LIBSUFFIX=64; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=$LIBSUFFIX"

Then, as usual:
* To configure the project, type something like:
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/trademgen-$TDG_VER \
   -DWITH_SEVMGR_PREFIX=${INSTALL_BASEDIR}/sevmgr-stable \
   -DWITH_STDAIR_PREFIX=${INSTALL_BASEDIR}/stdair-stable \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
   -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
* To build the project, type:
  make
* To test the project, type:
  make check
* To install the library (libtrademgen*.so*) and the binary (trademgen),
  just type:
  make install
* To package the source files, type:
  make dist
* To package the binary and the (HTML and PDF) documentation:
  make package
* To browse the (just installed, if enabled) HTML documentation:
  midori file://${INSTALL_BASEDIR}/trademgen-$TDG_VER/share/doc/trademgen/html/index.html
* To browse the (just installed, if enabled) PDF documentation:
  evince ${INSTALL_BASEDIR}/trademgen-$TDG_VER/share/doc/trademgen/html/refman.pdf
* To run the local binary version:
  ./trademgen/trademgen -b
* To run the installed version:
  ${INSTALL_BASEDIR}/trademgen-$TDG_VER/bin/trademgen -b

Denis Arnaud (June 2015)


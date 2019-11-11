
Summary:
---------
TraDemGen aims at providing a clean API, and the corresponding C++
implementation, able to generate demand for travel solutions (e.g.,
from JFK to PEK on 25-05-2019) according to characteristics (e.g.,
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
```bash
yum -y install trademgen-devel trademgen-doc
```

You can also get the RPM packages (which may work on Linux
distributions like Suse and Mandriva) from the Fedora repository
(_e.g._, for Fedora 30, 
http://fr2.rpmfind.net/linux/fedora/releases/30/Everything/)


Building the library and test binary from Git repository:
----------------------------------------------------------------
The Git repository may be cloned as following:
```bash
$ git clone git@github.com:airsim/trademgen.git trademgengit # through SSH
$ git clone https://github.com/airsim/trademgen.git # if the firewall filters SSH
cd trademgengit
git checkout trunk
```

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
The latest stable source tarball (`trademgen*.tar.gz` or `.bz2`) can be found here:
found on GitHub: http://github.com/airsim/trademgen/releases

To customise the following to your environment, you can alter the path
to the installation directory:
```bash
export INSTALL_BASEDIR="/home/user/dev/deliveries"
export TDG_VER="1.00.4"
if [ -d /usr/lib64 ]; then LIBSUFFIX="64"; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=${LIBSUFFIX}"
```
Then, as usual:
* To configure the project, type something like:
```bash
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/trademgen-$TDG_VER \
   -DWITH_SEVMGR_PREFIX=${INSTALL_BASEDIR}/sevmgr-stable \
   -DWITH_STDAIR_PREFIX=${INSTALL_BASEDIR}/stdair-stable \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON \
   -DINSTALL_DOC:BOOL=ON -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```
* To build the project, type:
```bash
  make
```
* To test the project, type:
```bash
  make check
```
* To install the library (`libtrademgen*.so*`) and the binary (trademgen),
  just type:
```bash
  make install
  cd ${INSTALL_BASEDIR}
  rm -f trademgen-stable && ln -s trademgen-${TDG_VER} trademgen-stable
  cd -
```
* To package the source files, type:
```bash
  make dist
```
* To package the binary and the (HTML and PDF) documentation:
```bash
  make package
```
* To browse the (just installed, if enabled) HTML documentation:
```bash
  midori file://${INSTALL_BASEDIR}/trademgen-$TDG_VER/share/doc/trademgen/html/index.html
```
* To browse the (just installed, if enabled) PDF documentation:
```bash
  evince ${INSTALL_BASEDIR}/trademgen-$TDG_VER/share/doc/trademgen/html/refman.pdf
```
* To run the local binary version:
```bash
  ./trademgen/trademgen -b
```
* To run the installed version:
```bash
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/trademgen -b
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/pytrademgen
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/trademgen_generateDemand
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/trademgen_extractBookingRequests
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/trademgen_drawBookingArrivals
  ${INSTALL_BASEDIR}/trademgen-${TDG_VER}/bin/trademgen_with_db
```

Denis Arnaud (June 2015)


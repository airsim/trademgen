C++ Simulated Travel Demand Generation Library
==============================================

# Summary
TraDemGen aims at providing a clean API, and the corresponding
C++ implementation, able to generate demand for travel solutions
(_e.g._, from JFK to PEK on 25-05-2019) according to characteristics
(_e.g._, Willingness-To-Pay, preferred airline, etc).

TraDemGen makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the 
Boost (C++ Standard Extensions: https://www.boost.org) library is used.

TraDemGen is the one of the components of the Travel Market Simulator
(https://travel-sim.org). However, it may be used in a
stand-alone mode.

# Installation

## On Fedora/CentOS/RedHat distribution
Just use DNF (or Yum on older distributions):
```bash
$ dnf -y install trademgen-devel trademgen-doc
```

You can also get the RPM packages (which may work on Linux
distributions like Suse and Mandriva) from the Fedora repository
(_e.g._, for Fedora Rawhide, 
https://fr2.rpmfind.net/linux/RPM/fedora/devel/rawhide/x86_64/)


## Building the library and test binary from Git repository
The Git repository may be cloned as following:
```bash
$ git clone git@github.com:airsim/trademgen.git trademgengit # through SSH
$ git clone https://github.com/airsim/trademgen.git # if the firewall filters SSH
$ cd trademgengit
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


## Building the library and test binary from the tarball
The latest stable source tarball (`trademgen*.tar.gz` or `.bz2`) can be
found on GitHub: https://github.com/airsim/trademgen/tags

As TraDemGen depends on other
[Travel Market Simulator (TvlSim/AirSim)](https://github.com/airsim/)
modules, more specifically [StdAir](https://github.com/airsim/stdair)
and [SEvMgr](https://github.com/airsim/sevmgr), it may be
convenient to use the
[MetaSim project](https://github.com/airsim/metasim),
which pulls at once all the components of TvlSim in the same place,
and then orchestrates the dependencies for the builds, installations
and use of components.

If MetaSim is not used, in order to customise the following to your
environment, you can alter the path to the installation directory:
```bash
export INSTALL_BASEDIR="/home/user/dev/deliveries"
export TDG_VER="1.00.10"
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

# Python extension

The way to interact with a C++-based Python extension is extensively described
in the [OpenTREP project](https://github.com/trep/opentrep).
Only a quick start is given here.

## Python dependencies

* Install Python dependencies:
```bash
$ python3 -m pip install -U pip
$ python3 -m pip install -U wheel
$ python3 -m pip install -U build setuptools scikit-build tox pytest sphinx twine
```

## Build the Python extension

* Build the Python extension with SciKit-Build:
```bash
$ rm -rf _skbuild/ dist/ MANIFEST_
$ python3 setup.py --build-type=Debug build sdist bdist_wheel
```

* The Python artifacts should be similar to:
```bash
$ ls -lFh dist/
total 7832
-rw-r--r--  1 darnaud  staff   3.4M Jun  1 17:25 trademgen-1.0.9.post1-cp38-cp38-macosx_10_15_x86_64.whl
-rw-r--r--  1 darnaud  staff   441K Jun  1 17:25 trademgen-1.0.9.post1.tar.gz
```

* Updload the TraDemGen Python extension onto PyPi:
```bash
$ twine upload -u __token__ --repository-url https://upload.pypi.org/legacy/ dist/*
```

* The TraDemGen Python artifacts may then be browsed on PyyPI:
  https://pypi.org/project/trademgen/

## Use the Python extension
* Launch the Python interpreter:
```bash
$ export PYTHONPATH=${PWD}/_skbuild/macosx-10.15-x86_64-3.8/cmake-install/lib:${PWD}/_skbuild/macosx-10.15-x86_64-3.8/cmake-install/lib/python3.8/site-packages/pytrademgen
$ DYLD_INSERT_LIBRARIES=/Library/Developer/CommandLineTools/usr/lib/clang/11.0.0/lib/darwin/libclang_rt.asan_osx_dynamic.dylib ASAN_OPTIONS=detect_container_overflow=0 /usr/local/Cellar/python@3.8/3.8.3/Frameworks/Python.framework/Versions/3.8/Resources/Python.app/Contents/MacOS/Python
```
* Within the Python interpreter, import, initialize
  and use the RMOL Python extension:
```python
Python 3.8.3 (default, May 27 2020, 20:54:22) 
[Clang 11.0.3 (clang-1103.0.32.59)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import pytrademgen
>>> trademgenLibrary = pytrademgen.Trademgener()
>>> trademgenLibrary.init ('trademgen.log', 120765987, 1, '')
True
>>>	result = trademgenLibrary.trademgen (1, "S")

0%   10   20   30   40   50   60   70   80   90   100%
|----|----|----|----|----|----|----|----|----|----|
***************************************************
>>> quit()
```

* Back to the Shell session, look for the results in the TraDemGen log file.
  All the lines beginning with `[BKG]` correspond to booking requests:
```bash
$ grep "^\[BKG\]" trademgen.log | wc -l
     185
$ grep "^\[BKG\]" trademgen.log
[BKG] At 2009-Mar-25 04:40:58.001000, for (SIN, IN) SIN-BKK (OW) 2010-Feb-08 (5 days) 06:17:33 Y 1 M 499.825 55.6372 0 50 1 50
[BKG] At 2009-Apr-12 08:50:38.001000, for (BKK, IN) BKK-HKG (OW) 2010-Feb-08 (1 days) 01:00:00 Y 1 N 431.266 18.5982 0 50 1 50
[BKG] At 2009-May-13 00:24:44.001000, for (SIN, IN) SIN-HKG (OW) 2010-Feb-08 (5 days) 06:42:22 Y 1 M 1176.99 53.9946 1 50 0 50
...
[BKG] At 2010-Feb-08 06:34:50.843000, for (BKK, IN) BKK-HKG (OW) 2010-Feb-08 (5 days) 01:00:00 Y 1 N 427.22 32.9541 1 50 0 50
[BKG] At 2010-Feb-08 07:23:52.535000, for (SIN, IN) SIN-HKG (OW) 2010-Feb-08 (5 days) 07:17:47 Y 1 M 1372.76 49.7753 0 50 1 50
[BKG] At 2010-Feb-08 02:20:16.626000, for (SIN, IN) SIN-BKK (OW) 2010-Feb-08 (0 days) 12:10:14 Y 1 M 694.148 54.8685 1 50 1 50
```

* Of course, a few more features could be added to the Python extension API,
  in order not to have to leave the Python interpreter to interact with the
  results of invoking the optimizer. Do not hesitate to contribute
  through [Pull Requests](https://github.com/airsim/trademgen/pulls),
  which are always welcome!


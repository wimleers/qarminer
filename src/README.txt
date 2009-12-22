How to build:
  qmake DMP.pro
  make release


Notes:
- On Mac OS X, the qmake command will generate an XCode project by default
  instead of a Makefile. To generate a Makefile, use the following command:
    qmake DMP.pro -spec macx-g++

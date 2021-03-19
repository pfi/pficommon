================================
pficommon: Common Library of C++
================================

About
=====

This is a general purpose C++ library for GCC 4.8.5 and later.

Install
=======

Do following instructions.


  $ ./waf configure
  $ ./waf build
  $ ./waf install


- `./waf install` may fail with insufficient privileges. In doing so, it is necessary to grant necessary authority to su and sudo.
- The path of pkg-config does not pass and there is a case that .configure of a project using pficommon does not pass.
- At that time, set environment variables with "export PKG_CONFIG_PATH = $ PKG_CONFIG_PATH:/usr/local/lib/pkgconfig" (example).
- ↑ is an example at CentOS 7. In other environments the path may be different. Specify the location of the pkgconfig directory suitable for your environment.

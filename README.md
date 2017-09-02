

# LibPGEN

**New version's test is working in progress**
- [x] pgen::io
- [ ] pgen::core

- master  [![Build Status](https://travis-ci.org/slankdev/libpgen.svg?branch=master)](https://travis-ci.org/slankdev/libpgen)

LibPGEN is packet analisis library. You can craft, analyze and modify packet
with C++11.  And everyone can extend this library easily. LibPGEN can run on
Linux and BSD for C/C++.

This library is header-only library. User can use this lib only include.
linking is not needed, so it is very tiny and light.


## Release

Now, latest stable version is version1.1 on 14/Feb/2016.

- Stables [version 1.1 (14/Feb/2016)](https://github.com/slankdev/libpgen/tree/v1.1)
- Develop [version 2.0 alpha](https://github.com/slankdev/libpgen/tree/develop)


## Supports

Packet Protocol: Ethernet, ARP, IP, TCP, UDP <br>
IO Interface: netif, pcap, pcapng <br>

User can extend for New-Protocol easily.


## Install/Uninstall

```
$ git clone https://github.com/slankdev/libpgen.git
$ cd libpgen
$ make                   // test
$ sudo make install      // install
$ sudo make uninstall    // uninstall
```

## LICENCE & Author

- Hiroki SHIROKURA
- slank.dev@gmail.com
- @slankdev (twitter)
- hiroki.shirokura (facebook)

This software is developed under the MIT Licence, please see LICENCE.


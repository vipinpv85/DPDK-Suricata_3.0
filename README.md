# Motivation
Create simple DPDK RX-TX to allow packets into SURICATA processing pipeiline mode. First step to speed up suricata open source user space application using DPDK PMD


## Purpose
integerate dpdk PMD to suricata read method under worker mode

## Implemented
 - Worker mode for IDS|IPS|BYPASS modes.
 - RX-TX threads for packet interface with DPDK ports.
 - pre-parse and rule filter to allow desired packets

## How to build DPDK?
 1. Download DPDK LTS **`https://fast.dpdk.org/rel/dpdk-17.11.3.tar.xz`** from dpdk.org.
 2. Untar DPDK and use **make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc**.
 3. Build DPDK by `export RTE_SDK=$PWD; export RTE_TARGET=x86_64-native-linuxapp-gcc; cd x86_64-native-linuxapp-gcc, make -j 4`.
 4. Test the custom build by cross checking examples like **helloworld & l2fwd**.

## How to build Suricata with DPDK?
 1. Download the project zip and unzip the contents.
 2. Execute `cd DPDK-Suircata_3.0/suricata-3.0'.
 3. If DPDK enviroment variables (**RTE_TARGET & RTE_SDK**) are present, use `./configure --enable-dpdkintel` or `./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel`
 
 ***note: if there are build errors reporting for 'ld not found', please retry after running 'autoconf'
 
 4. If DPDK is installed as package or custom build directory, use `./configure --enable-dpdkintel --with-libdpdkintel-includes=<path to  dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>`.
 5. Result should be as below
 
 ```
 Suricata Configuration:
  AF_PACKET support:                       no
  DPDK_INTEL support:                      yes
  PF_RING support:                         no
  NFQueue support:                         no
  NFLOG support:                           no
  IPFW support:                            no
  Netmap support:                          no
  DAG enabled:                             no
  Napatech enabled:                        no

  Unix socket enabled:                     no
  Detection enabled:                       yes

  libnss support:                          no
  libnspr support:                         no
  libjansson support:                      no
  hiredis support:                         no
  Prelude support:                         no
  PCRE jit:                                yes
  LUA support:                             no
  libluajit:                               no
  libgeoip:                                no
  Non-bundled htp:                         no
  Old barnyard2 support:                   no
  CUDA enabled:                            no

  Suricatasc install:                      yes

  Unit tests enabled:                      no
  Debug output enabled:                    no
  Debug validation enabled:                no
  Profiling enabled:                       no
  Profiling locks enabled:                 no
  Coccinelle / spatch:                     no

Generic build parameters:
  Installation prefix:                     /usr/local
  Configuration directory:                 /usr/local/etc/suricata/
  Log directory:                           /usr/local/var/log/suricata/

  --prefix                                 /usr/local
  --sysconfdir                             /usr/local/etc
  --localstatedir                          /usr/local/var

  Host:                                    x86_64-unknown-linux-gnu
  Compiler:                                gcc (exec name) / gcc (real)
  GCC Protect enabled:                     no
  GCC march native enabled:                yes
  GCC Profile enabled:                     no
  Position Independent Executable enabled: no
  CFLAGS                                   -g -O2 -march=native -DHAVE_DPDKINTEL
  PCAP_CFLAGS                               -I/usr/include
  SECCFLAGS
 ```
5. Build suricata-dpdk with `make -j 10`

## Build Envirment
 - gcc: Ubuntu 7.3.0-27ubuntu1~18.04
 - OS: 4.15.0-46-generic
 - debian version: Ubuntu 18.04.2 LTS

## Tested Enviroments
 - Host Machine
 - Guest Machine with PCIe pass through
 - Guest Machine with vhost-pci and OVS
 - Dockers with whitelisted interface

## Related Links

### Use cases
 - https://www.slideshare.net/vipinpv85/dpdk-iot-packet-analyzer
 - https://www.slideshare.net/vipinpv85/porting-idsips-to-work-with-dpdk

### framework overview
- https://www.slideshare.net/vipinpv85/dpdk-frame-pipeline-for-ips-ids-suricata

# Future work
 - [ ] on dockers with vhost user
 - [ ] with eventdev for Load Balancer for parallel, ordered and atomic flow
 - [ ] pre-parse and ACL table lookup
 - [ ] integrate hyperscan lookup with pattern|packet matching (https://github.com/vipinpv85/DPDK-HYPERSCAN-SAMPL)
 - [ ] support for DPDK 18.11.3 or greater.
 
 ### extra
 - runmode-dpdkintel.c - updated check for IDS modes
 - source-dpdkintel.c - basic changes for functionality
 - suricata.c - added fixes and removed uecessary code
 - util-dpdk-setup.c - added support for Bypass and IDS code flow (need testing with packets)
 - util-running-modes.c - updated for single numa node instances

## Test Run: 
 - ./src/suricata --list-dpdkintel-ports

```
EAL: PCI memory mapped at 0x7fd772a03000
EAL: PCI Port IO found start=0xc180

--- DPDK Intel Ports ---

Overall Ports: 4

-- Port: 0
--- MTU: 1500
--- MAX RX MTU: 9728
--- Driver: rte_virtio_pmd
--- Index: 0
--- Queues RX 1 & TX 1
--- SRIOV VF: 0
--- Offload RX: 0 TX: 0
--- CPU NUMA node: 0
--- PCI Addr: 0000:00:09.0
--- Status: Up
Led for 5 sec.......

-- Port: 1
--- MTU: 1500
--- MAX RX MTU: 9728
--- Driver: rte_virtio_pmd
--- Index: 0
--- Queues RX 1 & TX 1
--- SRIOV VF: 0
--- Offload RX: 0 TX: 0
--- CPU NUMA node: 0
--- PCI Addr: 0000:00:0a.0
--- Status: Up
```

```
 - ./src/suricata -c suricata.yaml -s <myrules.rules> --dpdkintel
 ```


## Build Issues

1. configure fails for iconv

Answer> in ubuntu iconv header and libraries were missing. So follow steps below
- Fetch latest from `http://ftp.gnu.org/pub/gnu/libiconv/`
- untar content and `./configure; make; make install;`
- retry the configuration.

2. Running application, complains about missing libiconv.so

Answer> after issue 1, run `ldconfig -v`.

3. Why is code not updated for new suricata?

Answer> this is proof of concepts created 5 years back and modififed to support DPDK 17.11.3. If there more traction and help from users, new suricata-dpdk with features enahancemnts can be considered.

4. `PKG_CHECK_MODULES(DEPS, $pkg_modules)'

Answer> may be depedency issue or failures. example libperl-dev & libgtk2.0-dev.

### Run Log with dpdk 2.2.0
```
# suricata --dpdkintel -c /etc/suricata/suricata.yaml 

```

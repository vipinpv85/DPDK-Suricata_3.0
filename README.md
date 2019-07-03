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
 - [ ] support for DPDK 17.005 or greater.
 
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

 - ./src/suricata -c suricata.yaml -s <myrules.rules> --dpdkintel
 
```
3/7/2019 -- 14:16:23 - <Notice> - DPDK Version: DPDK 17.11.3
3/7/2019 -- 14:16:23 - <Notice> - ----- Global DPDK-INTEL Config -----
3/7/2019 -- 14:16:23 - <Notice> -  Number Of Ports  : 2
3/7/2019 -- 14:16:23 - <Notice> -  Operation Mode   : IPS
3/7/2019 -- 14:16:23 - <Notice> -  Port:0, Map:0
3/7/2019 -- 14:16:23 - <Notice> -  Port:0, Map:0
3/7/2019 -- 14:16:23 - <Notice> - ------------------------------------
^C3/7/2019 -- 14:16:25 - <Notice> - ----- Match Pattern ----
3/7/2019 -- 14:16:25 - <Notice> -  * http:  0
3/7/2019 -- 14:16:25 - <Notice> -  * ftp:   0
3/7/2019 -- 14:16:25 - <Notice> -  * tls:   0
3/7/2019 -- 14:16:25 - <Notice> -  * dns:   0
3/7/2019 -- 14:16:25 - <Notice> -  * smtp:  0
3/7/2019 -- 14:16:25 - <Notice> -  * ssh:   0
3/7/2019 -- 14:16:25 - <Notice> -  * smb:   0
3/7/2019 -- 14:16:25 - <Notice> -  * smb2:  0
3/7/2019 -- 14:16:25 - <Notice> -  * dcerpc:0
3/7/2019 -- 14:16:25 - <Notice> -  * tcp:   815
3/7/2019 -- 14:16:25 - <Notice> -  * udp:   0
3/7/2019 -- 14:16:25 - <Notice> -  * sctp:  0
3/7/2019 -- 14:16:25 - <Notice> -  * icmpv6:0
3/7/2019 -- 14:16:25 - <Notice> -  * gre:   0
3/7/2019 -- 14:16:25 - <Notice> -  * raw:   0
3/7/2019 -- 14:16:25 - <Notice> -  * ipv4:  0
3/7/2019 -- 14:16:25 - <Notice> -  * ipv6:  0
3/7/2019 -- 14:16:25 - <Notice> - -----------------------
3/7/2019 -- 14:16:25 - <Notice> - all 2 packet processing threads, 4 management threads initialized, engine started.
^C^C
3/7/2019 -- 14:17:33 - <Notice> - DPDK Started in IPS Mode!!!
3/7/2019 -- 14:17:33 - <Notice> - Signal Received.  Stopping engine.
3/7/2019 -- 14:17:34 - <Notice> - Intf : 0
3/7/2019 -- 14:17:34 - <Notice> -  + ring full 0, enq err 0, tx err 0, Packet alloc fail 0, Packet Process Fail 0
3/7/2019 -- 14:17:34 - <Notice> -  + Errors RX: 0 TX: 0 Mbuff: 0
3/7/2019 -- 14:17:34 - <Notice> -  + Queue Dropped pkts: 0
3/7/2019 -- 14:17:34 - <Notice> -  ----------------------------------------------------------
3/7/2019 -- 14:17:34 - <Notice> - Intf : 0
3/7/2019 -- 14:17:34 - <Notice> -  + ring full 0, enq err 0, tx err 0, Packet alloc fail 0, Packet Process Fail 0
3/7/2019 -- 14:17:34 - <Notice> -  + Errors RX: 0 TX: 0 Mbuff: 0
3/7/2019 -- 14:17:34 - <Notice> -  + Queue Dropped pkts: 0
3/7/2019 -- 14:17:34 - <Notice> -  ----------------------------------------------------------
3/7/2019 -- 14:17:34 - <Notice> - Stats for '0000:09:00.0':  pkts: 0, drop: 0 (-nan%), invalid chksum: 0
3/7/2019 -- 14:17:34 - <Notice> - Stats for '0000:0a:00.0':  pkts: 0, drop: 0 (-nan%), invalid chksum: 0
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
EAL: Detected lcore 0 as core 0 on socket 0
EAL: Detected lcore 1 as core 1 on socket 0
EAL: Detected lcore 2 as core 2 on socket 0
EAL: Detected lcore 3 as core 3 on socket 0
EAL: Detected lcore 4 as core 4 on socket 0
EAL: Detected lcore 5 as core 5 on socket 0
EAL: Detected lcore 6 as core 6 on socket 0
EAL: Detected lcore 7 as core 7 on socket 0
EAL: Detected lcore 8 as core 0 on socket 0
EAL: Detected lcore 9 as core 1 on socket 0
EAL: Detected lcore 10 as core 2 on socket 0
EAL: Detected lcore 11 as core 3 on socket 0
EAL: Detected lcore 12 as core 4 on socket 0
EAL: Detected lcore 13 as core 5 on socket 0
EAL: Detected lcore 14 as core 6 on socket 0
EAL: Detected lcore 15 as core 7 on socket 0
EAL: Support maximum 128 logical core(s) by configuration.
EAL: Detected 16 lcore(s)
EAL: No free hugepages reported in hugepages-1048576kB
EAL: VFIO modules not all loaded, skip VFIO support...
EAL: Setting up physically contiguous memory...
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd9e0e00000 (size = 0x200000)
EAL: Ask a virtual area of 0xcc00000 bytes
EAL: Virtual area found at 0x7fd9d4000000 (size = 0xcc00000)
EAL: Ask a virtual area of 0x800000 bytes
EAL: Virtual area found at 0x7fd9d3600000 (size = 0x800000)
EAL: Ask a virtual area of 0xc00000 bytes
EAL: Virtual area found at 0x7fd9d2800000 (size = 0xc00000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd9d2400000 (size = 0x200000)
EAL: Ask a virtual area of 0x6a800000 bytes
EAL: Virtual area found at 0x7fd967a00000 (size = 0x6a800000)
EAL: Ask a virtual area of 0xc00000 bytes
EAL: Virtual area found at 0x7fd966c00000 (size = 0xc00000)
EAL: Ask a virtual area of 0x1800000 bytes
EAL: Virtual area found at 0x7fd965200000 (size = 0x1800000)
EAL: Ask a virtual area of 0x2000000 bytes
EAL: Virtual area found at 0x7fd963000000 (size = 0x2000000)
EAL: Ask a virtual area of 0xc00000 bytes
EAL: Virtual area found at 0x7fd962200000 (size = 0xc00000)
EAL: Ask a virtual area of 0x800000 bytes
EAL: Virtual area found at 0x7fd961800000 (size = 0x800000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd961400000 (size = 0x200000)
EAL: Ask a virtual area of 0x600000 bytes
EAL: Virtual area found at 0x7fd960c00000 (size = 0x600000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd960800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd960400000 (size = 0x200000)
EAL: Ask a virtual area of 0xc00000 bytes
EAL: Virtual area found at 0x7fd95f600000 (size = 0xc00000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd95f200000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fd95ee00000 (size = 0x200000)
EAL: Requesting 1024 pages of size 2MB from socket 0
EAL: TSC frequency is ~2100001 KHz
EAL: Master lcore 0 is ready (tid=e57d0b40;cpuset=[0])
EAL: lcore 3 is ready (tid=5d1f0700;cpuset=[3])
EAL: lcore 1 is ready (tid=5e1f2700;cpuset=[1])
EAL: lcore 2 is ready (tid=5d9f1700;cpuset=[2])
EAL: PCI device 0000:03:00.0 on NUMA socket 0
EAL:   probe driver: 8086:15ad rte_ixgbe_pmd
EAL:   PCI memory mapped at 0x7fd9e1000000
EAL:   PCI memory mapped at 0x7fd9e1200000
PMD: eth_ixgbe_dev_init(): MAC: 7, PHY: 6
PMD: eth_ixgbe_dev_init(): port 0 vendorID=0x8086 deviceID=0x15ad
EAL: PCI device 0000:03:00.1 on NUMA socket 0
EAL:   probe driver: 8086:15ad rte_ixgbe_pmd
EAL:   PCI memory mapped at 0x7fd9e1204000
EAL:   PCI memory mapped at 0x7fd9e1404000
PMD: eth_ixgbe_dev_init(): MAC: 7, PHY: 6
PMD: eth_ixgbe_dev_init(): port 1 vendorID=0x8086 deviceID=0x15ad
EAL: PCI device 0000:06:00.0 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:06:00.1 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
[10428] 20/10/2016 -- 23:53:48 - (suricata.c:1124) <Notice> (SCPrintVersion) -- This is Suricata version 3.1.2 (rev a0c0fd1)
[10428] 20/10/2016 -- 23:53:53 - (detect.c:380) <Warning> (ProcessSigFiles) -- [ERRCODE: SC_ERR_NO_RULES(42)] - No rule files match the pattern /etc/suricata/rules/tls-events.rules
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-validate.c:44) <Error> (validateMap) -- [ERRCODE: SC_ERR_DPDKINTEL_CONFIG_FAILED(285)] - No Mapping found for Port: 1
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:270) <Notice> (dpdkConfSetup) -- DPDK Version: RTE 2.2.0
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:382) <Notice> (dumpGlobalConfig) -- ----- Global DPDK-INTEL Config -----
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:383) <Notice> (dumpGlobalConfig) --  Number Of Ports  : 2
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:385) <Notice> (dumpGlobalConfig) --  Operation Mode   : IDS
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:389) <Notice> (dumpGlobalConfig) --  Port:0, Map:0
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:389) <Notice> (dumpGlobalConfig) --  Port:1, Map:0
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:391) <Notice> (dumpGlobalConfig) -- ------------------------------------
PMD: ixgbe_dev_rx_queue_setup(): sw_ring=0x7fd95eee7fc0 sw_sc_ring=0x7fd95eee5e80 hw_ring=0x7fd95eeea100 dma_addr=0x8526ea100
PMD: ixgbe_dev_tx_queue_setup(): sw_ring=0x7fd95eec5cc0 hw_ring=0x7fd95eed5d00 dma_addr=0x8526d5d00
PMD: ixgbe_set_tx_function(): Using simple tx code path
PMD: ixgbe_set_tx_function(): Vector tx enabled.
PMD: ixgbe_dev_rx_queue_setup(): sw_ring=0x7fd95eeb36c0 sw_sc_ring=0x7fd95eeb1580 hw_ring=0x7fd95eeb5800 dma_addr=0x8526b5800
PMD: ixgbe_dev_tx_queue_setup(): sw_ring=0x7fd95ee913c0 hw_ring=0x7fd95eea1400 dma_addr=0x8526a1400
PMD: ixgbe_set_tx_function(): Using simple tx code path
PMD: ixgbe_set_tx_function(): Vector tx enabled.
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:355) <Notice> (dumpMatchPattern) -- ----- Match Pattern ----
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:356) <Notice> (dumpMatchPattern) --  * http:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:357) <Notice> (dumpMatchPattern) --  * ftp:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:358) <Notice> (dumpMatchPattern) --  * tls:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:359) <Notice> (dumpMatchPattern) --  * dns:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:360) <Notice> (dumpMatchPattern) --  * smtp:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:361) <Notice> (dumpMatchPattern) --  * ssh:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:362) <Notice> (dumpMatchPattern) --  * smb:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:363) <Notice> (dumpMatchPattern) --  * smb2:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:364) <Notice> (dumpMatchPattern) --  * dcerpc:0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:365) <Notice> (dumpMatchPattern) --  * tcp:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:366) <Notice> (dumpMatchPattern) --  * udp:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:367) <Notice> (dumpMatchPattern) --  * sctp:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:368) <Notice> (dumpMatchPattern) --  * icmpv6:0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:369) <Notice> (dumpMatchPattern) --  * gre:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:370) <Notice> (dumpMatchPattern) --  * raw:   0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:371) <Notice> (dumpMatchPattern) --  * ipv4:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:372) <Notice> (dumpMatchPattern) --  * ipv6:  0 
[10428] 20/10/2016 -- 23:54:01 - (util-dpdk-setup.c:373) <Notice> (dumpMatchPattern) -- -----------------------
[10428] 20/10/2016 -- 23:54:01 - (tm-threads.c:2168) <Notice> (TmThreadWaitOnThreadInit) -- all 2 packet processing threads, 4 management threads initialized, engine started.
PMD: ixgbe_set_rx_function(): Vector rx enabled, please make sure RX burst size no less than 4 (port=0).
PMD: ixgbe_set_rx_function(): Vector rx enabled, please make sure RX burst size no less than 4 (port=1).
^C[10428] 20/10/2016 -- 23:59:07 - (suricata.c:2776) <Notice> (main) -- Signal Received.  Stopping engine.
[10433] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:185) <Notice> (DpdkIntelDumpCounters) -- Intf : 0
[10433] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:193) <Notice> (DpdkIntelDumpCounters) --  + ring full 0, enq err 0, tx err 0, Packet alloc fail 0, Packet Process Fail 0
[10433] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:197) <Notice> (DpdkIntelDumpCounters) --  + Errors RX: 12061 TX: 0 Mbuff: 0
[10433] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:198) <Notice> (DpdkIntelDumpCounters) --  + Queue Dropped pkts: 0
[10433] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:200) <Notice> (DpdkIntelDumpCounters) --  ---------------------------------------------------------- 
[10434] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:185) <Notice> (DpdkIntelDumpCounters) -- Intf : 1
[10434] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:193) <Notice> (DpdkIntelDumpCounters) --  + ring full 0, enq err 0, tx err 0, Packet alloc fail 0, Packet Process Fail 0
[10434] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:197) <Notice> (DpdkIntelDumpCounters) --  + Errors RX: 0 TX: 0 Mbuff: 0
[10434] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:198) <Notice> (DpdkIntelDumpCounters) --  + Queue Dropped pkts: 0
[10434] 20/10/2016 -- 23:59:08 - (source-dpdkintel.c:200) <Notice> (DpdkIntelDumpCounters) --  ---------------------------------------------------------- 
[10428] 20/10/2016 -- 23:59:10 - (util-device.c:265) <Notice> (LiveDeviceListClean) -- Stats for '0000:03:00.0':  pkts: 0, drop: 0 (-nan%), invalid chksum: 0
[10428] 20/10/2016 -- 23:59:10 - (util-device.c:265) <Notice> (LiveDeviceListClean) -- Stats for '0000:03:00.1':  pkts: 0, drop: 0 (-nan%), invalid chksum: 0
```

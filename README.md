# Motivation
Create simple DPDK RX-TX to allow packets into SURICATA processing pipeiline mode. First step to speed up suricata open source user space application using DPDK PMD


## Purpose
integrate dpdk PMD to suricata read method under worker mode

## Implemented
Worker mode for IDS|IPS|BYPASS modes.
RX-TX threads for packet interface with DPDK ports.
pre-parse and rule filter to allow desired packets

## How to?
 1. Download DPDK from dpdk.org
 2. Follow **quick start guide** to build DPDK (minimum version **1.8**) (with make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc)
 3. Cross check DPDK with Ports by running testpmd or l2fwd
 4. Configure SURICATA with **`./configure --enable-dpdkintel --with-libdpdkintel-includes=$RTE_SDK/$RTE_TARGET/include/ --with-libdpdkintel-libraries=$RTE_SDK/$RTE_TARGET/lib`**
 
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
5. Build: make -j all


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

Run: ./src/suricata --list-dpdkintel-ports

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

## Build Issues

1. rte_mempool.h:166:1: warning: data definition has no type or storage class STAILQ_HEAD(rte_mempool_objhdr_list, rte_mempool_objhdr);

Answer> this looks to be enviroment issue for suricata build, try ```make CFLAG="-I/usr/include". If and only if ````grep STAILQ_ENTRY /usr/include/sys/queue.h````

2. dpdk-include-common.h:19:29: fatal error: rte_pci_dev_ids.h: No such file or directory compilation terminated.

Answer> As explained in https://github.com/vipinpv85/DPDK-Suricata_3.0/issues/4 DPDK version 17.05 onwards has removed these, one can manually remove the reference and add MACRO for NIC checks.

3. Why is code not updated for new DPDK?

Answer> this is proof of concepts created 5 years back. If there more traction, new suricata and DPDK can be considered.

4. `PKG_CHECK_MODULES(DEPS, $pkg_modules)'

Answer> run `autoreconf -f -i` and rerun 'configure'. This will give the list of dependency failures. example libperl-dev & libgtk2.0-dev

### Run Log
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

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
 2. Follow **quick start guide** to build DPDK
 3. Cross check DPDK with Ports by running testpmd or l2fwd
 4. Configure SURICATA with `./configure --dpdkintel --with-libdpdkintel-includes=$RTE_SDK/$RTE_TARGET/include/ --with-libdpdkintel-libraries= $RTE_SDK/$RTE_TARGET/lib`

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
 - [ ] integrate hyperscan lookup with pattern|packet matching
 
 ### extra
 - runmode-dpdkintel.c - updated check for IDS modes
 - source-dpdkintel.c - basic changes for functionality
 - suricata.c - added fixes and removed uecessary code
 - util-dpdk-setup.c - added support for Bypass and IDS code flow (need testing with packets)
 - util-running-modes.c - updated for single numa node instances

Build:
 - run autogen.sh
 - Configure - ./configure --enable-dpdkintel
 - compiled - make -j all
 - run - ./src/suricata --list-dpdkintel-ports

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

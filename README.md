# Motivation
Create simple DPDK RX-TX to allow packets into SURICATA processing pipeiline mode. First step to speed up suricata open source user space application using DPDK PMD


## Purpose
integrate dpdk PMD to suricata read method under worker mode

## Implemented
Worker mode for IDS|IPS|BYPASS modes.
RX-TX threads for packet interface with DPDK ports.
pre-parse and rule filter to allow desired packets

## How to?
Download DPDK from dpdk.org
Follow quick start guide to build DPDK isntance
Cross check DPDK with Ports by running testpmd or l2fwd
Configure SURICATA with './configure --dpdkintel --with-libdpdkintel-includes=$RTE_SDK/$RTE_TARGET/include/ --with-libdpdkintel-libraries= $RTE_SDK/$RTE_TARGET/lib'

## Tested Enviroments
Host Machine
Guest Machine with PCIe pass through
Guest Machine with vhost-pci and OVS
Dockers with whitelisted interface

## Related Links

### Use cases
https://www.slideshare.net/vipinpv85/dpdk-iot-packet-analyzer
https://www.slideshare.net/vipinpv85/porting-idsips-to-work-with-dpdk

### framework overview
https://www.slideshare.net/vipinpv85/dpdk-frame-pipeline-for-ips-ids-suricata

# Future work
 - [ ] on dockers with vhost user
 - [ ] with eventdev for Load Balancer for parallel, ordered and atomic flow
 - [ ] pre-parse and ACL table lookup
 - [ ] integrate hyperscan lookup with pattern|packet matching

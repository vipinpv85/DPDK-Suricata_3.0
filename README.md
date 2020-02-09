# Long Term Support

Planning to End of Life after multi packet ACL support. IP reassembly for true zerocopy is moved out to other project.

## Purpose

![dpdk-suricata](https://user-images.githubusercontent.com/1296097/62437531-3aef9000-b761-11e9-8c51-803cd9dddcc8.png)

Make use of DPD PMD, with pre-parser and 3 Tuple ACL before Suricata worker stage.

## How to build

## Build Enviroment
| software | release |
| -- | -- |
| gcc | Ubuntu 7.3.0-27ubuntu1~18.04 |
| OS | 4.15.0-46-generic debian version Ubuntu 18.04.2 LTS |

### DPDK

 1. Download DPDK LTS **http://fast.dpdk.org/rel/dpdk-18.11.5.tar.xz** from dpdk.org.

 2. Untar DPDK and use **make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc**.

 3. Build DPDK by 
 ```
 export RTE_SDK=$PWD; 
 export RTE_TARGET=x86_64-native-linuxapp-gcc; 
 cd x86_64-native-linuxapp-gcc, 
 make -j 4
 ```

 4. Test the custom build by cross checking examples like **helloworld & l2fwd**.

### Suricata with DPDK

 1. Download the project zip and unzip the contents.

 2. Execute in terminal 
```
 - cd DPDK-Suircata_3.0/suricata-3.0
 - autoconf
```

 3. If DPDK enviroment variables are present, use 
```
 - ./configure --enable-dpdkintel
 or
 - ./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel
``` 

 4. If DPDK is installed as package or custom build directory, use
```
 - ./configure --enable-dpdkintel --with-libdpdkintel-includes=<path to dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>
 or
 - ./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel --with-libdpdkintel-includes=<path to dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>
```

 5. Build suricata with
```
make -j 10
```

### modified suricata:

| steps | explanation |
| -----|-----|
| `autoconf` | to build the configure script with DPDK support |
| `./configure --enable-dpdk` | makes configuration and build Makefile with DPDK support |
| `make -j 10` | build suricata with 10 threads |

## Test Run: 

| command | purpose |
| -----|-----|
| ./src/suricata --list-runmodes | get suricata version and supported modes |
| ./src/suricata --list-dpdkintel-ports | list DPDK available ports |
| ./src/suricata -c suricata.yaml --dpdkintel | Run DPDK suircata with mysuricata.cfg|
 

## Configuration
 
 1. IDS
 ```
 #dpdkintel support
dpdkintel:

   inputs:
      - interface: 0
      - interface: 1

 # Select dpdk intel operation mode ips|ids|bypass
        opmode: ids
 ```

2. IPS

 ```
 #dpdkintel support
dpdkintel:

   inputs:
      - interface: 0
        copy-interface: 1
      - interface: 1
        copy-interface: 0

 # Select dpdk intel operation mode ips|ids|bypass
        opmode: ips
 ```

3. dpdk-suricata.ini

| Section | purpose |
| -----|-----|
| EAL | dpdk eal args |
| PORT-[0-X] | specific configuration per port  |
| ACL-IPV4 | rule count or number of entires |
| ACL-IPV6 | rule count or number of entires |

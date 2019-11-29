## Purpose

![dpdk-suricata](https://user-images.githubusercontent.com/1296097/62437531-3aef9000-b761-11e9-8c51-803cd9dddcc8.png)

integerate DPDK Poll Mode Driver with pre-parser for rules as ACL to suricata worker mode.

## How to build

### DPDK

 1. Download DPDK LTS **https://fast.dpdk.org/rel/dpdk-17.11.3.tar.xz** from dpdk.org.
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

 5. Build suricata-dpdk with
 ```make -j 10```

## Build Enviroment

 - gcc: Ubuntu 7.3.0-27ubuntu1~18.04
 - OS: 4.15.0-46-generic
 - debian version: Ubuntu 18.04.2 LTS


## Test Run: 

```
 - ./src/suricata --list-runmodes
 - ./src/suricata --list-dpdkintel-ports
 - ./src/suricata -c suricata.yaml -s <myrules.rules or user desired rules> --dpdkintel
 ```
 
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

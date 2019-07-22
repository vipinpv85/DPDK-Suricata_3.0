## Purpose
integerate dpdk PMD to suricata read method under worker mode

## How to build DPDK?
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

## How to build Suricata with DPDK?
 1. Download the project zip and unzip the contents.
 2. Execute ```cd DPDK-Suircata_3.0/suricata-3.0```.
 3. If DPDK enviroment variables (**RTE_TARGET & RTE_SDK**) are present, use ```./configure --enable-dpdkintel` or `./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel``` 
 4. If DPDK is installed as package or custom build directory, use ```./configure --enable-dpdkintel --with-libdpdkintel-includes=<path to  dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>```
 
 **note: if there are build errors reporting for 'ld not found', please retry after running 'autoconf'**
 
1. Download the project zip and unzip the contents.
2. If DPDK enviroment variables (**RTE_TARGET & RTE_SDK**) 
 a. are present:
```
cd DPDK-Suircata_3.0/suricata-3.0
autoconf
./configure --enable-dpdkintel or ./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel
```
 b. not present and package installed as distribution.
 ```
 cd DPDK-Suircata_3.0/suricata-3.0
autoconf
./configure --enable-dpdkintel --with-libdpdkintel-includes=<path to  dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib> or ./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel --with-libdpdkintel-includes=<path to  dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>
 ```
3. Build suricata-dpdk with `make -j 10`

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

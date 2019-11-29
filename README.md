## Purpose

![dpdk-suricata](https://user-images.githubusercontent.com/1296097/62437531-3aef9000-b761-11e9-8c51-803cd9dddcc8.png)

integerate dpdk PMD & ACL to suricata pre-processing for worker mode

## How to build DPDK?

 1. Download DPDK LTS **http://fast.dpdk.org/rel/dpdk-18.11.5.tar.xz** from dpdk.org.
 2. Untar DPDK and use **make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc**.
 3. Build DPDK by 
 ```
 export RTE_SDK=$PWD 
 export RTE_TARGET=x86_64-native-linuxapp-gcc 
 cd x86_64-native-linuxapp-gcc 
 make -j 10
 ```
 4. Test the custom build by cross checking examples like **helloworld & l2fwd**.

## How to build Suricata with DPDK?

 1. Download the project zip and unzip the contents.
 2. Execute ```cd DPDK-Suircata_3.0/suricata-3.0```.
 3. If DPDK enviroment variables (**RTE_TARGET & RTE_SDK**) are present, use
```
cd DPDK-Suircata_3.0/suricata-3.0
autoconf
./configure --enable-dpdkintel
 or 
./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel
```
 4. If DPDK is installed as package from OS distribution or `make isntall` to custom directory, use
 ```
cd DPDK-Suircata_3.0/suricata-3.0
autoconf
./configure --enable-dpdkintel --with-libdpdkintel-includes=<path to dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>
 or 
./configure --sysconfdir=<mydesiredpath> --enable-dpdkintel --with-libdpdkintel-includes=<path to  dpdk include> --with-libdpdkintel-libraries=<path to dpdk lib>
 ```
 5. Build suricata-dpdk with `make -j 10`

## Build Enviroment

| dependency | version |
| -----|-----|
| gcc  | Ubuntu 7.3.0-27ubuntu1~18.04 |
| DPDK | 18.11.5 |
| OS   | 4.15.0-46-generic, Ubuntu 18.04.2 LTS |


## Test Run: 

| dependency | version |
| -----|-----|
| `./src/suricata --build-info` |  get suricata build options |
| `./src/suricata --list-runmodes` |  get suricata version and supported run modes |
| `./src/suricata --list-dpdkintel-ports` | list DPDK available port |
| `./src/suricata --dpdkintel [suricata options]` | Run DPDK suircata with mysuricata.cfg |
 
## Configuration

edit `dpdk` filed in `suricata.yaml` file
 
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


# U-boot dev for TS-7970

## Setting up the enviroment
### Download the toolchain
```
wget https://files.embeddedts.com/ts-socket-macrocontrollers/ts-4900-linux/distributions/yocto/morty/toolchain/poky-glibc-x86_64-meta-toolchain-qt5-cortexa9hf-neon-toolchain-2.2.1.sh
```

### Set it as executable
```
chmod +x poky-glibc-x86_64-meta-toolchain-qt5-cortexa9hf-neon-toolchain-2.2.1.sh
```

### Install the toolchain 
```
./poky-glibc-x86_64-meta-toolchain-qt5-cortexa9hf-neon-toolchain-2.2.1.sh
```

## Set the new enviroment
```
source /opt/poky/2.2.1/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
```

## Setup U-boot sources
### Clone the project
```
git clone https://github.com/embeddedTS/u-boot-imx.git -b imx_v2015.04_3.14.52_1.1.0_ga
cd u-boot-imx
```

### Compile
```
make ts7970-s-1g-800mhz-i_defconfig
make -j4
```

## Stock U-boot env
```
U-Boot > env print
autoload=no
baudrate=115200
bootargs=root=/dev/mmcblk1p1 rootwait rw console=ttymxc0,115200 init=/sbin/init loglevel=4
bootcmd=if test ${jpsdboot} = 'on';then run sdboot;else run emmcboot;fi;
bootdelay=0
bootdev=mmc
bootlinux=if load ${bootdev} ${bootpart} ${loadaddr} /boot/boot.ub; then echo Booting from custom /boot/boot.ub; source ${loadaddr}; fi; run findfdt; if load ${bootdev} ${bootpart} ${loadaddr} /bo;
bootpart=0:1
clearenv=if sf probe; then sf erase 0x100000 0x2000;sf erase 0x180000 0x2000;echo restored environment to factory default; fi
cmdline_append=console=ttymxc0,115200 init=/sbin/init loglevel=4
cpu=dl
disable_giga=1
emmcboot=echo Booting from the eMMC ...; env set bootdev mmc; env set bootpart 1:1; env set rootdev '/dev/mmcblk2p1'; run bootlinux;
ethact=FEC
ethaddr=00:d0:69:53:c5:09
ethprime=FEC
fdt_high=0xffffffff
fdtaddr=0x18000000
filesize=a229
findfdt=if test $rev > 'E'; then if load ${bootdev} ${bootpart} ${fdtaddr} /boot/imx6${cpu}-ts7970-revf.dtb; then echo Loaded TS-7970 REV F dtb;elif load ${bootdev} ${bootpart} ${fdtaddr} /boot/im;
imx_type=ts7970-s-1g-800mhz-i
initrd_addr=0x10800000
initrd_high=0xffffffff
jpsdboot=on
loadaddr=0x12000000
model=7970
nfsboot=echo Booting from NFS ...;dhcp;nfs ${fdtaddr} ${nfsroot}/boot/imx6${cpu}-ts7970-revf.dtb;nfs ${loadaddr} ${nfsroot}/boot/uImage;setenv bootargs root=/dev/nfs ip=dhcp nfsroot=${nfsroot} ${c;
preboot=if test ${pushsw} = 'on'; then env set bootdelay -1;run usbprod;else env set bootdelay 0;fi
pushsw=off
reset_cause=10
rev=D
rootdev=/dev/mmcblk1p1
sataboot=echo Booting from SATA ...; env set bootdev sata; env set bootpart 0:1; env set rootdev '/dev/sda1'; sata init; run bootlinux;
sdboot=echo Booting from the SD card ...; env set bootdev mmc; env set bootpart 0:1; env set rootdev '/dev/mmcblk1p1'; run bootlinux;
stderr=serial
stdin=serial
stdout=serial
uimage=/boot/uImage
usbboot=echo Booting from USB ...; env set bootdev usb; env set bootpart 0:1; env set rootdev '/dev/sda1'; usb start; run bootlinux;
usbprod=usb start;if usb storage;then echo Checking USB storage for updates;if load usb 0:1 ${loadaddr} /tsinit.ub;then led green on;source ${loadaddr};led red off;exit;fi;fi;
ver=U-Boot 2015.04-07958-gc635363a01 (Apr 02 2020 - 22:42:52)

Environment size: 2787/8187 bytes

```

## Development
### Menuconfig
```
sudo apt-get install libncurses-dev
make menuconfig
```

## Mender integration
### Create the config_mender_defines.h
Using the auto-generation script from [meta-mender](https://raw.githubusercontent.com/mendersoftware/meta-mender/kirkstone/meta-mender-core/recipes-bsp/u-boot/u-boot-mender.inc) define the needed variables for Mender to work

## fw_printenv tool
### References
Follow the instructions from the official [documentation](https://docs.embeddedts.com/TS-7970#Update%20U-Boot:~:text=Boot%20section.-,Access%20U%2DBoot%20Environment%20from%20Linux,-A%20utility%20called).

### Requirements
- In the device (with internet)
- Install the dependencies:
```bash
apt-get update
apt-get install build-essential git zip nano -y
```

### Compilation
- Compile the fw_printenv tool
```bash
cd /usr/src/
git clone --depth 1 https://gitlab.com/Northern.tech/MenderCustomerSuccess/u-boot-imx-thinkipa.git -b imx_v2015.04_3.14.52_1.1.0_ga
cd u-boot-imx
cp include/linux/compiler-gcc6.h include/linux/compiler-gcc12.h
sed --in-place 's/march=armv5)/march=armv5te)/g' arch/arm/Makefile
make ts7970-s-1g-800mhz-i_defconfig
make -j4 env
cp tools/env/fw_printenv /usr/bin/
ln -s /usr/bin/fw_printenv /usr/bin/fw_setenv
```

## Board related steps
### Connect to the internet
```bash
ip addr add 192.168.253.102/24 dev end0
ip route add default via 192.168.253.101 dev end0
ip link set dev end0 up
echo "nameserver 8.8.8.8" > /etc/resolv.conf
```
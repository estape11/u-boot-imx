# Uboot binaries to support Mender for the TS-7970 board

## Files
- u-boot.imx -> patched bootloader
- fw_env.config -> env address configuration
- fw_printenv -> bootloader env tool

## Install the new uboot
- Copy the `u-boot.imx` file to the root of a SDcard
- Reboot and enter the uboot shell
- Run the following commands
    ```bash
    mmc dev 0
    load mmc 0:1 ${loadaddr} /u-boot.imx
    sf probe
    sf erase 0 0x80000
    sf write ${loadaddr} 0x400 $filesize
    ```
- Restart and enter the uboot shell again
- Run the following commands
    ```bash
    env default a
    saveenv
    ```

## Install the env tools
- Copy the files to the SD card of a working rootfs
- Copy the `fw_printenv` to the following directory
    ```bash
    cp fw_printenv /usr/bin/
    ```
- Create the symlink for the `fw_setenv`
    ```bash
    ln -s /usr/bin/fw_printenv /usr/bin/fw_setenv
    ```
- Copy the configuration
    ```bash
    cp fw_env.config /etc/fw_env.config
    ```

## Test
Test if the `fw_printenv` tool can access the bootloader enviroment by running
```bash
fw_printenv
```
The output should look similar to:
```
altbootcmd=run mender_altbootcmd; run bootcmd
autoload=no
baudrate=115200
bootcmd=if test ${jpsdboot} = 'on';then run sdboot;else run emmcboot;fi;
bootcount=1
bootdelay=0
bootlimit=1
bootlinux=if load ${bootdev} ${bootpart} ${loadaddr} /boot/boot.ub; then echo Booting from custom /bo;
clearenv=if sf probe; then sf erase 0x100000 0x2000;sf erase 0x180000 0x2000;echo restored environmeni
cmdline_append=console=ttymxc0,115200 rootwait ro init=/sbin/init
cpu=dl
disable_giga=1
emmcboot=echo Booting from the eMMC ...; env set bootdev mmc; env set bootpart 1:1; env set rootdev ';
ethact=FEC
ethaddr=00:d0:69:53:c5:09
ethprime=FEC
fdt_high=0xffffffff
fdtaddr=0x18000000
findfdt=if test $rev > 'E'; then if load ${bootdev} ${bootpart} ${fdtaddr} /boot/imx6${cpu}-ts7970-re;
imx_type=ts7970-s-1g-800mhz-i
initrd_addr=0x10800000
initrd_high=0xffffffff
jpsdboot=on
loadaddr=0x12000000
mender_altbootcmd=if test ${mender_boot_part} = 1; then setenv mender_boot_part 2; setenv mender_bootp
mender_boot_kernel_type=bootm
mender_boot_part=1
mender_boot_part_hex=1
mender_check_saveenv_canary=1
mender_dtb_name=imx6dl-ts7970-revf.dtb
mender_kernel_name=uImage
mender_saveenv_canary=1
mender_setup=if test "${mender_saveenv_canary}" != "1"; then setenv mender_saveenv_canary 1; saveenv;i
mender_try_to_recover=if test ${upgrade_available} = 1; then reset; fi
mender_uboot_boot=mmc 0:0
mender_uboot_dev=0
mender_uboot_if=mmc
model=7970
nfsboot=echo Booting from NFS ...;dhcp;nfs ${fdtaddr} ${nfsroot}/boot/imx6${cpu}-ts7970-revf.dtb;nfs ;
preboot=if test ${pushsw} = 'on'; then env set bootdelay -1;run usbprod;else env set bootdelay 0;fi
pushsw=off
reset_cause=10
rev=D
sataboot=echo Booting from SATA ...; env set bootdev sata; env set bootpart 0:1; env set rootdev '/de;
sdboot=echo Booting from the SD card ...; run mender_setup; env set bootdev mmc; env set bootpart 0:1;
stderr=serial
stdin=serial
stdout=serial
uimage=/boot/uImage
upgrade_available=0
usbboot=echo Booting from USB ...; env set bootdev usb; env set bootpart 0:1; env set rootdev '/dev/s;
usbprod=usb start;if usb storage;then echo Checking USB storage for updates;if load usb 0:1 ${loadadd;
ver=U-Boot 2015.04-07974-g8a66219795 (Oct 15 2024 - 12:28:55)
```

Now if you set a new variable, this should be saved in the env memory

```bash
fw_setenv test 1234
fw_printenv | grep test
```

It should output:

```bash
test=1234
```
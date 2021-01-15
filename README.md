# assignment_el
Download this packages:  
```bash
sudo apt-get install gawkwget git diffstat unzip texinfo gcc-multilib build-essential  
sudo apt-get install chrpath socat libsdl1.2-dev xterm ncurses-dev lzop
sudo apt-get install minicom u-boot-tools curl net-tools python2.7 python
```
clone the repository:   
```bash
git clone https://github.com/francescasilvano/assignment_el.git
git clone http://git.yoctoproject.org/git/poky
```
then enter in the repository:   
```
cd poky
git checkout -b dunfell origin/dunfell
```
add new layers to the poky directory.
```
git clone -b dunfell git://git.openembedded.org/meta-openembedded
git clone -b dunfell https://github.com/meta-qt5/meta-qt5
git clone -b dunfell git://git.yoctoproject.org/meta-security
git clone -b dunfell git://git.yoctoproject.org/meta-raspberrypi
```
prepare the building:
```
source oe-init-build-env build_rpi3
cd build_rpi3
```
add in the file conf/local.conf the next line:
```
MACHINE ?= "raspberrypi3"
ENABLE_UART = "1"
EXTRA_IMAGE_FEATURES += "debug-tweaks tools-debug eclipse-debug ssh-server-openssh"
IMAGE_INSTALL_append = " linux-firmware-rpidistro-bcm43455"
IMAGE_FSTYPES = "tar.xz ext3 rpi-sdimg"
```
and then in poky/build_rpi3:
```bash
bitbake core-image-full-cmdline
```
in poky/build_rpi3/conf/local.conf add the next line:   
```
IMAGE_INSTALL_append = " mydriver"
KERNEL_MODULE_AUTOLOAD += "mydriver"
IMAGE_INSTALL_append = " heartbeat"
```
in the repository poky/meta-example/recipes-example add the directory heartbeat and mydriver.    
in the repository poky/build_rpi3/tmp/work/raspberrypi3-poky-linux-gnueabi/mydriver/1.0-r0 add the data.h   
then in poky/build_rpi3 reuse the bitbake core-image minimal:     
```bash
bitbake-layers add-layer ../meta-example
bitbake core-image-full-cmdline
```
insert the microsd and add the image into it:
```
sudo dd if=tmp/deploy/images/raspberrypi3/core-image-full-cmdline-raspberrypi3.rpi-sdimg of=/dev/sdb bs=1M
```
insert the sd in the raspberry and start the booting  
set the address of the raspbarry with ifconfig eth0 <ip>  
set the address of the pc and then connect throw
```
  ssh root@<ip> 
```
enter in the rpi3 by root and start the app:
```
/usr/bin/heartbeat
```


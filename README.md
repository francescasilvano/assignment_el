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
```
then enter in the repository and prepare the build system:   
```
cd poky
git checkout -b dunfell origin/dunfell
source oe-init-build-env build_qemuarm
cd build_qemuarm
bitbake core-image-minimal
```
add in the file conf/local.conf the next line:
```
MACHINE ?= "qemuarm"
```
and then in poky/build_qemuarm:
```bash
bitbake core-image-minimal
```
in poky/build_qemuarm/conf/local.conf add the next line:   
```
IMAGE_INSTALL_append = " mydriver"
KERNEL_MODULE_AUTOLOAD += "mydriver"
IMAGE_INSTALL_append = " heartbeat"
```
in the repository poky/meta-example/recipes-example add the directory heartbeat and mydriver.   
then in poky/build_qemuarm reuse the bitbake core-image minimal:   
```bash
bitbake core-image-minimal
runqemu qemuarm
```
enter in the qemuarm as root and execute the next line:
```
/usr/bin/heartbeat
```


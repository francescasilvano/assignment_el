# assignment_el
clone the repository: 
git clone https://github.com/francescasilvano/assignment_el.git
in the repository of poky:
cd poky
source oe-init-build-env build_qemuarm
add in the file conf/local conf the next three lines:
IMAGE_INSTALL_append = " mydriver"
KERNEL_MODULE_AUTOLOAD += "mydriver"
IMAGE_INSTALL_append = " heartbeat"
in the repository poky/build_qemuarm:
cd build_qemuarm
bitbake core-image-minimal
runqemu qemuarm
/usr/bin/heartbeat



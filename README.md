# distortionpipeline package

### This package contains the Barrel ditortion correction model accelerated in FPGA.

'distortionpipeline' folder contains the OpenCL host code file and a C++ accel file that demonstrate the call of Vitis Vision functions to build for Vitis.
'build' folder inside 'distortionpipeline' folder has the configuration file that would help modify the default configuration of the function.

## Commands to run:

% source < path-to-Vitis-installation-directory >/settings64.sh

% source < part-to-XRT-installation-directory >/setup.sh

% export DEVICE=< path-to-platform-directory >/< platform >.xpfm

## For using in targeted embedded devices:

Download the platform, and common-image from Xilinx Download Center. Run the sdk.sh script from the common-image directory to install sysroot using the command : "./sdk.sh -y -d ./ -p"

Unzip the rootfs file : "gunzip ./rootfs.ext4.gz"

export SYSROOT=< path-to-platform-sysroot >

export PERL=< path-to-perl-installed-location > #For example, "export PERL=/usr/bin/perl". Please make sure that Expect.pm package is available in your Perl installation.

make host xclbin TARGET=< sw_emu|hw_emu|hw > BOARD=Zynq ARCH=< aarch32 | aarch64 >

make run TARGET=< sw_emu|hw_emu|hw >  BOARD=Zynq ARCH=< aarch32 | aarch64 > #This command will only generate the sd_card folder in case of hardware build. Run will not happen.

### For hw run on embedded devices, copy the generated sd_card folder content, under package_hw, to an SDCARD and run the following commands on the board:

% source /opt/xilinx/xrt/setup.sh

% cd /mnt
   
% export XCL_BINDIR=< xclbin-folder-present-in-the-sd_card > #For example, "export XCL_BINDIR=xclbin_zcu102_base_hw"
   
% ./< executable > < arguments >

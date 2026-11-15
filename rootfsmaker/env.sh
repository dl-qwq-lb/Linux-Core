export PROJECT_PATH=/home/liuhenglin/rootfsmaker

export CONFIG_PREFIX=$PROJECT_PATH/rootfs
export SYSROOT=$PROJECT_PATH/rootfs_template
export ARCH=i386
export BINUTILS_PATH=/opt/binutils-2.18/bin

export HOSTCC="gcc-4.1"
export CC="gcc-4.1"

export CFLAGS="-m32 -march=i386 -mtune=i386 -fno-stack-protector -fno-pie --sysroot=$SYSROOT -I$SYSROOT/usr/include"
export LDFLAGS="--sysroot=$SYSROOT -L$SYSROOT/usr/lib -m32 -no-pie"

export AS=$BINUTILS_PATH/as
export LD=$BINUTILS_PATH/ld

export AR=$BINUTILS_PATH/ar
export NM=$BINUTILS_PATH/nm
export OBJCOPY=$BINUTILS_PATH/objcopy
export OBJDUMP=$BINUTILS_PATH/objdump

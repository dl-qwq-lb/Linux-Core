# 编译器
CC = gcc-4.1

# 编译选项
ARCH_FLAGS = -m32
OPT_FLAGS = -O2
#LINK_FLAGS = -static -s
LINK_FLAGS =
CFLAGS = $(ARCH_FLAGS) $(OPT_FLAGS) $(LINK_FLAGS)
LDFLAGS = $(ARCH_FLAGS) $(LINK_FLAGS)

# 内核地址
KERNEL_IMAGE := ../linux-2.6.24/arch/x86/boot/bzImage

# 根文件系统
ROOTFS := initramfs.cpio.gz

# 项目设置
TARGET = init
SRCS = init.c
OBJS = $(SRCS:.c=.o)

# 构建规则
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

init_rootfs: $(TARGET)
	sudo rm rootfs -rf
	sudo cp rootfs_template rootfs -rf
	cp $(TARGET) rootfs
	cd rootfs && find . | cpio -o -H newc | gzip > ../$(ROOTFS)

bb_rootfs:
	sudo rm rootfs -rf
	sudo cp rootfs_template rootfs -rf
	cd busybox-1_5_stable && make install CONFIG_PREFIX=../rootfs
	cd rootfs && find . | cpio -o -H newc | gzip > ../$(ROOTFS)

#运行
init_run: $(ROOTFS)
	qemu-system-i386 -kernel $(KERNEL_IMAGE) -initrd $(ROOTFS) -append "console=ttyS0 rdinit=/init" -m 256M -nographic

bb_run: $(ROOTFS)
	qemu-system-i386 -kernel $(KERNEL_IMAGE) -initrd $(ROOTFS) -append "console=ttyS0 rdinit=/bin/sh" -m 256M -nographic

# 清理
clean:
	rm -f $(TARGET) $(OBJS) $(ROOTFS)

.PHONY: all clean run rootfs

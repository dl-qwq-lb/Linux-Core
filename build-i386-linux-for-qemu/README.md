# 搭建i386linux环境

#### 介绍


#### 步骤
1. 准备Linux开发环境（使用WSL或虚拟机，Linux版本要求20.04）
2. 安装docker。运行脚本  install_docker.sh
3. 部署docker镜像
```shell
sudo docker build -t linux -f dockerfile . --build-arg USERNAME=$USER
```
4. 创建容器

```shell
sudo docker run -itd --name linux -v /home/$USER:/home/$USER linux
```
5. 进入容器
```shell
sudo docker exec -it linux /bin/bash
```
6. 解压linux内核文件到指定位置

7. 配置Linux内核
```shell
make ARCH=i386 defconfig
```

8. 编译Linux内核

```shell
make ARCH=i386 CC="gcc-4.1 -m32" LD="/opt/binutils-2.18/bin/ld -m32" AS="/opt/binutils-2.18/bin/as -march=i386" -j8
```

9. 编译init

```shell
gcc-4.1 -m32 -static -s -o init init.c
```

10. 打包根文件系统

```shell
cd rootfs && find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz
```


11. 启动qemu

```shell
qemu-system-i386 -kernel arch/x86/boot/bzImage -initrd ../initramfs.cpio.gz -append "console=ttyS0 rdinit=/init" -m 256M -nographic
```
qemu的退出方法 ctrl+a   然后输入x


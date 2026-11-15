ls -l
cd ./home
sudo apt update
sudo apt install git vim
git log
git -v
git --version
vim --version
git config --global user.name "Steve Banner"
git config --global user.email "2938117471@qq.com"
git config --global core.editor vim
git clone https://gitee.com/tjuscs_liym/build-i386-linux-for-qemu.git
ls -l
cd ./build-i386-linux-for-qemu/
./install_docker.sh
ls -l
cd ./rootfs/
ls -l
cd ..
sudo apt install docker
ls -l
cd ./build-i386-linux-for-qemu/
ls -l
chmod 744 ./dockerfile 
ls -l
./dockerfile
git remote -v
git pull origin master
ls -l
./install_docker.sh
cd ..
sudo vi /etc/docker/daemon.json
sudo service docker stop
sudo service docker start
sudo nano /etc/wsl.conf
wsl --shutdown
sudo vi /etc/docker/daemon.json
sudo service docker stop
sudo service docker start
/.bashrc
sudo vi ~/.bashrc
sudo service docker stop
sudo service docker start
sudo vi ~/.bashrc
sudo service docker stop
sudo service docker start
sudo vi ~/.bashrc
sudo service docker start
sudo rm -rf /var/lib/docker
cd linux-2.6.24
ls -l
cd build-i386-linux-for-qemu/
ls -l
cd ./linux-2.6.24
make ARCH=i386 defconfig
make ARCH=i386 CC="gcc-4.1 -m32" LD="/opt/binutils-2.18/bin/ld -m32" AS="/opt/binutils-2.18/bin/as -march=i386" -j8
ls arch/x86/boot/
sudo docker exit
docker stop 9c803a
sudo docker stop -it linux
sudo docker stop linux
sudo vi /etc/docker/daemon.json
cd ..
sudo vi /etc/docker/daemon.json
sudo service docker stop
sudo vi /etc/docker/daemon.json
docker --help
sudo service docker stop
sudo service docker start
docker --help
docker --version
cd ./build-i386-linux-for-qemu/
sudo service docker start
sudo docker exec -it linux /bin/bash
sudo docker start linux
sudo docker exec -it linux /bin/bash
sudo vi /etc/docker/daemon.json
sudo docker exec -it linux /bin/bash
sudo service docker start
sudo docker exec -it linux /bin/bash
sudo docker exec -it linux 
qemu-system-i386 -kernel arch/x86/boot/bzImage -initrd ../initramfs.cpio.gz -append "console=ttyS0 rdinit=/init" -m 256M -nographic
cd ./build-i386-linux-for-qemu/
cd ./linux-2.6.24/
qemu-system-i386 -kernel arch/x86/boot/bzImage -initrd ../initramfs.cpio.gz -append "console=ttyS0 rdinit=/init" -m 256M -nographic
sudo docker exec -it linux /bin/bash
cd ./build-i386-linux-for-qemu/linux-2.6.24
sudo docker exec -it linux /bin/bash
ls -l
cd ./rootfsmaker/
ls -l
cd ./busybox-1_5_stable/
ls -l
cd ..
ls -l
cd ./build-i386-linux-for-qemu/
ls- l
ls -l
cd ./linux-2.6.24/
ls -l
cd ..
cd ./rootfsmaker/
ls -l
vi ./env.sh
ls -l
sudo service docker start
sudo docker start linux
sudo docker exec -it linux /bin/bash
sudo service docker start
sudo docker start linux
sudo docker exec -it linux /bin/bash
cd ..
ls -l
cd ./rootfsmaker/
sudo service docker start
sudo docker start linux
sudo docker exec -it linux /bin/bash
cd ..
ls -l
cd ..
cd ./liuhenglin/
cd ./build-i386-linux-for-qemu/
ls -l
cd ..
cd ./rootfsmaker/
sudo service docker start
sudo docker start linux
sudo docker exec -it linux /bin/bash
cd ..
ls -l
cd ./build-i386-linux-for-qemu/
ls -l
cd ./linux-2.6.24/
ls -l
cd ./arch
ls -l
cd ./x86
ls -l
cd ./boot
ls -l
cd /home/liuhenglin/rootfsmaker/
qemu-system-i386 -kernel ../build-i386-linux-for-qemu/linux-2.6.24/arch/x86/boot/bzImage -initrd initramfs.cpio.gz -append "console=ttyS0 rdinit=/bin/sh" -m 256M -nographic
ls -l
vi ./env.sh 
source ~/.env
source ~/.bashrc
cd ./busybox-1_5_stable/
vi ./env.sh 
ls -l
cd ..
ls -l
vi ./env.sh 
sudo server docker start
sudo service docker start
sudo docker start linux
sudo docker exec -it linux /bin/bash
grep -n "NR_syscalls" include/asm-x86/unistd_32.h | tail -1
cd ./build-i386-linux-for-qemu/linux-2.6.24/
grep -n "NR_syscalls" include/asm-x86/unistd_32.h | tail -1
ls -lh arch/x86/boot/bzImage
grep -n 338 arch/x86/kernel/syscall_table.S
grep -n 325 ./arch/x86/kernel/syscall_table.S
grep -n 325 ./arch/x86/kernel/syscall_table_32.S
grep -R "sys_printprocess" -n .
grep UTS_RELEASE include/generated/utsrelease.h
grep UTS_RELEASE ./include/generated/utsrelease.h
find . -name utsrelease.h
grep UTS_RELEASE ./include/linux/utsrelease.h
grep -n sys_printprocess ./arch/x86/kernel/syscall_table_32.S
cd ..
grep CONFIG_IA32_EMULATION ./.config
grep CONFIG_IA32_EMULATION ./linux-2.6.24/.config
grep CONFIG_X86_32 ./linux-2.6.24/.config

# YOLOV3-TINY
**Ứng dụng phát hiện đối tượng bằng mô hình YOLOV3-TINY sử dụng OpenCL**
## Fix bug makefile
Cài đặt tất cả các tool/ thư viện sau:
+ Intel FPGA SDK for OpenCL 18.1 pro edition
+ Thư viện OpenBlas
+ Thư viện OpenCV phiên bản 2.4 hoặc thấp hơn
+ Toolchain gcc

  ### Intel FPGA SDK for OpenCL
  Download [phiên bản pro 18.1](https://www.intel.com/content/www/us/en/software-kit/665444/intel-fpga-sdk-for-opencl-pro-edition-software-version-18-1.html?)
  
  Cài đặt theo hướng dẫn:
  + Giải nén file vừa tải, mở của sổ terminal, cd tới files vừa giải nén, sau đó run the command sudo ./setup.sh.
  + Chọn địa chỉ lưu và các gói cài đặt(ở đây chỉ cài đặt gói support cho board Arria 10)
  + Trước khi call SDK commands, bạn phải set operating system’s environment variables trỏ tới vị trí cài đặt: mở terminal và run lệnh source <quartusroot>/hld/init_opencl.sh.
  + Kiểm tra the installation và environment variables bằng cách kiểm tra kết quả của lệnh aocl version.<br>

  Crack theo hướng dẫn:
    + Bước 1: tải **license.dat** và **Quartus_18.1_Cracker_Linux.sh** từ [thư mục Quartus](https://drive.google.com/drive/folders/1IqREmcEjr6xuZHuFsaeVyMZVEY-ONlog?usp=sharing)
    + Bước 2: Copy Quartus_18.1_Cracker_Linux.sh vào /root/intelFPGA_Pro/18.1/quartus/linux64/, sau đó mở cửa sổ terminal run _./Quartus_18.1_Cracker_Linux.sh_( Sẽ có dòng chữ tàu chào mừng đã crack của các pháp sư trung hoa).
    + Bước 3: Thay XXXXXXXXXXXX trong license.dat bằng network card ID (Chọn License Setup ở dưới  Tools menu trong Quartus Prime,sẽ thấy được NIC ID, copy cái thứ 2).
    + Bước 4: Trong Quartus Prime's Tools menu, chọn License Setup, trỏ tới License file, cuối cùng click OK.
      Note: sau đó bật Quartus thấy chữ tàu và vào License Setup thấy nhiều packet của Altera là thành công!

  ### Thư viện OpenBlas
  Cài đặt thư viện openBlas mới nhất bằng cú pháp sau:
+ $ git clone https://github.com/xianyi/OpenBLAS
+ $ cd OpenBLAS
+ $ make
+ $ make PREFIX=/usr/local install   
  ### Thư viện OpenCV
  Cài đặt thư viện OpenCV phiên bản 2.4 hoặc thấp hơn:
+ $ git clone https://github.com/opencv/opencv.git
+ $ cd opencv
+ $ git checkout 2.4
+ $ mkdir build
+ $ cd build
+ $ cmake ..
+ $ make
+ $ sudo make install
  ### Toolchain gcc
+ sudo apt update
+ sudo apt install build-essential
+ Check version: gcc --version( Thực hiện các bước sau nếu phương bản đã cài đặt không phải phiên bản mới nhất).
+ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
+ sudo apt update
+ sudo apt install gcc-11 g++-11

### Phụ lục bổ sung:
Nếu gặp lỗi: /usr/bin/ld: obj/detector.o: in function `run_detector(int, char**)':
                                  detector.c:(.text+0x43c9): undefined reference to `demoX'<br>
                                  Thêm demoX.o vào OBJ+=
                                  
# Make Linux image 32G
Download essential files here form Build folder(ask Mr. Truong if you don't have permission): https://drive.google.com/drive/folders/11iGXC7THoxfNUEgtJbBwwkZr2vlbAjyC?usp=drive_link
## 1. Build lại rootfs
UNzip and open the folder Build<br>

```

sudo apt install qemu-user-static<br>
chmod +x ch-mount.sh<br>
mkdir rootfs<br>
sudo tar xpf ubuntu-base-18.04.5-base-armhf.tar.gz -C rootfs/<br>
sudo cp /usr/bin/qemu-arm-static rootfs/usr/bin/<br>
```
Mount proc, sys, dev, dev/pts to new fileystem

```
sudo ./ch-mount.sh -m ./rootfs/
```
Now,in chroot environment


#set DNS 8.8.8.8 or 114.114.114.114
```
echo nameserver 8.8.8.8 > /etc/resolv.conf
```
#install minimal packages required for X server and some core utils
```
apt update
apt-get install language-pack-en-base vim sudo ssh net-tools ethtool wireless-tools lxde xfce4-power-manager xinit xorg xserver-xorg-video-fbdev xserver-xorg-input-all network-manager ntpdate iputils-ping lightdm-gtk-greeter alsa-utils mplayer lightdm bash-completion lxtask htop python-gobject-2 python-gtk2 --no-install-recommends
apt-get install synaptic

apt install locales-all tzdata resolvconf  --no-install-recommends

echo "a10soc">/etc/hostname
echo "127.0.0.1 localhost" >> /etc/hosts
echo "127.0.1.1 a10soc" >> /etc/hosts
```
#Now add a user of your choice and include him in suitable groups
```
adduser knat && addgroup knat adm && addgroup knat sudo && addgroup knat audio
```
#set root without password
```
sed -i 's/^root\:\*/root\:/g' /etc/shadow 
```
#modify getty@.service
```
sed -i 's/^ExecStart=-\/sbin\/agetty.*$/ExecStart=-\/sbin\/agetty --noclear %I $TERM/' /lib/systemd/system/getty@.service
```
#set lightdm.conf
```
echo -e "[SeatDefaults]\nautologin-user=root\nautologin-user-timeout=0" > /etc/lightdm/lightdm.conf
```
#set rc.local 
```
echo -e '#!/bin/sh -e\n#\n# rc.local\n#\n#In order to enable or disable this script just change the execution bits\n\nmodprobe altvipfb\nservice lightdm start\nmount -t vfat /dev/mmcblk0p1 /mnt\n\nexit 0' > /etc/rc.local

chmod +x /etc/rc.local

echo -e 'auto lo\niface lo inet loopback\n\nauto eth0\niface eth0 inet dhcp' > /etc/network/interfaces
```
#update DNS automatically,Set ‘timezone’,Make X used by ‘anyuser’
```
dpkg-reconfigure tzdata

dpkg-reconfigure network-manager

dpkg-reconfigure resolvconf

dpkg-reconfigure x11-common
```
#exit chroot environment
```
exit
```
Now,umount proc, sys, dev, dev/pts 
```
sudo bash ./ch-mount.sh -u rootfs/
```
## 2. Sau đó copy các thành phần sau vào thư mục rootfs/root và nhớ cấp quyền sudo chmod -R 755 rootfs/root
	a. opencl_arm
	b. PipeCNN
	c. opencv 3.4.15
# 3. Build sd card image
```
	sudo ./make_sdimage_p3.py -f -P uboot_w_dtb-mkpimage.bin,num=3,format=raw,size=10M,type=A2 -P rootfs/*,num=2,format=ext3,size=14000M -P zImage,socfpga.rbf,socfpga_arria10_socdk_sdmmc.dtb,num=1,format=vfat,size=1000M -s 15200M -n sdimage_a10_cv2.img
```
 #4. Boot image onto sd card

 For window, we use Rufus to boot.
## Đọc hiểu DARKNET

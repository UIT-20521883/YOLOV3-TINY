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




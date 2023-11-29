// DARKNET RESEARCH <br>
#MARKFILE
    GPU=0: Đặt thành 0 nếu bạn không sử dụng GPU để tăng tốc độ tính toán. Đặt thành 1 nếu bạn sử dụng GPU.

    CUDNN=0: Đặt thành 0 nếu bạn không sử dụng thư viện cuDNN (CUDA Deep Neural Network) của NVIDIA để tăng tốc mạnh mẽ cho các mô hình mạng nơ-ron sâu. Đặt thành 1 nếu bạn sử dụng cuDNN.

    OPENCV?=1: Đặt thành 1 để kích hoạt hỗ trợ OpenCV. OpenCV là một thư viện mã nguồn mở cho xử lý hình ảnh và video.

    OPENEXR=0: Đặt thành 0 nếu bạn không cần hỗ trợ OpenEXR. OpenEXR là một định dạng hình ảnh với độ phân giải cao, thường được sử dụng trong đồ họa máy tính.

    DEBUG?=0: Đặt thành 1 để kích hoạt chế độ gỡ lỗi (debug mode) khi biên dịch.

    FPGA?=1: Đặt thành 1 nếu bạn muốn hỗ trợ FPGA (Field-Programmable Gate Array), một loại linh kiện dùng để xây dựng các hệ thống số.

    FPGA_EMU?=0: Đặt thành 1 nếu bạn muốn sử dụng chế độ mô phỏng (emulation mode) cho FPGA.

    VIEW_TIME=0: Đặt thành 1 nếu bạn muốn hiển thị thông tin về thời gian thực hiện của từng lớp trong mô hình.

    FOLDBN=1: Đặt thành 1 để kích hoạt tính năng "folded batch normalization".

    FP32=0: Đặt thành 1 nếu bạn muốn sử dụng định dạng số thực 32-bit (float) thay vì 16-bit.

    BLAS=1: Đặt thành 1 nếu bạn muốn sử dụng thư viện BLAS (Basic Linear Algebra Subprograms) để tăng tốc độ tính toán ma trận.


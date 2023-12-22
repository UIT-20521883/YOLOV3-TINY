# DARKNET FLOW WORK
Sumary of darknet flow work.
         
            time=what_time_is_it_now();
            printf("Loaded in %f seconds.\n",  what_time_is_it_now()-time);
## 1. darknet.c
Khi chúng ta dùng câu lệnh: ./darknet detect cfg/ttt5_224_160.cfg weight/ttt5_224_160_last.weights data/dog.jpg. Thì chương trình sẽ bắt đầu khỏi chạy từ hàm main của
file darknet.c

```
if (0 == strcmp(argv[1], "detect")){
        float thresh = find_float_arg(argc, argv, "-thresh", .24);
        char *filename = (argc > 4) ? argv[4]: 0;
        char *outfile = find_char_arg(argc, argv, "-out", 0);
        int fullscreen = find_arg(argc, argv, "-fullscreen");
        test_detector("cfg/coco.data", argv[2], argv[3], filename, thresh, .5, outfile, fullscreen);
```
Chương trình sẽ gọi tới hàm **test_detector** thuộc file detector.c
## 2. test_detector
Hàm sẽ nhận các đối số truyền vào và bắt đầu load dataconfig, config của mô hình mạng, weights, ... Để tiết kiệm thời gian thì trong bước này, chúng ta cũng đồng thời
khởi động OpenCL luôn.
```
// load anh 
image im = load_image_color(input,0,0);
// resize kich thuoc 
image sized = letterbox_image(im, net.w, net.h);
```
Tiếp theo đưa tấm ảnh vào mô hình mạng để đầu dự đoán.
```
            float *X = sized.data; // lay du lieu buc anh
            time=what_time_is_it_now();
            network_predict(net, X); // dua buc anh va mo hinh vao ham network_predict de bat dau du doan
            printf("%s: Predicted in %f seconds.\n", input, what_time_is_it_now()-time); // in tong thoi gian du doan 
            get_region_boxes(l, im.w, im.h, net.w, net.h, thresh, probs, boxes, masks, 0, 0, hier_thresh, 1); // lay thong tin bounding bõx va xac xuat
            if (nms) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms); // ap dung non-maximum suppression
            draw_detections(im, l.w*l.h*l.n, thresh, boxes, probs, masks, names, alphabet, l.classes); // vẽ bounding box và xác xuất lên ảnh
            if(outfile){
                save_image(im, outfile);
            }
            else{
                save_image(im, "predictions"); // lưu kết quả sau đó dùng hàm im_show để show ảnh kết quả.
```
## 3. network_predict 
Hàm này cơ bản chỉ truyền net vào forward_network để tính toán trên toàn bộ mô hình và trả kết quả về, các tham số khác gán bằng 0 vì không phải quá trinh traning.
```
 float *network_predict(network net, float *input)
{

    net.input = input;
    net.truth = 0;
    net.train = 0;
    net.delta = 0;
    forward_network(net);
    return net.output;
}
```
## 3. forward_network
Đầu tiên, hàm sẽ chuyển đổi weights và bias từ float về dạng half
```
#ifdef OPENEXR
    // printf(" OPENEXR = 1 \n");
    for(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        if(l.type != CONVOLUTIONAL) continue;
        float2half(l.c*l.n*l.size*l.size, l.weights,       1, l.weights_hf,       1);
        float2half(l.batch*l.outputs,     l.biased_output, 1, l.biased_output_hf, 1);
    }
#endif
```
Tiếp theo, ta dùng vòng lặp for để duyệt qua hết tất cả các lớp trong mô hình mang.
```
for(i = 0; i < net.n; ++i){
    net.index = i;
    layer l = net.layers[i];
    // ...
    l.forward(l, net);
    // ...
}

```
Thiết lập net.index bằng chỉ số của lớp hiện tại để sử dụng trong forward pass của lớp đó.
Gọi hàm l.forward(l, net) để thực hiện chạy tiến trình qua lớp hiện tại. 
## 4. forward_convolutional_layer_hf
Chuẩn bị kích thước đầu vào cho phép nhân ma trận gemm, cụ thể là im2row:
```
    // with im2row version for gemm_ntt
    m = out_h*out_w; // height*width of output
    k = l.size*l.size*l.c; // (size of filter)*(size of filter)*(channels of input)
    n = l.n; // channels of input

```
Nếu là layer thứ 0,2 và 7 sẽ thực hiện theo flow sau:
```
if(1 && (net.index==0 || net.index==2 || net.index==7))
        {
            float *a = net.workspace;
            float *b = l.weights;
            float *c = l.output;
            TensorDim in_dim  ={ 1, l.c, l.h, l.w };
            TensorDim filt_dim={ l.out_c, l.c, l.size, l.size };
            CppConvnetIm2Row(a, net.input, out_w, out_h, k, in_dim, filt_dim, l.stride, l.pad);
            //printf("%9.6f ", what_time_is_it_now()-time);
#ifdef CBLAS
           //printf(" 776 \n");
            cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, a, m, b, k, 1, c, m); //OK
#endif

```
Các layer khác sẽ thực hiện theo flow sau:
```
else 
        {          // gemm_ntt_jikK.cl
       // printf(" 794 \n");
            float *a = net.workspace;
            //float *b = l.weights;
            float *c = l.output;
            float *A = (float*)malloc(sizeof(float)*(l.out_w*l.out_h)*(l.size*l.size*l.c));
            half *a_hf = net.workspace_hf;
            half *b_hf = l.weights_hf;
            half *c_hf = l.output_hf;
            TensorDim in_dim  ={ 1, l.c, l.h, l.w };
            TensorDim filt_dim={ l.out_c, l.c, l.size, l.size };
            CppConvnetIm2Row(a, net.input, out_w, out_h, k, in_dim, filt_dim, l.stride, l.pad);
            col2row_cblas(l.c*l.size*l.size, out_w*out_h, a, A);
            float2half(m*k, A, 1, a_hf, 1);
            //printf("%9.6f ", what_time_is_it_now()-time);
            gemm_hf(0,1,1, m, n, k, 1, a_hf, k, b_hf, k, 1, c_hf, m);     //OK for instead of FPGA Model
            half2float(m*n, c_hf, 1, c, 1);
            free(A);
        }

```
## 5. CppConvnetIm2Row() 

## 6. cblas_sgemm

## 7.gemm_hf
    m = out_h*out_w; // height*width of output
    k = l.size*l.size*l.c; // (size of filter)*(size of filter)*(channels of input)
    n = l.n; // channels of input
   ```
    
gemm_hf(     0,      1,      1, 
             m,      n,      k,  1, 
             a_hf, k, 
             b_hf, k, 1, 
             c_hf, m); 
             
gemm_hf(int TA, int TB, int TC,
        int M, int N, int K, float ALPHA, 
        half *A, int lda, 
        half *B, int ldb,
        float BETA,
        half *C, int ldc);
 gemm_ntt_fpga_half(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
void gemm_ntt_fpga_half(int M, int N, int K, float ALPHA, 
        half *A, int lda, 
        half *B, int ldb,
        half *C, int ldc)
    if(!(K%27))
        kernel = kernels[GEMM9W];
    else
        kernel = kernels[GEMMfW];

  
   ```

gemm_ntt_fpga_half





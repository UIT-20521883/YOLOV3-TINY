# DARKNET FLOW WORK
Sumary of darknet flow work.
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

## 4. forward_convolutional_layer_hf

## 5. CppConvnetIm2Row() 

## 6. cblas_sgemm

## 7.gemm_hf
gemm_ntt_fpga_half



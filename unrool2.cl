#pragma OPENCL EXTENSION cl_khr_fp16 : enable
float sum16(float16 a) {
  return a.s0 + a.s1 + a.s2 + a.s3 + a.s4 + a.s5 + a.s6 + a.s7 + a.s8 + a.s9 +
         a.sa + a.sb + a.sc + a.sd + a.se + a.sf;
}
kernel void gemm_nnfW(const int M, const int N, const int K, const float ALPHA,
                      global half *restrict A, const int lda,
                      global half *restrict B, const int ldb,
                      global half *restrict C, const int ldc) {
  int i, j, k;
  int wK = K / 16; // int wK = K/16;
  int wlda = lda / 16;
  float16 B_BUF[4608 / 16];

  for (j = 0; j < N; ++j) {
    // Load B into buffer
    for (k = 0, i = 0; k < wK; ++k)
      B_BUF[i++] = vload_half16((j * wlda + k), B);

    // Unroll the loop to perform multiple computations simultaneously

    for (i = 0; i < M; ++i) {
      float Cn = 0.0f;
      #pragma unroll 16
      for (k = 0; k < wK; k += 16) { // Thực hiện 16 phép tính cùng lúc
        float16 Ax[16];
        float16 Bx[16];
        float16 Cx[16];
        // Load A
        for (int m = 0; m < 16; ++m){
          Ax[m] = vload_half16((i * wlda + k + m), A);
        // Load B
        
          Bx[m] = B_BUF[k + m];

        // Compute C = A * B
        
          Cx[m] = Bx[m] * Ax[m];

        // Accumulate results
        
          Cn += sum16(Cx[m]);
        }
         }
      // Store results
      C[i + ldc * j] += Cn;
    }
  }
}
kernel void gemm_nn9W(const int M, const int N, const int K, const float ALPHA,
                      global half *restrict A, const int lda,
                      global half *restrict B, const int ldb,
                      global half *restrict C, const int ldc) {
  int i, j, k;
  int wK = K / 16; // int wK = K/16;
  int wlda = lda / 16;
  float16 B_BUF[4608 / 16];

  for (j = 0; j < N; ++j) {
    // Load B into buffer
    for (k = 0, i = 0; k < wK; ++k)
      B_BUF[i++] = vload_half16((j * wlda + k), B);

    // Unroll the loop to perform multiple computations simultaneously

    for (i = 0; i < M; ++i) {
      float Cn = 0.0f;
      #pragma unroll 9
      for (k = 0; k < wK; k += 9) { // Thực hiện 16 phép tính cùng lúc
        float16 Ax[9];
        float16 Bx[9];
        float16 Cx[9];
        // Load A
        for (int m = 0; m < 9; ++m){
          Ax[m] = vload_half16((i * wlda + k + m), A);
        // Load B
        
          Bx[m] = B_BUF[k + m];

        // Compute C = A * B
        
          Cx[m] = Bx[m] * Ax[m];

        // Accumulate results
        
          Cn += sum16(Cx[m]);
        }
      }
      // Store results
      C[i + ldc * j] += Cn;
    }
  }
}

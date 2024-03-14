#!/bin/sh
cd unroll1
aoc -report -v gemm.cl -high-effort -o gemm_fpga.aocx
cd ../unroll2
aoc -report -v gemm.cl -high-effort -o gemm_fpga.aocx

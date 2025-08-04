#!/bin/bash

nvcc -c cuda_kernel.cu -o cuda_kernel.o
g++ -std=c++11 -o gpustic_cuda main.cpp utils.cpp cuda_kernel.o -lcuda -lcudart -lcrypto

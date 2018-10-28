#!/bin/bash
g++ `pkg-config --cflags opencv` stream_reader.cc `pkg-config --libs opencv` -lcurl  -o stream_reader;
./stream_reader


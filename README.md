# TODO / Implement

// Remove committed git file
 git filter-branch --force --index-filter ' git rm --cached --ignore-unmatch  test-deck/data/mnist_data/mnist_train.csv' --prune-empty --tag-name-filter cat


https://github.com/Kolkir/mlcpp
https://cocodataset.org/#home
https://github.com/h2oai/deepwater
https://www.h2o.ai/
https://Comet.ml
http://www.teracrunch.com/#solution



https://discuss.mxnet.io/t/run-time-is-different-between-python-and-c/4052/2
https://github.com/h2oai/deepwater/blob/master/mxnet/image_train.cxx
https://github.com/Kolkir/mlcpp/tree/master/rcnn-mxnet
https://beta.mxnet.io/guide/4-train.html

# Logo detection
https://www.oreilly.com/ideas/logo-detection-using-apache-mxnet

# Handwritten recognition
https://github.com/rohun-tripathi/Handwriting_recognition/tree/master/project_documentation
https://github.com/awslabs/handwritten-text-recognition-for-apache-mxnet/blob/master/5_a_character_error_distance.ipynb


# Database of hand written text
http://www.fki.inf.unibe.ch/DBs/iamGraphDB/iLogin/index.php

# Generating Test Data with im2rec
https://github.com/leocvml/mxnet-im2rec_tutorial
https://arthurcaillau.com/image-record-iter/
~/dev/3rdparty/mxnet
```
python im2rec.py /home/gbugaj/dev/lbp-matcher/test-deck/data/rec/query /home/gbugaj/dev/lbp-matcher/test-deck/data/rec --recursive --list --num-thread 8
python im2rec.py /home/gbugaj/dev/lbp-matcher/test-deck/data/rec/query.lst /home/gbugaj/dev/lbp-matcher/test-deck/data/rec
```

# mxnet setup

## Setup up OpenCV
https://milq.github.io/install-opencv-ubuntu-debian/
https://github.com/milq/milq/blob/master/scripts/bash/install-opencv.sh

# ADD Needed dependencies

```
sudo apt-get install -y libopenblas-dev liblapack-dev
sudo apt-get install libblas-dev liblapack-dev
sudo apt install libjemalloc1 libjemalloc-dev
```

# setup 
https://apt.llvm.org/
Need to add the key
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -


https://medium.com/analytics-vidhya/cnns-architectures-lenet-alexnet-vgg-googlenet-resnet-and-more-666091488df5

https://github.com/julitopower/FindMxNet
https://github.com/c3sr/scope/tree/master/cmake/modules


http://34.201.8.176/versions/1.1.0/install/ubuntu_setup.html
https://nextjournal.com/mpd/compiling-mxnet

https://github.com/dmlc/gluon-cv/blob/master/scripts/deployment/cpp-inference/CMakeLists.txt


sudo apt-cache search libgl | grep '^libgl[^a-z]'

libgl1-mesa-dev - free implementation of the OpenGL API -- GLX development files
libgl1-mesa-dri - free implementation of the OpenGL API -- DRI modules


Build without CUDE/OpenCV
https://github.com/RobotLocomotion/drake/issues/2087

$ sudo rm /usr/lib/x86_64-linux-gnu/libGL.so
$ sudo ln -s /usr/lib/libGL.so.1 /usr/lib/x86_64-linux-gnu/libGL.so

WORKING BUILD
cmake -DUSE_CPP_PACKAGE=1  -DUSE_CUDA=0  -DUSE_OPENCV=1 -DCMAKE_BUILD_TYPE=Release -GNinja .. && ninja -j7
 

Build without CUDA support

mkdir -p build
cmake -DUSE_CUDA=0 -DUSE_CUDA_PATH=/usr/local/cuda -DUSE_CUDNN=1 -DUSE_MKLDNN=1 -DUSE_CPP_PACKAGE=1 -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja -v


*** IMPORTANT ***
ERROR : 
make[3]: *** No rule to make target '/usr/lib/x86_64-linux-gnu/libGL.so', needed by 'src/lbp_matcher'.  Stop.

FIX :

sudo rm /usr/lib/x86_64-linux-gnu/libGL.so
sudo ln -s /usr/lib/x86_64-linux-gnu/libGL.so.1  /usr/lib/x86_64-linux-gnu/libGL.so


*** IMPORTANT ** 

ERROR :
/usr/bin/ld: cannot find -lavresample

FIX : 
sudo apt-get install libavresample-dev


ninja -v
[1/23] cd /home/gbugaj/dev/3rdparty/mxnet/cpp-package/scripts && echo Running:\ OpWrapperGenerator.py && python OpWrapperGenerator.py /home/gbugaj/dev/3rdparty/mxnet/build/libmxnet.so
Running: OpWrapperGenerator.py


Build with CUDA ls
 
cmake -DUSE_CUDA=1 -DUSE_CUDA_PATH=/usr/local/cuda -DUSE_CUDNN=1 -DUSE_MKLDNN=1 -DUSE_CPP_PACKAGE=1 -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja -v

sudo apt-get install -y build-essential libatlas-base-dev libopencv-dev graphviz virtualenv cmake ninja-build libopenblas-dev liblapack-dev python3-dev

make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas

 -DUSE_LAPACK=OFF \

```
#!/bin/bash
mkdir -p build \
&& cd build \
&& cmake \
		-DUSE_CPP_PACKAGE=1 \
        -DUSE_SSE=ON \
        -DUSE_CUDA=OFF \
        -DUSE_OPENCV=ON \
        -DUSE_OPENMP=ON \
        -DUSE_MKL_IF_AVAILABLE=OFF \
        -DUSE_SIGNAL_HANDLER=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -GNinja .. \
&& ninja -j7
```

Makefile:362: WARNING: Significant performance increases can be achieved by installing and enabling gperftools or jemalloc development packages
cd /home/gbugaj/dev/3rdparty/mxnet/3rdparty/dmlc-core; make libdmlc.a USE_SSE=1 config=/home/gbugaj/dev/3rdparty/mxnet/make/config.mk; cd /home/gbugaj/dev/3rdparty/mxnet
lsg++ -shared -fPIC example/lib_api/mylib.cc -o libsample_lib.so -I include/mxnet
make[1]: Entering directory '/home/gbugaj/dev/3rdparty/mxnet/3rdparty/dmlc-core'
make[1]: 'libdmlc.a' is up to date.
make[1]: Leaving directory '/home/gbugaj/dev/3rdparty/mxnet/3rdparty/dmlc-core'


https://mxnet.apache.org/api/cpp/docs/tutorials/basics
----------------------------------



VERY GOOD Information
https://pdfs.semanticscholar.org/8e03/d2433f982a51c12c9cbb7dc8502b3d22807b.pdf
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4435149/
https://www.ijcaonline.org/archives/volume153/number10/swaroop-2016-ijca-912165.pdf


# Noise reduction by using a threashold

# References 
https://pdfs.semanticscholar.org/82f9/0f605287e026e00ac5d4ee34af6e3ec8bb91.pdf

Efficient Spatio-Temporal Local Binary Patterns for Spontaneous Facial Micro-Expression Recognition
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4438071/


A CO-TRAINING APPROACH TO AUTOMATIC FACE RECOGNITION
https://www.researchgate.net/figure/LBP-face-recognition_fig2_258936085


* http://www.cse.oulu.fi/wsgi/CMV/Downloads/LBPMatlab
* https://www.researchgate.net/publication/264605316_PersianArabic_Handwritten_Digit_Recognition_Using_Local_Binary_Pattern

Matlab extractLBPFeatures
https://www.mathworks.com/help/vision/ref/extractlbpfeatures.html


http://pi-virtualworld.blogspot.com/2014/03/uniform-lbp-features-and-spatial.html

# Catch2 Testing Framework


# Test cases are build with Catch2 test framework

* https://github.com/catchorg/Catch2
* https://github.com/catchorg/Catch2/issues/882
* https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#where-to-put-it

# Distance measures

* https://dataaspirant.com/2015/04/11/five-most-popular-similarity-measures-implementation-in-python/
* https://stackoverflow.com/questions/6499491/comparing-two-histograms
* http://www.cs.huji.ac.il/~werman/Papers/ECCV2010.pdf

# Some build info

C++17 Required

An official test PPA:

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-7
gcc-7 --version
Not available in 16.10.

It looks like 17.10 will have it by default: https://launchpad.net/ubuntu/+source/gcc-7

GCC 7 was release in May 2017, so too late for 17.04 main release.
=======
http://www.cse.psu.edu/~rtc12/CSE598G/LKintro_6pp.pdf
https://docs.opencv.org/2.4/modules/imgproc/doc/object_detection.html?highlight=matchtemplate#matchtemplate
https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html

Manhatan Distance Instead of Hamming Distance

Profiler setup
=====
https://www.jetbrains.com/help/clion/cpu-profiler.html

# Textons

https://pdf.sciencedirectassets.com/282073/1-s2.0-S2212017312X00070/1-s2.0-S2212017312006147/main.pdf?X-Amz-Security-Token=AgoJb3JpZ2luX2VjEBIaCXVzLWVhc3QtMSJIMEYCIQCTFj7PXlOkVZJVroYe463dtL5otT9OMiqdOzVjZFq3agIhAITdAyJcXU6AF1b78TlOj6zxlBQ0ww5hyCCfC5yslRNhKuMDCJv%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEQAhoMMDU5MDAzNTQ2ODY1IgwPq7z5hu122yk9mOcqtwOcXT7Z01LahEsLKMfgpRyy7g9sHO56VjT7EAjR6APOjJHjXi7k%2BpMFUeSrIOYdH%2Fhn4pFU1t%2B03lawoMTCCofGXIK3qKOtHh1snbYvqRQRq6TPIysOZKpWiNcpqp0d2zBxtjv6TvoOdMjpfIpzD0MXFdcrQIMHv8AaI1r1OO1w6Z49Yd0LD%2Fxu%2BBCkrBeg2iOI9WZ95pQBtzwf%2FLgM%2BTHI1XKOvFuh8U6R49V1Zh2O6pampe6bEHjpRyoxQdW6gF7ioXIR5MhOIcjKqxE9HRZu1r7%2FQBtNO%2Fw5zRxRUhD3DJncKveIEPzq8cHSsE1KND8PJKuk%2FjTEPYrJfmll6W8hKqSx1TwHdiFgDFJTvZbOLlfgc%2BkKIUFEi9%2BmH0ae0JI5xJvL97HqJ1eDeO1yMK0fpjRhgf%2BjcpGe%2B0OCYW2eq%2F728Q2srJ6dk6iCG9778fppjPrxT2Gfr9Lci%2B7N4W6gnDqKfpUtsJjjRkrlwg4rIi%2BoHYyd3NOGmtJ8DkjepVzqlZydAN6wUPRZUPlhH4Vr8sU%2Fb%2BtnHB0GjNWaM3rzkbCczYXpV6W%2Fxv55tP6QqXb7JfZbVjwQMJTAuOoFOrMB8quIWSzFHfE2HLt0VRkzzUfDsyrTNuOC8nww8%2BKKHRDDqKbl0ZBa12UK0TuiMoSEC0W%2BsumJXbTYtvC9UgwioogCW%2FQqqoH%2BWmyHTebXodlxKUmWiGGH0Lue%2BfD95eFzG7cE1iOm58NlIYwUMOysNuq0jgsPatpxH%2FrxgTFSA%2BYyMm8iyia%2BX6ME%2BnyTLUAEZI1IjIsQUW599qgPHvpqqrjqlXM4HvrVCM1St2a8Hdj1o4Y%3D&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20190810T022704Z&X-Amz-SignedHeaders=host&X-Amz-Expires=300&X-Amz-Credential=ASIAQ3PHCVTY3GBI6NRN%2F20190810%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=a04d67635fe738eaae186ca0a16334051bf8dd0523eb75df5d596a92141884ba&hash=a0181550c08769441fb5e2c44327eca5e6bc8f460ff959c22060e8781df8958a&host=68042c943591013ac2b2430a89b270f6af2c76d8dfd086a07176afe7c76c2c61&pii=S2212017312006147&tid=spdf-ddf65a7f-a7df-4435-9150-578e134093b5&sid=a5a8248b24524143226937e-a0202c5d1503gxrqa&type=client


https://www.researchgate.net/figure/Different-texture-primitives-detected-by-the-LBP_fig3_251202290
https://www.researchgate.net/publication/251202290_Texture_analysis_with_local_binary_patterns
http://www.mecs-press.org/ijigsp/ijigsp-v4-n7/IJIGSP-V4-N7-6.pdf 
https://www.researchgate.net/publication/268349505_Texton_Based_Shape_Features_on_Local_Binary_Pattern_for_Age_Classification



FROM nvidia/cuda:12.9.0-devel-ubuntu24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

ARG CUDA_ARCHITECTURES="75;80;86;89;90"
ENV CMAKE_CUDA_ARCHITECTURES=${CUDA_ARCHITECTURES}

ARG BUILD_JOBS=8

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    wget \
    ca-certificates \
    ninja-build \
    pkg-config \
    libeigen3-dev \
    libflann-dev \
    libboost-all-dev \
    libqhull-dev \
    libusb-1.0-0-dev \
    libpng-dev \
    liblapack-dev \
    libblas-dev \
    libopenni-dev \
    libopenni2-dev \
    ccache \
    libvtk9-dev \
    libvtk9-qt-dev \
    qtbase5-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    libxmu-dev \
    libxi-dev \
    && rm -rf /var/lib/apt/lists/*

ENV CUDA_HOME=/usr/local/cuda-12.9
ENV CUDA_PATH=/usr/local/cuda-12.9
ENV PATH=${CUDA_HOME}/bin:${PATH}
ENV LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${CUDA_HOME}/extras/CUPTI/lib64:${LD_LIBRARY_PATH}
ENV LIBRARY_PATH=${CUDA_HOME}/lib64/stubs:${LIBRARY_PATH}

RUN nvcc --version && \
    nvcc --version | grep "release 12.9"

WORKDIR /tmp
RUN git clone --branch cuda_12.9_fix https://github.com/Wiktor-99/pcl.git


WORKDIR /tmp/pcl/build
RUN cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DCMAKE_CUDA_COMPILER=/usr/local/cuda-12.9/bin/nvcc \
    -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-12.9 \
    -DCMAKE_CUDA_ARCHITECTURES=86 \
    -DCUDA_ARCH_BIN=86 \
    -DCUDA_ARCH_PTX= \
    -DWITH_CUDA=ON \
    -DBUILD_GPU=ON \
    -DBUILD_CUDA=ON \
    -DBUILD_gpu_common=ON \
    -DBUILD_gpu_features=ON \
    -DBUILD_gpu_kinfu=OFF \
    -DBUILD_gpu_kinfu_large_scale=OFF \
    -DBUILD_gpu_octree=ON \
    -DBUILD_gpu_people=OFF \
    -DBUILD_gpu_sample_consensus=ON \
    -DBUILD_gpu_segmentation=ON \
    -DBUILD_gpu_surface=ON \
    -DBUILD_gpu_tracking=ON \
    -DBUILD_gpu_utils=ON \
    -DBUILD_common=ON \
    -DBUILD_features=ON \
    -DBUILD_filters=ON \
    -DBUILD_geometry=ON \
    -DBUILD_io=ON \
    -DBUILD_kdtree=ON \
    -DBUILD_keypoints=ON \
    -DBUILD_ml=ON \
    -DBUILD_octree=ON \
    -DBUILD_outofcore=OFF \
    -DBUILD_people=OFF \
    -DBUILD_recognition=ON \
    -DBUILD_registration=ON \
    -DBUILD_sample_consensus=ON \
    -DBUILD_search=ON \
    -DBUILD_segmentation=ON \
    -DBUILD_stereo=ON \
    -DBUILD_surface=ON \
    -DBUILD_tracking=ON \
    -DBUILD_visualization=ON \
    -DBUILD_apps=ON \
    -DBUILD_tools=ON \
    -DBUILD_examples=OFF \
    -DBUILD_global_tests=OFF \
    -DWITH_VTK=ON \
    -DWITH_QT=ON \
    -DWITH_OPENGL=ON \
    -DPCL_ENABLE_SSE=ON \
    -DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG -fPIC" \
    -DCMAKE_CUDA_FLAGS="-O3 --use_fast_math -Xcompiler -fPIC --expt-relaxed-constexpr" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCUDA_PROPAGATE_HOST_FLAGS=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    .. && ninja -j${BUILD_JOBS} && \
    ninja install && \
    ldconfig

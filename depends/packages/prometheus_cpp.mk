package=prometheus_cpp
$(package)_version=0.13.0
$(package)_download_path=https://github.com/jupp0r/prometheus-cpp/releases/download/v$($(package)_version)/
$(package)_file_name=prometheus-cpp-with-submodules.tar.gz
$(package)_sha256_hash=c8e4b479fc3af24dc0a0b4592010f88f72b31620a72bf50a6ef2e8734dba8358

define $(package)_set_vars
  $(package)_cxxflags=-std=c++17 -O -fPIC
  $(package)_cflags=-g -fPIC
  $(package)_cxx=g++
  $(package)_cc=gcc
endef

define $(package)_config_cmds
  $($(package)_cmake)  . -DOVERRIDE_CXX_STANDARD_FLAGS=OFF -DCMAKE_CXX_STANDARD=17 \
   -DCMAKE_CXX_COMPILER=$($(package)_cxx) \
   -DCMAKE_C_COMPILER=$($(package)_cc) \
   -DCMAKE_CXX_FLAGS="$(CXXFLAGS) $($(package)_cxxflags)" \
   -DCMAKE_C_FLAGS="-fPIC -g" \
   -DENABLE_COMPRESSION=OFF -DENABLE_PUSH=OFF -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=OFF
endef

define $(package)_build_cmds
 cmake --build .  --verbose --parallel 4
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef


CXX_ = clang++
CC_ = clang

all: build_dir
all:
	@cd ./build > /dev/null \
	&& CXX=$(CXX_) CC=$(CC_) cmake .. \
	&& make -j 8

clean:
	@rm -rf ./build

build_dir:
	@mkdir -p ./build

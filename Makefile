PHONY: app

CXX_ := clang++
CC_ := clang

app: build_dir
	@cd ./build > /dev/null \
	&& CXX=$(CXX_) CC=$(CC_) cmake .. \
	&& make -j 8 \
	&& cp bmstu_iu7_cg_course ./..


clean:
	@rm -rf ./build ./bmstu_iu7_cg_course

build_dir:
	@mkdir -p ./build

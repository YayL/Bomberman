TOOLCHAIN ?= riscv32-unknown-elf-
BUILD_DIR ?= ./build
ZIP_TMP_DIR ?= /tmp/bomberman
ZIP_OUT_FILE ?= bomberman.zip

.PHONY: default_target build clean cbuild zip

default_target: build

zip:
	rm -rf $(ZIP_TMP_DIR)
	mkdir $(ZIP_TMP_DIR)
	cp -r ./lib ./src ./includes README.md CMakeLists.txt COPYING Makefile $(ZIP_TMP_DIR)
	cd $(ZIP_TMP_DIR)/.. && zip -r $(ZIP_OUT_FILE) bomberman
	mv $(ZIP_TMP_DIR)/../bomberman.zip .

dump:
	$(TOOLCHAIN)objdump -D $(BUILD_DIR)/bomberman.elf > bomberman.elf.txt

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

cbuild: clean
	cd $(BUILD_DIR) && cmake .. && cmake --build .

build:
	$(MAKE) -C ./build

TOOLCHAIN ?= riscv32-unknown-elf-
BUILD_DIR ?= ./build

.PHONY: default_target build clean cbuild

default_target: build

dump:
	$(TOOLCHAIN)objdump -D $(BUILD_DIR)/bomberman.elf > bomberman.elf.txt

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

cbuild: clean
	cd $(BUILD_DIR) && cmake .. && cmake --build .

build:
	$(MAKE) -C ./build

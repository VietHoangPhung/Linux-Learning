
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

list_10:
	sudo lsmod | tail

find:
	sudo lsmod | grep $*

load: all
	sudo insmod $(obj).ko

unload:
	sudo rmmod $(obj)

check:
	sudo dmesg | tail


new-%:
	@echo "Creating new folder: $*"
	@mkdir -p $*
	@cp Makefile $*/Makefile || echo "no template Makefile found in current folder"
	@echo "Folder $* created with a Makefile inside :D"
	@{ echo "obj = $*\nobj-m += $*.o"; cat $*/Makefile; } > $*/Makefile.tmp
	@mv $*/Makefile.tmp $*/Makefile


clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


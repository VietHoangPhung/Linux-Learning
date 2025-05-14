obj-m += module_full.o
module_full-objs := 01start.o 02stop.o

all:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

list:
	@sudo lsmod | tail

find-%:
	@sudo lsmod | grep $* || echo "no module found"

load:
	@sudo insmod $(obj).ko
	@echo "$(obj) loaded"

unload:
	@sudo rmmod $(obj)
	@echo "$(obj) unloaded"

log:
	@sudo dmesg | tail 

log-%:
	@sudo dmesg -l $* | tail

new-%:
	@echo "Creating new folder: $*"
	@mkdir -p $*
	@cp Makefile $*/Makefile || echo "no template Makefile found in current folder"
	@echo "Folder $* created with a Makefile inside :D"
	@{ echo "obj = $*\nobj-m += $*.o"; cat $*/Makefile; } > $*/Makefile.tmp
	@mv $*/Makefile.tmp $*/Makefile


clean: 
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


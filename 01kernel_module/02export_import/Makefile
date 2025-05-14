obj-m += module_num1.o
obj-m += module_num2.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

list:
	sudo lsmod | tail

find-%:
	@sudo lsmod | grep $* || echo "no module found"

log:
	sudo dmesg | tail



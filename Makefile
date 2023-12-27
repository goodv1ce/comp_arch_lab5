ifneq ($(KERNELRELEASE),)

obj-m := hello.o
ccflags-y += -g # debug info
else
KDIR ?= /lib/modules/`uname -r`/build
default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
%.s %.i: %.c # just use make hello.s instead of objdump
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif
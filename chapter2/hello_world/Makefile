
# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.
# ifeq ($(KERNELRELEASE),)

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := hello.o


modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules


clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean






all: spu compiler

spu:
	@echo "������ spu"
	$(MAKE) -C src

compiler:
	@echo "������ compiler"
	$(MAKE) -C compiler
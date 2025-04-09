all: spu compiler

spu:
	@echo "—борка spu"
	$(MAKE) -C src

compiler:
	@echo "—борка compiler"
	$(MAKE) -C compiler
TOP_MODULE = nexys_adder

VC_FILE ?= ../ViDBo.vc
VERILATOR_OPTIONS = -Wno-fatal -Mdir build -y ../

VERILATOR ?= verilator

# Make sim
build/V$(TOP_MODULE): V$(TOP_MODULE).mk
	$(MAKE) -C build $(MAKE_OPTIONS) -f $<
	
# Generate makefile
V$(TOP_MODULE).mk:
	$(VERILATOR) --top-module $(TOP_MODULE) -f $(VC_FILE) $(VERILATOR_OPTIONS)

.PHONY : nexys
nexys:
	python3 -m http.server --directory NexysA7board/

.PHONY : server
server:
	./build/V$(TOP_MODULE)

.PHONY : clean
clean :
	rm -r build

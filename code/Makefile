DIRS=Kconfig2Logic Logic2BDD counter looseVars extractVariables randomconf

all:
	@for i in $(DIRS); do  $(MAKE) -C $$i; done
	cd cudd-3.0.0 && bash myconfigure.sh
	$(MAKE) -C cudd-3.0.0
	sudo $(MAKE) -C cudd-3.0.0 install
clean:
	@for i in $(DIRS); do $(MAKE) -C $$i clean; done
	$(MAKE) -C cudd-3.0.0 clean


export RELVER := 1.0.0

release: psp

prep:
	$(shell bash ./required/gen_makefile.sh)
	mkdir -p BUILD

clean:  prep
	make -C BUILD -f ../required/makefile_psp clean
	rm -rf BUILD
	rm -rf PACK

pack: psp
	mkdir -p PACK/temp/seplugins
	cp BUILD/*.prx PACK/temp/seplugins
	cd PACK/temp && tar cfvz ../-$(RELVER).tar.gz *

psp: prep
	make -C BUILD -f ../required/makefile_psp

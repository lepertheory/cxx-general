all:
	make -C SafeInteger all
	make -C Arbitrary   all

clean:
	make -C SafeInteger clean
	make -C Arbitrary   clean

love:
	@echo "Not war."

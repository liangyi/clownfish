ALLDIRS = clownfish/base tests/base

all:
	@for i in ${ALLDIRS}; \
	do \
		cd $$i; \
		echo "Making $$i"; \
		make || exit -1; \
		cd -; \
		echo "Done"; \
	done

clean:
	@for i in ${ALLDIRS}; \
	do \
		cd $$i; \
		echo "Cleaning $$i"; \
		make clean || exit -1; \
		cd -; \
	done

install:
	@for i in ${ALLDIRS}; \
	do \
		cd $$i; \
		echo "Installing $$i"; \
		make install || exit -1; \
		cd -; \
	done

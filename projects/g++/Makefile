.PHONY: release debug clean clean-release clean-debug install

DIRS:= build build/release build/debug

all: $(DIRS) release


$(DIRS):
	mkdir $@


release: $(DIRS)
	( cd build/release && cmake -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory  && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

debug: $(DIRS)
	( cd build/debug && cmake -DCMAKE_BUILD_TYPE=debug ../.. && $(MAKE) --no-print-directory && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

clean: clean-release clean-debug

clean-release:
	( cd build/release && $(MAKE) --no-print-directory clean )

clean-debug:
	( cd build/debug && $(MAKE) --no-print-directory clean )


#release:
#	( cd build/release && cmake -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory && make  --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

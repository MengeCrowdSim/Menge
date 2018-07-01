.PHONY: release debug clean clean-release clean-debug install test-debug

DIRS:= build build/release build/debug

all: $(DIRS) release

test: $(DIRS) test-debug

$(DIRS):
	mkdir $@


docs:
	cd ../.. && doxygen doc/MengeFull.cfg

release:
	( cd build/release && cmake -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory  && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

debug:
	( cd build/debug && cmake -DCMAKE_BUILD_TYPE=debug ../.. && $(MAKE) --no-print-directory && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

release-clang:
	( cd build/release && cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CC_COMPILER=clang -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory  && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

debug-clang:
	( cd build/debug && cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CC_COMPILER=clang -DCMAKE_BUILD_TYPE=debug ../.. && $(MAKE) --no-print-directory && make --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

clean: clean-release clean-debug

clean-release:
	( cd build/release && $(MAKE) --no-print-directory clean )

clean-debug:
	( cd build/debug && $(MAKE) --no-print-directory clean )

test-debug:
	( cd build/debug && cmake -DCMAKE_BUILD_TYPE=debug ../.. && $(MAKE) --no-print-directory && make --no-print-directory test)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

#release:
#	( cd build/release && cmake -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory && make  --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

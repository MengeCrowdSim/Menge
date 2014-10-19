.PHONY: release debug clean clean-release clean-debug install

DIRS:= ../build ../build/release ../build/debug ../build/release/PedPlugins ../build/debug/PedPlugins

all: $(DIRS) release


$(DIRS):
	mkdir $@


release:
	( cd ../build/release/PedPlugins && cmake -DCMAKE_BUILD_TYPE=release ../../../PedPlugins && $(MAKE) --no-print-directory )
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

debug:
	( cd ../build/debug/PedPlugins && cmake -DCMAKE_BUILD_TYPE=debug ../../../PedPlugins && $(MAKE) --no-print-directory)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

clean: clean-release clean-debug

clean-release:
	( cd ../build/release && $(MAKE) --no-print-directory clean )

clean-debug:
	( cd ../build/debug && $(MAKE) --no-print-directory clean )


#release:
#	( cd build/release && cmake -DCMAKE_BUILD_TYPE=release ../.. && $(MAKE) --no-print-directory && make  --no-print-directory install)
#	ctags -R  --language-force=c++ *.*
#	ctags -eR  --language-force=c++ *.*

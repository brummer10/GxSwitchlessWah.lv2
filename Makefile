
	ifeq ($(shell whoami),root)
	INSTALL_DIR = /usr/lib/lv2
	else 
	INSTALL_DIR = ~/.lv2
	endif

	# check CPU and supported optimization flags
	ifneq ($(shell cat /proc/cpuinfo | grep sse3 ) , )
		SSE_CFLAGS = -msse3 -mfpmath=sse
	else ifneq ($(shell cat /proc/cpuinfo | grep sse2 ) , )
		SSE_CFLAGS = -msse2 -mfpmath=sse
	else ifneq ($(shell cat /proc/cpuinfo | grep sse ) , )
		SSE_CFLAGS = -msse -mfpmath=sse
		else ifneq ($(shell cat /proc/cpuinfo | grep ARM ) , )
		ifneq ($(shell cat /proc/cpuinfo | grep ARMv7 ) , )
			ifneq ($(shell cat /proc/cpuinfo | grep vfpd32 ) , )
				SSE_CFLAGS = -march=armv7 -mfpu=vfpv3 
			else ifneq ($(shell cat /proc/cpuinfo | grep vfpv3 ) , )
				SSE_CFLAGS = -march=armv7 -mfpu=vfpv3
			endif
		else
			ARMCPU = "YES"
		endif
	else
		SSE_CFLAGS =
	endif

	# set bundle name
	NAME = gx_switchless_wah
	BUNDLE = GxSwitchlessWah.lv2
	VER = 0.1
	# set compile flags
	CXXFLAGS += -I. -fPIC -DPIC -O2 -Wall -funroll-loops -ffast-math -fomit-frame-pointer \
	 -fstrength-reduce $(ABI_CXXFLAGS) $(SSE_CFLAGS)
	LDFLAGS += -I. -shared $(ABI_LDFLAGS) -Llibrary -lm 
	# invoke build files
	OBJECTS = $(NAME).cpp
	## output style (bash colours)
	BLUE = "\033[1;34m"
	RED =  "\033[1;31m"
	NONE = "\033[0m"
	# check OS specific stuff
	OS := $(shell echo $$OS)
	UNAME_S := $(shell uname -s)
	# default library extension
	LIB_EXT = so
	# cross compilation (e.g.: PKG_CONFIG_PATH=/usr/local/pkgconfig make CROSS=x86_64-w64-mingw32- mod)
	ifneq (,$(findstring mingw,$(CROSS)))
		# Found
		TARGET = Windows
		STRIP = $(CROSS)strip
		PKGCONFIG = $(CROSS)pkg-config
		CC = $(CROSS)cc
		CXX = $(CROSS)g++
		LD = $(CROSS)ld
	else ifneq (,$(findstring mingw,$(CXX)))
		# Found
		TARGET = Windows
	else
		# Not found
		ifeq ($(UNAME_S), Linux) #LINUX
			TARGET = Linux
		endif
		ifeq ($(OS), Windows_NT) #WINDOWS
			TARGET = Windows
		endif
		ifeq ($(UNAME_S), Darwin) #APPLE
			TARGET = Apple
		endif
	endif
	ifeq ($(TARGET), Linux)
		ABI_CFLAGS = -Wl,-z,nodelete
		ABI_CXXFLAGS = -Wl,-z,relro,-z,now
		ABI_LDFLAGS = -Wl,-z,noexecstack
	endif
	ifeq ($(TARGET), Windows)
		ECHO += -e
		ABI_LDFLAGS = -static -lpthread
		GUI_LIBS = -liconv -lstdc++
		PKGCONFIG_FLAGS = --static
		TTLUPDATE = sed -i '/lv2:binary/ s/\.so/\.dll/ ' $(BUNDLE)/manifest.ttl
		LIB_EXT = dll
	endif

.PHONY : mod all clean install uninstall 

all : clean check $(NAME)
	@mkdir -p ./$(BUNDLE)
	@cp ./*.ttl ./$(BUNDLE)
	@mv ./*.$(LIB_EXT) ./$(BUNDLE)
	$(TTLUPDATE)
	@if [ -f ./$(BUNDLE)/$(NAME).$(LIB_EXT) ]; then echo $(BLUE)"build finish, now run make install"; \
	else echo $(RED)"sorry, build failed"; fi
	@echo $(NONE)

mod : clean $(NAME)
	@mkdir -p ./$(BUNDLE)
	@cp -R ./MOD/* ./$(BUNDLE)
	@mv ./*.$(LIB_EXT) ./$(BUNDLE)
	$(TTLUPDATE)
	@if [ -f ./$(BUNDLE)/$(NAME).$(LIB_EXT) ]; then echo $(BLUE)"build finish, now run make install"; \
	else echo $(RED)"sorry, build failed"; fi
	@echo $(NONE)

check :
ifdef ARMCPU
	@echo $(RED)ARM CPU DEDECTED, please check the optimization flags
	@echo $(NONE)
endif

clean :
	@rm -f $(NAME).$(LIB_EXT)
	@rm -rf ./$(BUNDLE)
	@echo ". ." $(BLUE)", done"$(NONE)

install :
ifneq ("$(wildcard ./$(BUNDLE))","")
	@mkdir -p $(DESTDIR)$(INSTALL_DIR)/$(BUNDLE)
	@cp -r ./$(BUNDLE)/* $(DESTDIR)$(INSTALL_DIR)/$(BUNDLE)
	@echo ". ." $(BLUE)", done"$(NONE)
else
	@echo ". ." $(BLUE)", you must build first"$(NONE)
endif

uninstall :
	@rm -rf $(INSTALL_DIR)/$(BUNDLE)
	@echo ". ." $(BLUE)", done"$(NONE)

$(NAME) :
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME).$(LIB_EXT)

ARTIFACT = Aircraft_Manage
#Build architecture/variant string, possible values: x86, armv7le, etc...
# PLATFORM ?= x86_64
PLATFORM := aarch64le

#Build profile, possible values: release, debug, profile, coverage
BUILD_PROFILE ?= debug


CONFIG_NAME := $(PLATFORM)-$(BUILD_PROFILE)
OUTPUT_DIR = build/$(CONFIG_NAME)
TARGET = $(OUTPUT_DIR)/$(ARTIFACT)


#Compiler definitions

CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CXX)




#User defined include/preprocessor flags and libraries
INCLUDES += -I../ATC_Shared_Files/public




#INCLUDES += -I/path/to/my/lib/include
#INCLUDES += -I../mylib/public




#LIBS += -L/path/to/my/lib/$(PLATFORM)/usr/lib -lmylib
#LIBS += -L../mylib/$(OUTPUT_DIR) -lmylib




#Compiler flags for build profiles
CCFLAGS_release += -O2
CCFLAGS_debug += -g -O0 -fno-builtin
CCFLAGS_coverage += -g -O0 -ftest-coverage -fprofile-arcs -nopipe -Wc,-auxbase-strip,$@
LDFLAGS_coverage += -ftest-coverage -fprofile-arcs
CCFLAGS_profile += -g -O0 -finstrument-functions
LIBS_profile += -lprofilingS




# Generic compiler flags (which include build type flags)
CCFLAGS_all += -Wall -fmessage-length=0 $(CCFLAGS_$(BUILD_PROFILE)) -fPIC
LDFLAGS_all += $(LDFLAGS_$(BUILD_PROFILE))
LIBS_all += $(LIBS_$(BUILD_PROFILE))




# Linking flags for the ATC_Shared_Files library
LIBS += -L../ATC_Shared_Files/build/$(CONFIG_NAME) -lATC_Shared_Files




# Dependency tracking
DEPS = -Wp,-MMD,$(@:%.o=%.d),-MT,$@




#Macro to expand files recursively: parameters $1 -  directory, $2 - extension, i.e. cpp
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))




# Source list (all .c and .cpp files in src)
SRCS = $(call rwildcard, src, c cpp) ../ATC_Shared_Files/src/Logger.cpp ../ATC_Shared_Files/src/Timer.cpp ../ATC_Shared_Files/src/Shared_Memory.cpp








#Object files list
OBJS = $(addprefix $(OUTPUT_DIR)/,$(addsuffix .o, $(basename $(SRCS))))




#Compiling rule
$(OUTPUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<

# Compiling rule for C++ files
$(OUTPUT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<

# Linking rule
$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(LDFLAGS_all) $(LDFLAGS) $(OBJS) $(LIBS_all) $(LIBS)

#  Copy the input file to the build directory
#  copy_input:
# 	@echo "Copying aircraft_input.txt to $(OUTPUT_DIR)/aircraft_input.txt"
# 	cp aircraft_input.txt $(OUTPUT_DIR)/aircraft_input.txt

# Deployment settings
DEPLOY_HOST = 192.168.141.9
DEPLOY_USER = user
DEPLOY_PATH = /dev/shmem/
DEPLOY_PORT = 8000

# Default target: Build
all: $(TARGET)
# all: $(TARGET) copy_input
# 	@echo "Deploying aircraft_input.txt to $(DEPLOY_PATH)/aircraft_input.txt via Qconn on port $(DEPLOY_PORT)"
# 	@qconn -h $(DEPLOY_HOST) -p $(DEPLOY_PORT) -u $(DEPLOY_USER) copy $(OUTPUT_DIR)/aircraft_input.txt $(DEPLOY_PATH)/aircraft_input.txt
# 	# The following SCP command is commented out:
# 	# @scp $(OUTPUT_DIR)/aircraft_input.txt $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_PATH)

# Clean target: Remove local build and remote file using Qconn (with port specified)
clean:
	rm -fr $(OUTPUT_DIR)
# 	@echo "Removing aircraft_input.txt from remote target at $(DEPLOY_PATH) via Qconn on port $(DEPLOY_PORT)"
# 	@qconn -h $(DEPLOY_HOST) -p $(DEPLOY_PORT) -u $(DEPLOY_USER) remove $(DEPLOY_PATH)/aircraft_input.txt
# 	# The following SSH removal command is commented out:
# 	# @ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "rm -f $(DEPLOY_PATH)/aircraft_input.txt"


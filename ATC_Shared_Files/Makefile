ARTIFACT = libATC_Shared_Files.so

# Build architecture/variant string, possible values: x86, armv7le, etc...
PLATFORM ?= x86_64

# Build profile, possible values: release, debug, profile, coverage
BUILD_PROFILE ?= debug

CONFIG_NAME ?= $(PLATFORM)-$(BUILD_PROFILE)
OUTPUT_DIR = build/$(CONFIG_NAME)
TARGET = $(OUTPUT_DIR)/$(ARTIFACT)

# Compiler definitions
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CXX)

# User defined include/preprocessor flags and libraries
INCLUDES += -I$(CURDIR)/../ATC_Shared_Files/public

# Compiler flags for build profiles
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
DEPS = -Wp,-MMD,$(@:%.o=%.d),-MT,$@

# Macro to expand files recursively: parameters $1 - directory, $2 - extension
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

# Source list (all .c and .cpp files in src)
SRCS = $(call rwildcard, src, c cpp)

# Object files list: placed in OUTPUT_DIR with the same basename as source files, but with .o extension.
OBJS = $(addprefix $(OUTPUT_DIR)/,$(addsuffix .o, $(basename $(SRCS))))

# Compiling rule for C files
$(OUTPUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $<

# Compiling rule for C++ files
$(OUTPUT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $<

# Linking rule for shared library
$(TARGET): $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	@echo "Linking shared library: $(LD) -shared -o $(TARGET) $(LDFLAGS_all) $(OBJS) $(LIBS_all)"
	$(LD) -shared -o $(TARGET) $(LDFLAGS_all) $(OBJS) $(LIBS_all)

# Default rule for building the library
all: $(TARGET)

# Clean rule to remove the output directory
clean:
	rm -rf $(OUTPUT_DIR)

# Rebuild rule: clean and then build
rebuild: clean all

# Include dependency files
-include $(OBJS:%.o=%.d)

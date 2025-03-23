ARTIFACT = Computer_System

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
# Add ATC_Shared_Files/public directory for header files
INCLUDES += -I../ATC_Shared_Files/public  # Path to public headers in ATC_Shared_Files

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

# Linking flags for the ATC_Shared_Files library
LIBS += -L../ATC_Shared_Files/build/$(CONFIG_NAME) -lATC_Shared_Files

# Dependency tracking
DEPS = -Wp,-MMD,$(@:%.o=%.d),-MT,$@

# Macro to expand files recursively: parameters $1 - directory, $2 - extension (e.g. cpp)
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

# Source list (all .c and .cpp files in src)
SRCS = $(call rwildcard, src, c cpp) ../ATC_Shared_Files/src/Logger.cpp ../ATC_Shared_Files/src/Timer.cpp ../ATC_Shared_Files/src/Shared_Memory.cpp

# Object files list
OBJS = $(addprefix $(OUTPUT_DIR)/,$(addsuffix .o, $(basename $(SRCS))))

# Compiling rule for C files
$(OUTPUT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<

# Compiling rule for C++ files
$(OUTPUT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $(CCFLAGS) $<

# Linking rule for the executable
$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(LDFLAGS_all) $(LDFLAGS) $(OBJS) $(LIBS_all) $(LIBS)

# Rules section for default compilation and linking
all: $(TARGET)

# Clean rule to remove compiled objects and output
clean:
	rm -fr $(OUTPUT_DIR)

# Rebuild rule to clean and build again
rebuild: clean all

# Inclusion of dependencies (object files to source and includes)
-include $(OBJS:%.o=%.d)

# Environment checks {{{

ifdef VERBOSE
	Q :=
	E := @true 
else
	Q := @
	E := @echo 

# Output coloring {{{

	ifndef NOCOLOR
		COLOR_COMPILE := \x1b[32;1m
		COLOR_LINK    := \x1b[36;1m
		COLOR_DEP     := \x1b[33;1m
		COLOR_LOCALE  := \x1b[34;1m
		COLOR_TOOL    := \x1b[35;1m
		COLOR_RESET   := \x1b[0m
		E := @/bin/echo -e 
	endif

# }}}

endif

# }}}
# Package metadata {{{

PACKAGE_NAME    := generals
PACKAGE_VERSION := $(shell git describe --always --dirty)

ifndef PACKAGE_VERSION
	PACKAGE_VERSION := 9999
endif

# }}}
# Directories {{{

SRCDIR := src
OBJDIR := obj
BINDIR := bin
DEPDIR := dep

# }}}
# Output files {{{

BINFILE := $(BINDIR)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)

# }}}
# Toolkit selection {{{

CXX := clang++

# }}}
# Compiler flags {{{

CXXWARNINGS := \
	-Wall \
	-Wextra \
	-Wpointer-arith \
	-Wwrite-strings \
	-Wmissing-declarations \
	-Wno-long-long \
	-Wuninitialized \
	-Wno-unused-parameter

LDLIBS := \
	GL \
	GLEW \
	glfw \
	pthread

CXXINCLUDES := \
	$(SRCDIR)

COMMONFLAGS := -Wfatal-errors
CXXFLAGS    := -std=c++0x
LDFLAGS     := $(addprefix -l, $(LDLIBS))

ifdef RELEASE
	COMMONFLAGS := -g0 -O2 $(COMMONFLAGS)
	CXXFLAGS    := -D_RELEASE $(CXXFLAGS)
else
	COMMONFLAGS := -g3 -O0 $(COMMONFLAGS)
endif

CXXFLAGS := $(COMMONFLAGS) $(CXXWARNINGS) $(CXXFLAGS) $(addprefix -I, $(CXXINCLUDES))

# }}}
# Code input and output files {{{

CXXFILES := $(shell find $(SRCDIR) -name *.cc)
DEPFILES := $(CXXFILES:$(SRCDIR)/%.cc=$(DEPDIR)/%.d)
OBJNAMES := $(CXXFILES:$(SRCDIR)/%.cc=%)
OBJFILES := $(OBJNAMES:%=$(OBJDIR)/%.o)

# }}}

-include Makefile.local

# Make targets {{{

.PHONY: all test dep doc clean

# Phony {{{

all: $(BINFILE)

test: $(BINFILE)
	./$(BINFILE)

dep: $(DEPFILES)

doc: $(CXXFILES) doxygen.conf
	$(Q)doxygen doxygen.conf 1>/dev/null

clean:
	$(Q)rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR) $(DEPDIR)

# Include generated dependencies if we're compiling or linking
ifeq (,$(filter $(MAKECMDGOALS),dep doc clean))
-include $(CXXFILES:$(SRCDIR)/%.cc=$(DEPDIR)/%.d)
endif

# }}}
# Output files {{{

$(BINFILE): $(OBJFILES)
	$(E)" [$(COLOR_LINK)LD $(COLOR_RESET)] $@"
	$(Q)mkdir -p `dirname $@`
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJFILES)
	$(Q)ln -sf $(PACKAGE_NAME)-$(PACKAGE_VERSION) $(BINDIR)/$(PACKAGE_NAME)

# }}}
# Dependency generation {{{

$(DEPDIR)/%.d: $(SRCDIR)/%.cc
	$(E)" [$(COLOR_DEP)DEP$(COLOR_RESET)] $@"
	$(Q)mkdir -p `dirname $@`
	$(Q)$(CXX) $(CXXFLAGS) -MM -MT $(<:$(SRCDIR)/%.cc=$(OBJDIR)/%.o) -o $@ $<

# }}}
# Code compilation {{{

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(E)" [$(COLOR_COMPILE)CXX$(COLOR_RESET)] $<"
	$(Q)mkdir -p `dirname $@`
	$(Q)$(CXX) -o $@ -c $< $(CXXFLAGS)

# }}}
# }}}

.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
AR      = ar
######################################
# Project Name (generate executable with this name)
TARGET = cs296_19_exe
LIBTARGET = cs296_19_exelib

# Project Paths
PROJECT_ROOT=./
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/myobjs
BINDIR = $(PROJECT_ROOT)/mybins
DOCDIR = $(PROJECT_ROOT)/doc
LIBDIR = $(PROJECT_ROOT)/mylibs
DATADIR = $(PROJECT_ROOT)/data
SCRIPTSDIR = $(PROJECT_ROOT)/scripts
PLOTSDIR = $(PROJECT_ROOT)/plots

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr/include/

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL

# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall -fno-strict-aliasing
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib

######################################
SHARED_LIB = FALSE

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LIBOBJS := $(OBJS:$(OBJDIR)/main.o=)

.PHONY: all setup doc clean distclean exe exelib data plot

all: setup

setup:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(BINDIR)
	@if (! test -d $(EXTERNAL_ROOT)/include/Box2D) || (! test -e $(EXTERNAL_ROOT)/lib/libBox2D.a); then \
	cd $(EXTERNAL_ROOT)/src; \
	tar xzf Box2D.tgz; \
	mkdir -p Box2D/build296; \
	cd Box2D/build296; \
	cmake ../; \
	make; \
	make install; \
	fi;

exe: setup $(OBJS)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

$(LIBDIR)/libCS296test.a: $(OBJS)
	@if [ "$(SHARED_LIB)" == "FALSE" ]; then \
	$(PRINTF) "$(MESG_COLOR)Building static library:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"; \
	$(AR) -cvr $@ $(LIBOBJS); \
	$(ECHO) -n "Static Library Built"; \
	fi;

$(LIBDIR)/libCS296test.so: $(OBJS)
	@if [ "$(SHARED_LIB)" != "FALSE" ]; then \
	$(PRINTF) "$(MESG_COLOR)Building dynamic library:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"; \
	$(CC) -shared -o $@ $(LIBOBJS); \
	$(ECHO) -n "Dynamic Library Built"; \
	fi;

exelib: setup $(LIBDIR)/libCS296test.a $(LIBDIR)/libCS296test.so
	@$(ECHO) -n "Linking Library and Building Executable... "
	@if [ "$(SHARED_LIB)" == "FALSE" ]; then \
	$(CC) -o $(BINDIR)/$(LIBTARGET) $(LDFLAGS) -L $(LIBDIR) $(OBJDIR)/main.o -l:libCS296test.a $(LIBS); else \
	$(CC) -o $(BINDIR)/$(LIBTARGET) $(LDFLAGS) -L $(LIBDIR) $(OBJDIR)/main.o -l:libCS296test.so $(LIBS); \
	fi;
	@$(ECHO) -n "Executable Successfully Built!"

-include $(OBJS:.o=.d)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	$(CC) -fPIC $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"

clean:
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR) *~ $(DEPS) $(SRCDIR)/*~ $(BINDIR) $(LIBDIR) $(PLOTSDIR) $(SCRIPTSDIR)/*.log $(SCRIPTSDIR)/*~
	@cd $(DATADIR); ls | grep -v pnp_austen.txt | xargs rm -rf
	@$(ECHO) "Done"

distclean: clean
	@$(ECHO) -n "Removing Box2D Install..."
	@$(RM) -rf $(BOX2D_ROOT)/include/* $(BOX2D_ROOT)/lib/* $(BOX2D_ROOT)/src/Box2D
	@$(ECHO) "Box2D Removed"

report: 
	@cd doc; \
	pdflatex cs296_report_19.tex; \
	pdflatex cs296_report_19.tex; \
	bibtex cs296_report_19; \
	pdflatex cs296_report_19.tex; \
	pdflatex cs296_report_19.tex;

data:
	@mkdir -p $(DATADIR)
	@cd $(SCRIPTSDIR); \
	./g19_gen_data.sh; \
	./g19_gen_csv.sh; \
	./g19_gen_data_csv.sh; \
	./g19_gen_data_random.sh;

plot:
	@mkdir -p $(PLOTSDIR)
	@cd $(SCRIPTSDIR); \
	./g19_gen_plot_data.sh; \
	./g19_gen_plot_data_random.sh; \
	./g19_gen_plot_data_02.sh; \
	sed -n '12751,12900p;12900q' ../$(DATADIR)/g19_lab05data_02.csv > ../$(DATADIR)/g19_lab05data_86.csv; \
	./g19_plot01.gpt; \
	mv g19_plot01.png ../$(PLOTSDIR)/ ;\
	./g19_plot02.gpt; \
	mv g19_plot02.png ../$(PLOTSDIR)/ ;\
	./g19_plot03.gpt; \
	mv g19_plot03.png ../$(PLOTSDIR)/ ;\
	./g19_plot04.gpt; \
	mv g19_plot04.png ../$(PLOTSDIR)/ ;\
	./g19_plot05.gpt; \
	mv g19_plot05.png ../$(PLOTSDIR)/ ;\
	$(RM) -rf $(DATADIR)/*plot*.csv;


# begin_generated_IBM_copyright_prolog                             
#                                                                  
# This is an automatically generated copyright prolog.             
# After initializing,  DO NOT MODIFY OR MOVE                       
# ================================================================ 
#                                                                  
# Licensed Materials - Property of IBM                             
#                                                                  
# Blue Gene/Q                                                      
#                                                                  
# (C) Copyright IBM Corp.  2010, 2012                              
#                                                                  
# US Government Users Restricted Rights -                          
# Use, duplication or disclosure restricted                        
# by GSA ADP Schedule Contract with IBM Corp.                      
#                                                                  
# This software is available to you under the                      
# Eclipse Public License (EPL).                                    
#                                                                  
# ================================================================ 
#                                                                  
# end_generated_IBM_copyright_prolog                               
#
# Make rules to install include files....
#

include ../../../Make.rules

$(eval $(call bgq_header_make, spi/include/mu))

OS	= $(shell uname)


DIRS = $(subst /Makefile,,$(wildcard */Makefile))

TARGETS_ALL		= $(addsuffix .all,$(DIRS))
TARGETS_INSTALL		= $(addsuffix .install,$(DIRS))
TARGETS_CLEAN		= $(addsuffix .clean,$(DIRS))
TARGETS_DISTCLEAN	= $(addsuffix .distclean,$(DIRS))

TARGETS = $(TARGETS_ALL)       \
	  $(TARGETS_INSTALL)   \
	  $(TARGETS_CLEAN)     \
	  $(TARGETS_DISTCLEAN)



# +++
#
# ---

.SUFFIXES:

.PHONY: default all install clean dist_clean $(TARGETS)

default:	install

# make the all also do an install so the top level default make works..
all: install

# +++
#
# ---

#all:		this.all	$(TARGETS_ALL)
install:	this.install	$(TARGETS_INSTALL) $(TARGET_HDRS)
clean:		this.clean	$(TARGETS_CLEAN)
distclean:	this.distclean	$(TARGETS_DISTCLEAN)



# +++
#	Each element in "$TARGETS" is of the form:
#		subdirectory.target
#	"$(basename $@)" picks off "subdirectory"
#	"$(subst .,,$(suffix $@))" picks off "target"
# ---

$(TARGETS):
	$(MAKE) --directory $(basename $@) $(subst .,,$(suffix $@))



# +++
#	Targets to be executed in the current directory
# ---

this.all:
	@echo "making all"

this.install:
	@echo "making install"

this.clean:
	@echo "making clean"
	$(cleancmd)
	rm -f *~

this.distclean: this.clean
	@echo "making distclean"



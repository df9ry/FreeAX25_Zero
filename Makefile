#   Project FreeAX25_Config
#   Copyright (C) 2015  tania@df9ry.de
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else
#----- End Boilerplate

VPATH = $(SRCDIR)

CXXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC \
			-fmessage-length=0 -fexceptions -pthread \
			-I$(SRCDIR)/../libFreeAX25Runtime \
			-I$(SRCDIR)/../libJsonX

LDXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC \
			-fmessage-length=0 -fexceptions -pthread \
			-L../../libFreeAX25Runtime/_$(_CONF) \
			-L../../libJsonX/_$(_CONF)

OBJS     =  PluginImpl.o \
			InstanceImpl.o \
			SessionImpl.o
			
LIBS     =  -lFreeAX25Runtime -lJsonX

TARGET   =	FreeAX25_Zero.so

INSTDIR  = ../../FreeAX25/_$(_CONF)

$(TARGET):	$(OBJS)
	$(CXX) $(LDXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<	
	
all: $(TARGET)
	+@[ -d $(INSTDIR) ] || mkdir -p $(INSTDIR)
	cp ../*.xml $(INSTDIR)
	cp $(TARGET) $(INSTDIR)
	echo "Build OK"
	
#----- Begin Boilerplate
endif

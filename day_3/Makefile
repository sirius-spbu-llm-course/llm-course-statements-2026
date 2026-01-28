CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

EXE = stego
SRCDIR = src
BINDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRCS))

all: $(EXE)

$(EXE): $(BINDIR) $(OBJECTS)
#	Если у вас Linux/Mac - скорее всего придется убрать -static
	$(CXX) $(OBJECTS) -o $(EXE) -static
  
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(BINDIR)/*.d)

$(BINDIR):
	mkdir $(BINDIR)

clean:
	rm -rf $(BINDIR) $(EXE)

.PHONY: clean all
# Définition des variables
CXX = /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

SOURCES = main.cpp oiseauxclass.cpp
HEADERS = oiseauxclass.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = mon_programme
ifeq ($(BR2_PACKAGE_CRYPTOXX),y)
LIBS += -lcryptopp
endif
ifeq ($(BR2_PACKAGE_LIBCURL),y)
LIBS += -lcurl
endif

# Règles de construction
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o: $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)




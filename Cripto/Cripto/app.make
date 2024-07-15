CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lgdi32

SRC = main.cpp AES.cpp SHA256.cpp RSA.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(TARGET)
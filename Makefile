
CXXFLAGS += -std=c++20
CXXFLAGS += -Wall -Wextra -Wpedantic -Wstack-usage=1024 -Wwrite-strings -Wfloat-equal -Winline
#CXXFLAGS += -Werror

TARGET=Cubish

OBJS=$(TARGET).o Color.o ColorPick.o Cube.o Dimension.o Face.o OptionWindow.o Output.o Quaternion.o Tile.o Vector.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S), Linux)
# Linux flags
LDLIBS+=-lstdc++ -lm -lGL -lGLU -lglut
else
# Mac flags
CXXFLAGS+=-Wno-deprecated -Wno-unknown-warning-option
LDLIBS+=-framework OpenGL -framework GLUT -lstdc++
endif

.PHONY: clean debug

$(TARGET): $(OBJS)

debug: CXXFLAGS += -g
debug: $(TARGET)


clean:
	$(RM) $(TARGET) $(OBJS)

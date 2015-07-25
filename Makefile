
CXXFLAGS+=-g -std=c++1y -Wall -Werror -Wextra -Wstack-usage=1024 -pedantic

OBJS=Cubish.o Color.o ColorPick.o Cube.o Dimension.o Face.o OptionWindow.o Output.o Quaternion.o Tile.o Vector.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S), Linux)
# Linux flags
LDLIBS+=-lstdc++ -lm -lGL -lGLU -lglut
else
# Mac flags
CXXFLAGS+=-Wno-deprecated -Wno-unknown-warning-option
LDLIBS+=-framework OpenGL -framework GLUT -lstdc++
endif

Cubish: $(OBJS)


clean:
	-rm *.o Cubish

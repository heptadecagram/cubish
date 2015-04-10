
CC=c++

CXXFLAGS=-g -std=c++1y -Wall -Werror -pedantic -Wno-deprecated

OBJS=Color.o ColorPick.o Cube.o Dimension.o Face.o Main.o OptionWindow.o Output.o Quaternion.o Tile.o Vector.o

LIBS=-framework OpenGL -framework GLUT

Cubish: $(OBJS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LIBS)


clean:
	@rm *.o Cubish

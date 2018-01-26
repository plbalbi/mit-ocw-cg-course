INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I /usr/include/vecmath

LINKFLAGS  = -framework OpenGL -framework GLUT -framework Cocoa
LINKFLAGS += -lvecmath

CFLAGS    = -O3 -Wall
CC        = g++
SRCS      = main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)


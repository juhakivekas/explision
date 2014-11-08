CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-Wall -lm

DEPS=

bin/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
main.exe: \
	bin/struct/model.o \
	bin/struct/shape.o \
	bin/struct/vector.o \
	bin/struct/transform.o \
	bin/io/ply_io.o \
	bin/io/svg_io.o \
	bin/module/make_shapes.o \
	bin/module/make_connectors.o \
	bin/module/pack_shapes.o \
	bin/module/draw_shapes.o \
	bin/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f bin/*.o bin/*/*.o


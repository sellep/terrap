CC=@gcc
CFLAGS=-Wall -v -march=haswell -fomit-frame-pointer -O3 -pipe

OBJ=terra_conf_read_glob_impl.o \
	terra_conf_read_glob.o \
	terra_conf_print.o

%.o : src/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/conf/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

all: clean $(OBJ)
	$(CC) $(CFLAGS) -o bin/terra src/terra.c $(addprefix obj/ $(OBJ))

clean:
	@mkdir -p bin obj
	@rm -rf bin/*
	@rm -rf obj/*
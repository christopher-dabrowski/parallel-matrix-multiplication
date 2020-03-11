CC=clang
DebugFlags=-std=c11 -Wall -g3 -pedantic
ReleseFlags=-Ofast -std=c11 -g0 -Wno-everything

## Here add names of modules (.h, .c) files that should be compiled (with .o extension)
# It is assumed that each one has .h and .c file
Objects= 

NAME=matrixOperations

release: CompileFlags=$(ReleseFlags)
release: $(Objects)
	$(CC) $(CompileFlags) -o $(NAME) $(Objects) main.c

rebuild: clean release

debug: CompileFlags=$(DebugFlags) -D DEBUG
debug: $(Objects)
	$(CC) $(CompileFlags) -o $(NAME) $(Objects) main.c

clean:
	rm -rf *.dSYM
	rm -f *.o

%.o : %.c %.h
	$(CC) $(CompileFlags) -c $< -o $@

# % - dowolne znaki. Jak już zadanie zostanie uruchomione to nie zmienia wargości
# $< - pierwsze wymaganie
# $@ - cała nazwa celu
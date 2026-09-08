# Nombre del programa final
PROGRAMA := scanner2

# Compilador y flags
CC := gcc
CFLAGS := -Wall

# Regla principal
all: $(PROGRAMA)

# Regla para generar el ejecutable usando flex y gcc
# TODO 1 y 2 resueltos acá abajo:
$(PROGRAMA): scanner2.l
	flex scanner2.l
	$(CC) $(CFLAGS) lex.yy.c -o $(PROGRAMA)

# Regla para limpiar los archivos generados
# TODO 3 resuelto acá abajo:
clean:
	rm -f $(PROGRAMA) lex.yy.c

.PHONY: all clean
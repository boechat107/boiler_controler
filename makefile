CC      = gcc
CFLAGS  = -Wall -std=gnu89 -pedantic -pthread

OBJ = 		controle.o \
		comunicacao_udp.o \
		monitor_concorrencia.o \
		tarefas.o \
		

all 	: $(OBJ)
	$(CC) $(CFLAGS) -o caldeira $(OBJ)


run	:
	./caldeira

mem	:
	valgrind --leak-check=yes ./caldeira


clean	:
	clear
	rm -f  ./*~ ./*.o caldeira

CFLAGS = -Wall -g
OBJS   =  hinky.o 

hangman : $(OBJS)
	g++ -g -o hinky $(OBJS)
clean:
	@rm -f hinky.o hinky *~
	@echo Clean done

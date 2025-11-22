# Nome dell'eseguibile
NAME = main

# File sorgenti
SRCS = main.c utils.c

# File oggetto
OBJS = $(SRCS:.c=.o)

# Compilatore
CC = cc

# Flag di compilazione
CFLAGS = -Wall -Wextra -Werror

# Librerie da linkare
LIBS = -lsqlite3

# Regola principale
all: $(NAME)

# Link finale
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Regola generica per compilare .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pulisce i .o
clean:
	rm -f $(OBJS)

# Pulisce .o e binario
fclean: clean
	rm -f $(NAME)

# Ricompila da zero
re: fclean all

.PHONY: all clean fclean re
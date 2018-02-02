NAME = ft_ls

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

RM = rm -Rf

SRC = *.c

INC = *.o

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(SRC) $(INC) -o $(NAME)

clean:
	$(RM) $(NAME)

re: clean all
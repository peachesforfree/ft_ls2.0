NAME = ft_ls

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

RM = rm -Rf

SRC =	main.c \
		command_parse.c \
		assemble_first_queue.c \
		print_queue.c

INC =	*.h \
		libft/libft.a

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(SRC) $(INC)

clean:
	$(RM) $(NAME)

re: clean all
NAME = ft_ls

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

RM = rm -Rf

SRC =	main.c

INC =	ft_ls.h \
		libft/libft.a

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(SRC) $(INC)

clean:
	$(RM) $(NAME)

re: clean all
	make -C libft
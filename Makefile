NAME = ft_ls

CC = gcc

FLAGS = -Wall -Werror -Wextra

RM = rm -Rf

SRC =	main.c \
		flags.c \
		new_item.c \
		alpha_sort.c \
		time_sort.c \
		print_long.c \
		print_files.c \
		linked_list_func.c \
		start_queue.c

INC =	ft_ls.h \
		libft/libft.a \
		ft_printf/libftprintf.a

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) -g $(SRC) $(INC)

clean:
	$(RM) $(NAME)

re: clean all
	make -C libft
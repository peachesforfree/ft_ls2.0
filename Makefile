NAME = ft_ls
CFLAGS = -Wall -Wextra -Werror -g
SRCS =	main \
		flags \
		#alpha_sort \
		#linked_list_func \
		#new_item \
		#print_files \
		#print_long \
		#start_queue \
		#time_sort

SRC_DIR = sources/
OBJ_DIR = obj/
INC_DIR = includes/
LIBFT_DIR = libft/
PRINTF = ft_printf/
PRINTF_LIB = $(addprefix $(PRINTF), libftprintf.a)
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
LIBFT = $(addprefix $(LIBFT_DIR), libft.a)
OBJ_FILES = $(SRCS:%=%.o)
SRC_FILES = $(SRCS:%=%.c)

all: $(OBJ_DIR) $(LIBFT) $(PRINTF_LIB) $(NAME)

$(LIBFT):
	mkdir -p $(OBJ_DIR)
	make -C libft/

$(OBJ_DIR):
	mkdir -p obj

$(PRINTF_LIB):
	make -C ft_printf/

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
	gcc -g -Wall -Werror -Wextra -I $(INC_DIR) -c $< -o $@

$(NAME): $(OBJ)
	gcc -g $(OBJ) $(LIBFT) $(PRINTF_LIB) -lm -o $(NAME)

clean:
	rm -Rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean 
	make -C $(PRINTF) clean 

fclean: clean
	rm -Rf $(NAME)
	rm -Rf $(LIBFT)
	rm -Rf $(PRINTF_LIB)

re: fclean all

.PHONY: clean fclean all re
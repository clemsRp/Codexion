NAME = codexion

SRC_DIR = coders/srcs
INC_DIR = coders/includes
OBJ_DIR = .obj

FILE =	codexion.c \
		init.c \
		moves.c \
		validation.c \
		routine.c \
		free.c \
		get.c \
		utils.c \
		enfile.c

SRC = $(addprefix $(SRC_DIR)/, $(FILE))
OBJ = $(addprefix $(OBJ_DIR)/, $(FILE:.c=.o))
DEP = $(addprefix $(OBJ_DIR)/, $(FILE:.c=.d))

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -MMD -I$(INC_DIR)

all: $(NAME) $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re

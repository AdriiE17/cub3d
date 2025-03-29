#**************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 19:37:02 by victor            #+#    #+#              #
#    Updated: 2025/03/29 13:52:36 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
NAME_BONUS = cub3D_bonus

LIBFT = libft/libft.a
MINI = MLX42/build/libmlx42.a

OBJ_DIR = ./obj
SRC_DIR = ./src
SRC_DIR_BONUS = ./src/bonus

# Lista de archivos fuente
SRC = $(SRC_DIR)/cub3D.c $(SRC_DIR)/camera.c $(SRC_DIR)/collition.c $(SRC_DIR)/parse.c \
	  $(SRC_DIR)/render.c $(SRC_DIR)/cub3D_utils.c	$(SRC_DIR)/validate_map.c \
	  $(SRC_DIR)/parse_utils.c $(SRC_DIR)/render_utils.c $(SRC_DIR)/raycasting.c

BONUS_SRC = $(SRC_DIR_BONUS)/cub3D_bonus.c

# Lista de archivos objeto y dependencias
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ = $(BONUS_SRC:$(SRC_DIR_BONUS)/%.c=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)
BONUS_DEP = $(BONUS_OBJ:.o=.d)

INCLUDE = -Iinc -Ilibft -IMLX42/include/MLX42
BONUS_INCLUDE = -Iinc/bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -MMD -MP $(INCLUDE)
LDFLAGS = $(LIBFT) $(MINI) -lglfw -lm

HBONUS = .bonus

all: prepare_libft prepare_mlx libs mlx $(NAME)

# Colores para mensajes
GREEN_DARK = \033[0;32m
WHITE = \033[0m
RED = \033[0;91m
RESET = \033[0m

# Clonar y preparar libft
prepare_libft:
	@if [ ! -d "libft/.git" ]; then \
		echo "$(WHITE)Cloning libft...$(RESET)"; \
		git clone git@github.com:victorberdugo1/libft.git libft; \
	fi

# Clonar y preparar MLX42
prepare_mlx:
	@if [ ! -d "MLX42/.git" ]; then \
		echo "$(WHITE)Cloning MLX42...$(RESET)"; \
		git clone https://github.com/codam-coding-college/MLX42.git MLX42; \
	fi

libs: prepare_libft
	@make -C libft

mlx: prepare_mlx
	@cmake MLX42 -B MLX42/build && make -C MLX42/build -j4

$(NAME): $(LIBFT) $(MINI) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN_DARK)Executable $(NAME) created successfully!$(RESET)"

$(NAME_BONUS): $(LIBFT) $(MINI) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_INCLUDE) $(BONUS_OBJ) -o $(NAME_BONUS) $(LDFLAGS)
	@echo "$(GREEN_DARK)Executable $(NAME_BONUS) created successfully!$(RESET)"

$(LIBFT):
	@make -C libft

$(MINI):
	@cmake MLX42 -B MLX42/build && make -C MLX42/build -j4

# Compilar archivos objeto y generar dependencias
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN_DARK)Compiled: $<$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR_BONUS)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(BONUS_INCLUDE) -c $< -o $@
	@echo "$(GREEN_DARK)Compiled: $<$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

-include $(DEP) $(BONUS_DEP)

bonus: libs mlx | $(OBJ_DIR)
	@if [ ! -f $(HBONUS) ] || \
		find $(BONUS_OBJ) -newer $(HBONUS) | grep -q .; then \
		$(MAKE) $(NAME_BONUS); \
		touch $(HBONUS); \
	else \
		echo "make bonus: Nothing to be done for 'bonus'."; \
	fi

clean:
	@make -C libft clean
	@make -C MLX42/build clean
	rm -f $(HBONUS)
	rm -rf $(OBJ_DIR)
	@echo "$(RED)Cleaned up object files.$(RESET)"

fclean: clean
	@make -C libft fclean
	rm -rf MLX42/build
	rm -f $(NAME) $(NAME_BONUS)
	@echo "$(RED)Executable $(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all bonus clean fclean re

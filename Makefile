# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 19:37:02 by victor            #+#    #+#              #
#    Updated: 2025/04/18 21:28:52 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME         = cub3D
NAME_BONUS   = cub3D_bonus

LIBFT        = libft/libft.a
MINI         = MLX42/build/libmlx42.a

OBJ_DIR      = obj
OBJ_DIR_BONUS = obj_bonus
SRC_DIR      = src
SRC_DIR_BONUS = src/bonus

# List of source files defined explicitly
SRC =   $(SRC_DIR)/cub3D.c \
		$(SRC_DIR)/camera.c \
		$(SRC_DIR)/parse.c \
		$(SRC_DIR)/render.c \
		$(SRC_DIR)/cub3D_utils.c \
		$(SRC_DIR)/validate_map.c \
		$(SRC_DIR)/parse_utils.c \
		$(SRC_DIR)/render_utils.c \
		$(SRC_DIR)/raycasting.c

BONUS_SRC =	$(SRC_DIR_BONUS)/cub3D_bonus.c \
			$(SRC_DIR_BONUS)/camera_bonus.c \
			$(SRC_DIR_BONUS)/collition_bonus.c \
			$(SRC_DIR_BONUS)/parse_bonus.c \
			$(SRC_DIR_BONUS)/render_bonus.c \
			$(SRC_DIR_BONUS)/cub3D_utils_bonus.c \
			$(SRC_DIR_BONUS)/validate_map_bonus.c \
			$(SRC_DIR_BONUS)/parse_utils_bonus.c \
			$(SRC_DIR_BONUS)/render_utils_bonus.c \
			$(SRC_DIR_BONUS)/raycasting_bonus.c \
			$(SRC_DIR_BONUS)/door_bonus.c \
			$(SRC_DIR_BONUS)/movement_bonus.c \
			$(SRC_DIR_BONUS)/enemies_bonus.c \
			$(SRC_DIR_BONUS)/minimap_bonus.c \
			$(SRC_DIR_BONUS)/textures_bonus.c \
			$(SRC_DIR_BONUS)/enemies_utils_bonus.c \
			$(SRC_DIR_BONUS)/minimap_utils_bonus.c \
			$(SRC_DIR_BONUS)/render_txt_bonus.c
      
# Manual conversion of sources to object files
OBJ       = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ = $(BONUS_SRC:$(SRC_DIR_BONUS)/%.c=$(OBJ_DIR_BONUS)/%.o)

INCLUDE      = -Iinc -Ilibft -IMLX42/include/MLX42
BONUS_INCLUDE = -Iinc/bonus

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -fsanitize=address -MMD -MP $(INCLUDE) 
LDFLAGS = $(LIBFT) $(MINI) -lglfw -lm

# Colors for messages
GREEN  = \033[0;32m
RED    = \033[0;91m
RESET  = \033[0m

all: prepare_mlx libs mlx $(NAME)

# Prepares MLX42 (if .git dir doesn't exists, then it's cloned)
prepare_mlx:
	@if [ ! -d "MLX42/.git" ]; then \
		echo "$(RESET)Cloning MLX42..."; \
		git clone https://github.com/codam-coding-college/MLX42.git MLX42; \
	fi

libs:
	@make -C libft

mlx: prepare_mlx
	@cmake MLX42 -B MLX42/build && make -C MLX42/build -j4

$(NAME): $(LIBFT) $(MINI) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)Executable $(NAME) successfully created.$(RESET)"

$(NAME_BONUS): $(LIBFT) $(MINI) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_INCLUDE) $(BONUS_OBJ) -o $(NAME_BONUS) $(LDFLAGS)
	@echo "$(GREEN)Executable $(NAME_BONUS) successfully created.$(RESET)"

# Bonus rule directly depends on the executable bonus
bonus: libs mlx $(NAME_BONUS)

# Rule for compiling source files of the main program
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled: $<$(RESET)"

# Rule for compiling source files of bonus
$(OBJ_DIR_BONUS)/%.o: $(SRC_DIR_BONUS)/%.c | $(OBJ_DIR_BONUS)
	$(CC) $(CFLAGS) $(BONUS_INCLUDE) -c $< -o $@
	@echo "$(GREEN)Compiled (bonus): $<$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR_BONUS):
	@mkdir -p $(OBJ_DIR_BONUS)

# Automatic inclusion of generated dependencies (-MMD -MP)
-include $(OBJ:.o=.d) $(BONUS_OBJ:.o=.d)

clean:
	@make -C libft clean
	@make -C MLX42/build clean
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_DIR_BONUS)
	@echo "$(RED)Object files deleted.$(RESET)"

fclean: clean
	@make -C libft fclean
	rm -rf MLX42/build
	rm -f $(NAME) $(NAME_BONUS)
	@echo "$(RED)Executables deleted.$(RESET)"

re: fclean all

.PHONY: all bonus clean fclean re

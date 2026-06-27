NAME        = push_swap
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

# Klasör Yapısı
SRC_DIR     = src
OBJ_DIR     = obj

# Kaynak Dosyalar (Dosyaları yazdıkça buraya ekleyebilirsin)
SRC_FILES   = main.c \
             

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Renk Kodları (Terminal çıktısı estetiği için)
GREEN       = \033[0;32m
YELLOW      = \033[0;33m
RESET       = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[✓] $(NAME) başarıyla derlendi!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)[-] Obje dosyaları temizlendi.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)[-] $(NAME) ve kütüphaneler temizlendi.$(RESET)"

re: fclean all

# Bonus kuralı (İleride checker yazarsan kullanmak için zorunlu)
bonus: all

.PHONY: all clean fclean re bonus

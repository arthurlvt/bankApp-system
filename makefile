# Nom de l'exécutable
TARGET = atm

# Dossiers
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
DATA_DIR = data

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR)
LIBS = -lcjson -lcurl

# Liste des fichiers sources
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Règle principale
all: check_dirs $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJ)
	@echo "🔧 Linking..."
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)
	@echo "✅ Build complete! Run with ./$(TARGET)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "🧩 Compiling $< ..."
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Vérification des dossiers
check_dirs:
	@if [ ! -d $(SRC_DIR) ]; then \
		echo "❌ Error: $(SRC_DIR) directory not found!"; \
		exit 1; \
	fi
	@if [ ! -d $(INC_DIR) ]; then \
		echo "❌ Error: $(INC_DIR) directory not found!"; \
		exit 1; \
	fi
	@if [ ! -d $(DATA_DIR) ]; then \
		mkdir -p $(DATA_DIR); \
	fi

# Nettoyage
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean

include .env

CC=clang++
APP_NAME = app
VERSION=-std=c++17
DEBUG= -g -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra
BUILD_DIR = bin
C_FILES = ./src/*.cpp ./src/Context/OpenGL/*.cpp ./src/Renderer/*.cpp ./vendor/glad/*.cpp ./vendor/ImGui/*.cpp ./src/Interface/*.cpp 
HEADER_FILES = -I./src/*.h -I./src/Context/OpenGL/*.h  -I./src/Renderer/*.h -I./src/Interface/*.h
APP_INCLUDES = -I$(INCLUDES_PATH) -framework Cocoa -framework OpenGL -framework IOkit -I$(INCLUDES_PATH)
APP_LINKERS = -L$(GLFW_BINARIES_PATH) -lglfw3 -L$(ASSIMP_BINARIES_PATH) -lassimp.5 

build:
	$(CC) $(VERSION) $(C_FILES) $(ENGINE_FILES) $(MATH_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(HEADER_FILES) $(APP_LINKERS)

debug:
	$(CC) $(VERSION) $(DEBUG) $(C_FILES) $(ENGINE_FILES) $(MATH_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(HEADER_FILES) $(APP_LINKERS)

clean:
	rm -rf bin/app

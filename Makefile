##
## EPITECH PROJECT, 2026
## ZAPPY
## File description:
## Makefile
##

CXX		?=	clang++

SERVER_NAME	=	zappy_server
GUI_NAME	=	zappy_gui
AI_NAME		=	zappy_ai

RAYLIB_DIR	=	libs_raylib/raylib-5.0_linux_amd64
RAYLIB_INC	=	$(RAYLIB_DIR)/include
RAYLIB_LIB	=	$(RAYLIB_DIR)/lib/libraylib.a

CXXFLAGS	=	-Wall -Wextra -std=c++20
CPPFLAGS	=	-iquote ./include

SERVER_SRC	=	src/server/commands/ACommands.cpp		\
				src/server/commands/AICommands.cpp		\
				src/server/commands/CommandParser.cpp	\
				src/server/commands/GUICommands.cpp		\
				src/server/commands/GUIEvents.cpp		\
				src/server/commands/HandleCommands.cpp	\
				src/server/commands/Incantation.cpp		\
				src/server/commands/NetworkBuffer.cpp	\
				src/server/HandleClients.cpp			\
				src/server/Handshake.cpp				\
				src/server/SpawnResources.cpp			\
				src/server/Hunger.cpp					\
				src/server/Victory.cpp					\
				src/server/Help.cpp						\
				src/server/Main.cpp						\
				src/server/Map.cpp						\
				src/server/ParseArgs.cpp				\
				src/server/Server.cpp					\
				src/gui/state/GameState.cpp				\
				src/gui/state/GameStatePlayer.cpp		\
				src/server/Utils.cpp					\

GUI_SRC		=	src/gui/Main.cpp		\
				src/gui/Run.cpp			\
				src/gui/Help.cpp		\
				src/gui/Args.cpp		\
				src/gui/Window.cpp		\
				src/gui/FreqButtons.cpp	\
				src/gui/HudClicks.cpp	\
				src/gui/network/Client.cpp			\
				src/gui/network/ClientUtils.cpp		\
				src/gui/network/ClientBuffer.cpp	\
				src/gui/parser/Parser.cpp			\
				src/gui/parser/Map.cpp				\
				src/gui/parser/Team.cpp				\
				src/gui/parser/Player.cpp			\
				src/gui/parser/Events.cpp			\
				src/gui/parser/EventsGame.cpp		\
				src/gui/state/GameState.cpp			\
				src/gui/state/GameStatePlayer.cpp		\
				src/gui/state/GameStateEffects.cpp		\
				src/gui/renderer/MapRenderer.cpp		\
				src/gui/renderer/MapRendererEggs.cpp	\
				src/gui/renderer/MapRendererEffects.cpp	\
				src/gui/MusicManager.cpp			\
				src/gui/renderer/PlayerRenderer.cpp		\
				src/gui/renderer/PlayerRendererModel.cpp	\
				src/gui/renderer/PlayerRendererUtils.cpp	\
				src/gui/renderer/CameraController.cpp	\
				src/gui/renderer/HudRenderer.cpp		\
				src/gui/renderer/HudRendererTeams.cpp		\
				src/gui/renderer/TileUtils.cpp			\
				src/gui/renderer/EggUtils.cpp			\
				src/gui/renderer/MinimapRenderer.cpp		\
				src/gui/renderer/SkyboxRenderer.cpp		\
				src/gui/renderer/PlanetRenderer.cpp		\
				src/gui/renderer/MoonRenderer.cpp

SERVER_OBJ	=	$(SERVER_SRC:.cpp=.o)
GUI_OBJ	=	$(GUI_SRC:.cpp=.o)

all:	raylib server gui ai

TESTS_SRC	=	tests/test_commands.cpp

TESTS_NAME	=	unit_tests

tests_run: $(TESTS_NAME)
	./$(TESTS_NAME)

$(TESTS_NAME): $(TESTS_SRC)
	$(CXX) -o $(TESTS_NAME) $(TESTS_SRC) src/gui/state/GameState.cpp \
	src/gui/state/GameStatePlayer.cpp \
	$(CXXFLAGS) \
	-iquote ./include -iquote ./include/server \
	-lcriterion

$(RAYLIB_LIB):
		mkdir -p libs_raylib
		wget -q https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz -O libs_raylib/raylib.tar.gz
		tar -xzf libs_raylib/raylib.tar.gz -C libs_raylib/
		$(RM) libs_raylib/raylib.tar.gz

raylib:	$(RAYLIB_LIB)

server:	$(SERVER_NAME)

gui:	$(GUI_NAME)

ai:	$(AI_NAME)

$(SERVER_NAME):	$(SERVER_OBJ)
		$(CXX) -o $(SERVER_NAME) $(SERVER_OBJ)

$(SERVER_OBJ):	CPPFLAGS += -iquote ./include/server

$(GUI_OBJ):	CPPFLAGS += -I$(RAYLIB_INC)

$(GUI_NAME):	$(GUI_OBJ)
		$(CXX) -o $(GUI_NAME) $(GUI_OBJ) $(RAYLIB_LIB) -lm -lpthread -ldl

$(AI_NAME):
	cp src/Zappy_ai/zappy_ai zappy_ai
		chmod +x zappy_ai
		chmod +x install.sh
		chmod +x start.sh

clean:
	$(RM) $(SERVER_OBJ) $(GUI_OBJ) $(AI_OBJ)

fclean:	clean
	$(RM) $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)

re:	fclean all

.PHONY:	all server gui ai clean fclean re tests_run

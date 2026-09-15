CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./libs/myteams -I./src/common \
            -I./src/server -I./src/client
LDLIBS   := -L./libs/myteams -lmyteams -luuid

# --- server ---
SERVER_BIN  := myteams_server
SERVER_SRCS := $(wildcard src/server/*.cpp) $(wildcard src/common/*.cpp)
SERVER_OBJS := $(SERVER_SRCS:.cpp=.o)

# --- client ---
CLIENT_BIN  := myteams_cli
CLIENT_SRCS := $(wildcard src/client/*.cpp) $(wildcard src/common/*.cpp)
CLIENT_OBJS := $(CLIENT_SRCS:.cpp=.o)

.PHONY: all re clean fclean

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

re:
	$(MAKE) fclean
	$(MAKE) all

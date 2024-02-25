#Will be the makefile to compute / execute / build the tests
#TODO, STILL IN PROGRESS

SRC_PATH = ./src
OBJ_PATH = ./obj
BIN_PATH = ./bin
BIN_TEST_PATH = $(BIN_PATH)/tests
INC_PATH = ./include

# ----------------------------------------------------------------------------
# PACKAGES NAME
# ----------------------------------------------------------------------------
MAIN_PATH = /main
TEST_PATH = /test
MONITOR_PATH = /monitor
CITIZEN_MANAGER_PATH = /citizen_manager
CI_OFFICER_PATH = /counter_intelligence_officer
ENEMY_COUNTRY_PATH = /enemy_country
ENEMY_SPY_NETWORK_PATH = /enemy_spy_network
SPY_SIMULATION_PATH = /spy_simulation
TIMER_PATH = /timer
COMMON_PATH = /common
UTILS_PATH = /utils

# ----------------------------------------------------------------------------
# EXECUTABLE NAME
# ----------------------------------------------------------------------------
TEST_LOGGER = $(BIN_TEST_PATH)/logger.test
TEST_TIMER = $(BIN_TEST_PATH)/timer.test
TEST_CAMERAS = $(BIN_TEST_PATH)/cameras.test.out
TEST_UNIT_CAMERAS = $(BIN_TEST_PATH)/cameras_unit.test
TEST_UTILS = $(BIN_TEST_PATH)/utils.test
TEST_EXEC = $(TEST_UTILS) $(TEST_LOGGER) $(TEST_CAMERAS) $(TEST_UNIT_CAMERAS)

# ----------------------------------------------------------------------------
# COMPILATION OPTIONS
# ----------------------------------------------------------------------------
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
CPPFLAGS = -I$(INC_PATH) 

# Compilation under MacOS X or Linux
ifeq ($(uname -s), Darwin)
    CPPFLAGS = -D_REENTRANT -I$(INC_PATH)  -I/usr/local/Cellar/ncurses/6.3/include
    LDFLAGS = -L/usr/local/Cellar/ncurses/6.3/lib -lncurses -lpthread -lm -lcriterion
else
    CPPFLAGS = -D_REENTRANT -I$(INC_PATH) 
    LDFLAGS = -lncurses -lpthread -lrt -lm -lcriterion
endif


# ----------------------------------------------------------------------------
# COMPILATION 
# ----------------------------------------------------------------------------
.PHONY: all clean distclean exec_tests TEST_EXEC

all: $(OBJ_PATH) $(BIN_PATH) $(BIN_TEST_PATH) $(TEST_EXEC) 

$(OBJ_PATH): 
	mkdir $@

$(BIN_PATH):
	mkdir $@

$(BIN_TEST_PATH):
	mkdir $@

# ----------------------------------------------------------------------------
# LOGGER OBJECTS TESTS FILES	->	Use this as a SAMPLE
# ----------------------------------------------------------------------------
OBJS_TEST_LOGGER = $(OBJ_PATH)/logger.o $(OBJ_PATH)/test_logger.o

$(OBJ_PATH)/%.o : $(SRC_PATH)$(TEST_PATH)$(MAIN_PATH)$(COMMON_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(TEST_LOGGER): $(OBJS_TEST_LOGGER)
	$(CC) $^ -o $@ $(LDFLAGS)




# ----------------------------------------------------------------------------
# TIMER OBJECTS TESTS FILES	
# ----------------------------------------------------------------------------
OBJS_TEST_TIMER = $(OBJ_PATH)/manage_timer.o $(OBJ_PATH)/test_manage_timer.o

$(OBJ_PATH)/%.o : $(SRC_PATH)$(TEST_PATH)$(MAIN_PATH)$(TIMER_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(TEST_TIMER): $(OBJS_TEST_TIMER)
	$(CC) $^ -o $@ $(LDFLAGS)

OBJS_TEST_CAMERAS = $(OBJ_PATH)/test_cameras.o $(OBJ_PATH)/cameras.o $(OBJ_PATH)/memory.o $(OBJ_PATH)/other.o $(OBJ_PATH)/path.o $(OBJ_PATH)/logger.o $(OBJ_PATH)/handle_fatal_error.o
OBJS_UNIT_TEST_CAMERAS = $(OBJ_PATH)/test_unit_cameras.o $(OBJ_PATH)/cameras.o $(OBJ_PATH)/memory.o $(OBJ_PATH)/other.o $(OBJ_PATH)/path.o $(OBJ_PATH)/logger.o


$(OBJ_PATH)/%.o : $(SRC_PATH)$(TEST_PATH)$(MAIN_PATH)$(CI_OFFICER_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(TEST_CAMERAS): $(OBJS_TEST_CAMERAS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TEST_UNIT_CAMERAS): $(OBJS_UNIT_TEST_CAMERAS)
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# UTILS
# ----------------------------------------------------------------------------
OBJS_UTIL = $(OBJ_PATH)/path.o \
			$(OBJ_PATH)/other.o 

OBJS_TEST_PATH = $(OBJ_PATH)/test_utils.o \
				 $(OBJS_UTIL)

$(OBJ_PATH)/test_utils.o : $(SRC_PATH)$(TEST_PATH)$(MAIN_PATH)$(UTILS_PATH)/test_utils.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(UTILS_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(TEST_UTILS): $(OBJS_TEST_PATH)
	$(CC) $^ -o $@ $(LDFLAGS)
# ----------------------------------------------------------------------------
# RUNNER
# ----------------------------------------------------------------------------

exec_tests:
	for test in $(BIN_TEST_PATH)/*.test; do ./$$test; done

# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
clean : 
	rm -rf $(OBJ_PATH)

distclean : clean 
	rm -rf $(BIN_PATH)
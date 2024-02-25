SRC_PATH = ./src
OBJ_PATH = ./obj
BIN_PATH = ./bin
INC_PATH = ./include

# ----------------------------------------------------------------------------
# PACKAGES NAME
# ----------------------------------------------------------------------------
MAIN_PATH = /main
UTILS_PATH = /utils
TEST_PATH = /test
MONITOR_PATH = /monitor
CITIZEN_MANAGER_PATH = /citizen_manager
CI_OFFICER_PATH = /counter_intelligence_officer
ENEMY_COUNTRY_PATH = /enemy_country
CASE_OFFICER_PATH = /case_officer
ENEMY_SPY_NETWORK_PATH = /enemy_spy_network
SPY_SIMULATION_PATH = /spy_simulation
TIMER_PATH = /timer
COMMON_PATH = /common
MEMORY_PATH = /memory

# ----------------------------------------------------------------------------
# EXECUTABLE NAME
# ----------------------------------------------------------------------------
MONITOR = $(BIN_PATH)/monitor.out
CITIZEN_MANAGER = $(BIN_PATH)/citizen_manager.out
CI_OFFICER = $(BIN_PATH)/counter_intelligence_officer.out
ENEMY_COUNTRY = $(BIN_PATH)/enemy_country.out
ENEMY_COUNTRY_TEST = $(BIN_PATH)/test_creator.out
ENEMY_SPY_NETWORK = $(BIN_PATH)/enemy_spy_network.out
SPY_SIMULATION = $(BIN_PATH)/spy_simulation.out
TIMER = $(BIN_PATH)/timer.out
CASE_OFFICER_MANAGER = $(BIN_PATH)/case_officer_manager.out

BUILD_LIST = $(MONITOR) $(CITIZEN_MANAGER) $(CI_OFFICER) $(ENEMY_COUNTRY) $(ENEMY_SPY_NETWORK) $(SPY_SIMULATION) $(TIMER) $(ENEMY_COUNTRY)

# ----------------------------------------------------------------------------
# COMPILATION OPTIONS
# ----------------------------------------------------------------------------
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
CPPFLAGS = -I$(INC_PATH) 

ifeq ($(uname -s), Darwin)
    CPPFLAGS = -D_REENTRANT -I$(INC_PATH)  -I/usr/local/Cellar/ncurses/6.3/include
    LDFLAGS = -L/usr/local/Cellar/ncurses/6.3/lib -lncurses -lpthread -lm -g 
else
    CPPFLAGS = -D_REENTRANT -I$(INC_PATH) 
    LDFLAGS = -lncurses -lpthread -lrt -lm -g 
endif


# ----------------------------------------------------------------------------
# COMPILATION 
# ----------------------------------------------------------------------------
.PHONY: all clean distclean tests

all: $(OBJ_PATH) $(BIN_PATH) $(MONITOR) $(TIMER) $(SPY_SIMULATION) $(CITIZEN_MANAGER) $(ENEMY_SPY_NETWORK) $(ENEMY_COUNTRY) $(CI_OFFICER)

$(OBJ_PATH) : 
	mkdir $@

$(BIN_PATH) :
	mkdir $@

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
OBJS_MONITOR = $(OBJ_PATH)/main_monitor.o \
			   $(OBJ_PATH)/monitor.o \
			   $(OBJ_PATH)/monitor_common.o \
			   $(OBJ_PATH)/memory.o \
			   $(OBJ_PATH)/path.o \
			   $(OBJ_PATH)/other.o \
			   $(OBJ_PATH)/logger.o \
			   $(OBJS_COMMON)

$(MONITOR): $(OBJS_MONITOR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_monitor.o : $(SRC_PATH)$(MAIN_PATH)$(MONITOR_PATH)/main_monitor.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(MONITOR_PATH)/%.c $(INC_PATH)$(MONITOR_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------
OBJS_COMMON = $(OBJ_PATH)/logger.o \
			  $(OBJ_PATH)/handle_fatal_error.o

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(COMMON_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

# ----------------------------------------------------------------------------
# UTILS OBJECTS FILES
# ----------------------------------------------------------------------------
OBJS_UTIL = $(OBJ_PATH)/path.o \
			$(OBJ_PATH)/other.o \
			$(OBJ_PATH)/common_signals.o

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(UTILS_PATH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

# ----------------------------------------------------------------------------
# MEMORY OBJECTS FILES
# ----------------------------------------------------------------------------

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(MEMORY_PATH)/%.c $(INC_PATH)$(MEMORY_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

# ----------------------------------------------------------------------------
# CITIZEN MANAGER
# ----------------------------------------------------------------------------
OBJS_CITIZEN_MANAGER = $(OBJ_PATH)/main_citizen_manager.o \
					   $(OBJ_PATH)/create_citizen.o \
					   $(OBJ_PATH)/citizen_manager.o \
					   $(OBJ_PATH)/action_citizen.o \
					   $(OBJ_PATH)/memory.o \
					   $(OBJS_UTIL) \
					   $(OBJS_COMMON)

$(CITIZEN_MANAGER): $(OBJS_CITIZEN_MANAGER)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_citizen_manager.o : $(SRC_PATH)$(MAIN_PATH)$(CITIZEN_MANAGER_PATH)/main_citizen_manager.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(CITIZEN_MANAGER_PATH)/%.c $(INC_PATH)$(CITIZEN_MANAGER_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 


# ----------------------------------------------------------------------------
# COUNTER INTELLIGENCE OFFICER
# ----------------------------------------------------------------------------
OBJS_COUNTER_INTELLIGENCE_OFFICER = $(OBJ_PATH)/main_counter_intelligence_officer.o \
									$(OBJ_PATH)/counter_intelligence_officer_manager.o \
									$(OBJ_PATH)/action_counter_intelligence_officer.o \
									$(OBJ_PATH)/memory.o \
									$(OBJ_PATH)/cameras.o \
									$(OBJS_UTIL) \
					   				$(OBJS_COMMON)


$(CI_OFFICER): $(OBJS_COUNTER_INTELLIGENCE_OFFICER)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_counter_intelligence_officer.o : $(SRC_PATH)$(MAIN_PATH)$(CI_OFFICER_PATH)/main_counter_intelligence_officer.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(CI_OFFICER_PATH)/%.c $(INC_PATH)$(CI_OFFICER_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 


# ----------------------------------------------------------------------------
# ENEMY COUNTRY
# ----------------------------------------------------------------------------
OBJS_ENEMY_COUNTRY = $(OBJ_PATH)/main_enemy_country.o \
					 $(OBJ_PATH)/enemy_country_manager.o \
					 $(OBJS_UTIL) \
					 $(OBJS_COMMON)

$(ENEMY_COUNTRY): $(OBJS_ENEMY_COUNTRY)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_enemy_country.o : $(SRC_PATH)$(MAIN_PATH)$(ENEMY_COUNTRY_PATH)/main_enemy_country.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(ENEMY_COUNTRY_PATH)/%.c $(INC_PATH)$(ENEMY_COUNTRY_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

# ----------------------------------------------------------------------------
# CASE OFFICER
# ----------------------------------------------------------------------------
OBJS_CASE_OFFICER = $(OBJ_PATH)/main_case_officer_manager.o \
					$(OBJ_PATH)/action_case_officer.o \
					$(OBJ_PATH)/case_officer_manager.o \
					$(OBJ_PATH)/create_case_officer.o \
					$(OBJ_PATH)/spies_manager.o \
					$(OBJ_PATH)/memory.o \
					$(OBJS_COMMON) \
					$(OBJS_UTIL)

$(CASE_OFFICER_MANAGER): $(OBJS_CASE_OFFICER)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_case_officer_manager.o : $(SRC_PATH)$(MAIN_PATH)$(CASE_OFFICER_PATH)/main_case_officer_manager.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(CASE_OFFICER_PATH)/%.c $(INC_PATH)$(CASE_OFFICER_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 


# ----------------------------------------------------------------------------
# ENEMY SPY NETWORK
# ----------------------------------------------------------------------------
OBJS_ENEMY_SPY_NETWORK = $(OBJ_PATH)/main_spy_network.o \
						$(OBJ_PATH)/create_spy_network.o \
						$(OBJ_PATH)/action_spy_network.o \
						$(OBJ_PATH)/spies_manager.o \
						$(OBJ_PATH)/day_action_spies.o \
						$(OBJ_PATH)/night_action_spies.o \
						$(OBJ_PATH)/fight_actions.o \
						$(OBJ_PATH)/memory.o \
						$(OBJ_PATH)/create_case_officer.o \
						$(OBJ_PATH)/action_case_officer.o \
						$(OBJ_PATH)/case_officer_manager.o \
						$(OBJS_UTIL) \
						$(OBJS_COMMON)
						
$(ENEMY_SPY_NETWORK): $(OBJS_ENEMY_SPY_NETWORK)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_spy_network.o : $(SRC_PATH)$(MAIN_PATH)$(ENEMY_SPY_NETWORK_PATH)/main_spy_network.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(ENEMY_SPY_NETWORK_PATH)/%.c $(INC_PATH)$(ENEMY_SPY_NETWORK_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 


# ----------------------------------------------------------------------------
# SPY SIMULATION
# ----------------------------------------------------------------------------
OBJS_SPY_SIMULATION = $(OBJ_PATH)/main_spy_simulation.o \
					  $(OBJ_PATH)/handle_timer_signal.o \
					  $(OBJ_PATH)/manage_timer.o \
					  $(OBJ_PATH)/handle_simulation.o \
					  $(OBJ_PATH)/handle_turn_action.o \
					  $(OBJ_PATH)/memory.o \
					  $(OBJS_COMMON)

$(SPY_SIMULATION): $(OBJS_SPY_SIMULATION)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_spy_simulation.o : $(SRC_PATH)$(MAIN_PATH)$(SPY_SIMULATION_PATH)/main_spy_simulation.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(SPY_SIMULATION_PATH)/%.c $(INC_PATH)$(SPY_SIMULATION_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 


# ----------------------------------------------------------------------------
# TIMER
# ----------------------------------------------------------------------------
OBJS_TIMER = $(OBJ_PATH)/main_timer.o \
			 $(OBJ_PATH)/manage_timer.o \
			 $(OBJ_PATH)/memory.o \
			 $(OBJS_COMMON)

$(TIMER): $(OBJS_TIMER)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/main_timer.o : $(SRC_PATH)$(MAIN_PATH)$(TIMER_PATH)/main_timer.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c 

$(OBJ_PATH)/%.o : $(SRC_PATH)$(MAIN_PATH)$(TIMER_PATH)/%.c $(INC_PATH)$(TIMER_PATH)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# BUILD TESTS
# ----------------------------------------------------------------------------
MAIN_OBJS = $(OBJS_MONITOR) \
			$(OBJS_CITIZEN_MANAGER) \
			$(OBJS_COUNTER_INTELLIGENCE_OFFICER) \
			$(OBJS_ENEMY_COUNTRY) \
			$(OBJS_ENEMY_SPY_NETWORK) \
			$(OBJS_SPY_SIMULATION) \
			$(OBJS_TIMER) \
			$(OBJS_COMMON)

build_test: $(MAIN_OBJS)
	make -f $(SRC_PATH)$(TEST_PATH)/Makefile MAIN_OBJS="$(MAIN_OBJS)"

# ----------------------------------------------------------------------------
# RUNNER @TODO
# ----------------------------------------------------------------------------

run:
	./$(BIN_PATH)/monitor

tests: $(OBJ_PATH) $(BIN_PATH) build_test
	make -f $(SRC_PATH)$(TEST_PATH)/Makefile exec_tests


# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
clean : 
	rm -rf $(OBJ_PATH)

distclean : clean 
	rm -rf $(BIN_PATH)

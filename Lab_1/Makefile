CC = gcc
CFLAGS = -O3 -g -lpthread -march=native -mtune=native

TARGET = gaussianpar
BUILD_DIR = build
INTRO_DIR = intro
TEST_DIR = tests
TEST = benchmark.sh
SRC_DIR = src
INC_DIR = inc
 
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
DEPS = $(wildcard $(INC_DIR)/*.h)

all: intro $(BUILD_DIR)/$(TARGET)

intro: $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/bankaccount $(INTRO_DIR)/bankaccount.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/falsesharing $(INTRO_DIR)/falsesharing.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/gaussianseq $(INTRO_DIR)/gaussianseq.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate $(INTRO_DIR)/pthreadcreate.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate2 $(INTRO_DIR)/pthreadcreate2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate3 $(INTRO_DIR)/pthreadcreate3.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/qsortseq $(INTRO_DIR)/qsortseq.c

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: clean cleanall

test: $(BUILD_DIR)/gaussianseq $(BUILD_DIR)/$(TARGET)
	./$(TEST_DIR)/$(TEST) $(BUILD_DIR)/gaussianseq $(BUILD_DIR)/$(TARGET)


clean:
	rm -rf $(BUILD_DIR)
	rm $(TEST_DIR)/gaussianseq
	rm $(TEST_DIR)/$(TARGET)

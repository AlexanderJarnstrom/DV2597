CC = gcc
CFLAGS = -O2 -g -lpthread

BUILD_DIR = build
INTRO_DIR = intro
TEST_DIR = tests
TEST = benchmark.sh
SRC_DIR = src
INC_DIR = inc
 
all: $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/bankaccount $(SRC_DIR)/bankaccount.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/falsesharing $(SRC_DIR)/falsesharing.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/gaussianseq $(SRC_DIR)/gaussianseq.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate $(SRC_DIR)/pthreadcreate.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate2 $(SRC_DIR)/pthreadcreate2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/pthreadcreate3 $(SRC_DIR)/pthreadcreate3.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/qsortseq $(SRC_DIR)/qsortseq.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/qsortpar $(SRC_DIR)/qsortpar.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/gaussianpar $(SRC_DIR)/gaussianpar.c

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: clean cleanall test/gaussian test/qsort

test/gaussian: $(BUILD_DIR)/gaussianseq $(BUILD_DIR)/gaussianpar
	./$(TEST_DIR)/$(TEST) $(BUILD_DIR)/gaussianseq $(BUILD_DIR)/gaussianpar

test/qsort: $(BUILD_DIR)/qsortpar $(BUILD_DIR)/qsortseq
	@echo -n "Running $(BUILD_DIR)/qsortpar ... "
	@./$(BUILD_DIR)/qsortpar > $(TEST_DIR)/qsortpar.txt
	@echo "done."
	@echo -n "Running $(BUILD_DIR)/qsortseq ... "
	@./$(BUILD_DIR)/qsortseq > $(TEST_DIR)/qsortseq.txt
	@echo "done."
	diff $(TEST_DIR)/qsortpar.txt $(TEST_DIR)/qsortseq.txt

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TEST_DIR)/qsortpar.txt
	rm -f $(TEST_DIR)/qsortseq.txt
	rm -f $(TEST_DIR)/callgrind/*

CC = gcc
CFLAGS = -O2 -g -lpthread

BUILD_DIR = build
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

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: clean cleanall

clean:
	rm -rf $(BUILD_DIR)

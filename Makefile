LAB_1_DIR = Lab_1
LAB_2_DIR = Lab_2
LAB_3_DIR = Lab_3

all: $(LAB_1_DIR) $(LAB_2_DIR) $(LAB_3_DIR)
	@make -C $(LAB_1_DIR)
	@make -C $(LAB_2_DIR)
	@make -C $(LAB_3_DIR)

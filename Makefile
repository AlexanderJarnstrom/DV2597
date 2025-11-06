.PHONY: dev/lab1 dev/lab2 dev/lab3

dev/lab1:
	docker run -v "./Lab_1/:/usr/src/" -ti jarnstrom/dev_base:latest

dev/lab2:
	docker run -v "./Lab_2/:/usr/src/" -ti jarnstrom/dev_base:latest

dev/lab3:
	docker run -v "./Lab_3/:/usr/src/" -ti jarnstrom/dev_base:latest

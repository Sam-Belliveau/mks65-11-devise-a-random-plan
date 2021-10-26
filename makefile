
all: random_plan

random_plan: main.c 
	gcc main.c -o random_plan

run: random_plan
	./random_plan
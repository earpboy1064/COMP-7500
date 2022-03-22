hellomake: AUbatch-temp.c
	gcc -g -w -c command_parser.c
	gcc -g -w -c dispatching_module.c
	gcc -g -w -c scheduling_module.c
	gcc -g -w -c AUbatch-temp.c -lpthread
	gcc -o batchtest AUbatch-temp.o command_parser.o scheduling_module.o dispatching_module.o -lpthread -lm

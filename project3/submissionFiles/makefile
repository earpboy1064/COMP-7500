hellomake: AUbatch.c
	gcc -g -w -c command_parser.c
	gcc -g -w -c dispatching_module.c
	gcc -g -w -c scheduling_module.c
	gcc -g -w -c AUbatch.c -lpthread
	gcc -g -o process process.c
	gcc -o AUbatch AUbatch.o command_parser.o scheduling_module.o dispatching_module.o -lpthread -lm

hellomake: AUbatch.c
	gcc -g -c command_parser.c
	gcc -g -c scheduling_module.c
	gcc -g -w -c AUbatch-temp.c -lpthread
	gcc -o batchtest AUbatch-temp.o command_parser.o scheduling_module.o -lpthread -lm

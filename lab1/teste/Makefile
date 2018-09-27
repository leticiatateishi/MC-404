arquivo1.s: arquivo1.c
	gcc -S arquivo1.c -o arquivo1.s
arquivo1.o: arquivo1.s
	as -o arquivo1.o arquivo1.s
arquivo2.s: arquivo2.c
	gcc -S arquivo2.c -o arquivo2.s
arquivo2.o: arquivo2.s
	as arquivo2.s -o arquivo2.o
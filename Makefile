all: Executable/main1 Executable/main2 Executable/main3 Executable/main4 Executable/main5 Executable/main6 Executable/main7 Executable/main8 Executable/main9

Fonctions/e1.o: Fonctions/e1.c
	gcc -Wall -c $^ -o $@

Fonctions/e2.o: Fonctions/e2.c
	gcc -Wall -c $^ -o $@

Fonctions/e3.o: Fonctions/e3.c
	gcc -Wall -c $^ -o $@

Fonctions/e4.o: Fonctions/e4.c
	gcc -Wall -c $^ -o $@

Fonctions/e5.o: Fonctions/e5.c
	gcc -Wall -c $^ -o $@

Fonctions/e6.o: Fonctions/e6.c
	gcc -Wall -c $^ -o $@

Fonctions/e7.o: Fonctions/e7.c
	gcc -Wall -c $^ -o $@

Fonctions/e8.o: Fonctions/e8.c
	gcc -Wall -c $^ -o $@

Fonctions/e9.o: Fonctions/e9.c
	gcc -Wall -c $^ -o $@

Executable/main1.o: Tests/main1.c
	gcc -Wall -c $^ -o $@

Executable/main2.o: Tests/main2.c
	gcc -Wall -c $^ -o $@

Executable/main3.o: Tests/main3.c
	gcc -Wall -c $^ -o $@

Executable/main4.o: Tests/main4.c
	gcc -Wall -c $^ -o $@

Executable/main5.o: Tests/main5.c
	gcc -Wall -c $^ -o $@

Executable/main6.o: Tests/main6.c
	gcc -Wall -c $^ -o $@

Executable/main7.o: Tests/main7.c
	gcc -Wall -c $^ -o $@

Executable/main8.o: Tests/main8.c
	gcc -Wall -c $^ -o $@

Executable/main9.o: Tests/main9.c
	gcc -Wall -c $^ -o $@

Executable/main1: Executable/main1.o Fonctions/e1.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main2: Executable/main2.o Fonctions/e1.o Fonctions/e2.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main3: Executable/main3.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main4: Executable/main4.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main5: Executable/main5.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o Fonctions/e5.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main6: Executable/main6.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o Fonctions/e5.o Fonctions/e6.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main7: Executable/main7.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o Fonctions/e5.o Fonctions/e6.o Fonctions/e7.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main8: Executable/main8.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o Fonctions/e5.o Fonctions/e6.o Fonctions/e7.o Fonctions/e8.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

Executable/main9: Executable/main9.o Fonctions/e1.o Fonctions/e2.o Fonctions/e3.o Fonctions/e4.o Fonctions/e5.o Fonctions/e6.o Fonctions/e7.o Fonctions/e8.o Fonctions/e9.o
	gcc -Wall -lm -lssl -lcrypto -o $@ $^

clean:
	rm Fonctions/*.o Executable/* candidates.txt keys.txt declarations.txt Executable/vgcore* -f Blockchain/*

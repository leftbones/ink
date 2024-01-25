build: main.c
	cc main.c util.c editor.c buffer.c cursor.c -lncurses
run:
	cc main.c util.c editor.c buffer.c cursor.c -lncurses
	./a.out
test:
	cc main.c util.c editor.c buffer.c cursor.c -lncurses
	./a.out test.txt

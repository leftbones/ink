build: main.c
	cc main.c editor.c buffer.c cursor.c util.c -lncurses
run:
	cc main.c editor.c buffer.c cursor.c util.c -lncurses
	./a.out
test:
	cc main.c editor.c buffer.c cursor.c util.c -lncurses
	./a.out test.txt

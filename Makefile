game: game.c
	gcc game.c -o game -I. -L. -lm -lpthread -lX11 -lXrandr -lGL -lGLU -lgl2d -lglfw
 
clean:
	rm game

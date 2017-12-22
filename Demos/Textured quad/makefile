# set this variable to the director in which you saved the common files
commondir = ../common/


all : textured

textured:  $(commondir)GL_utilities.c $(commondir)Ubuntu/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c textured.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Ubuntu/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c textured.c -o textured -I$(commondir) -I../common/Ubuntu -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

MicroGlut.o: MicroGlut.h 
	gcc -w -c $(commondir)Ubuntu/MicroGlut.c

clean :
	rm textured



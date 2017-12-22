# set this variable to the director in which you saved the common files
commondir = ../common/

all : transparent-teapot

transparent-teapot : transparent-teapot.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o transparent-teapot -I$(commondir) -I../common/Linux -lGL -DGL_GLEXT_PROTOTYPES transparent-teapot.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11

clean :
	rm transparent-teapot

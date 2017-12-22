# set this variable to the director in which you saved the common files
commondir = ../common/


all : mipmapcube4

mipmapcube4:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c mipmapcube4.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c mipmapcube4.c -o mipmapcube4 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11



MicroGlut.o: MicroGlut.h 
	gcc -w -c $(commondir)Linux/MicroGlut.c


clean :
	rm mipmapcube4
	
	







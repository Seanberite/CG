# set this variable to the director in which you saved the common files
commondir = ../common/


all : texcube

texcube:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c texcube.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c texcube.c -o texcube -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11



MicroGlut.o: MicroGlut.h 
	gcc -w -c $(commondir)Linux/MicroGlut.c


clean :
	rm texcube
	
	







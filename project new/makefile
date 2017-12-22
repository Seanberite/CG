# set this variable to the director in which you saved the common files
commondir = ../common/


all : project

project:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c project.c $(commondir)LoadTGA.c
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c project.c -o project -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXi -lX11



MicroGlut.o: MicroGlut.h
	gcc -w -c $(commondir)Linux/MicroGlut.c


clean :
	rm project

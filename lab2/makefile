# set this variable to the director in which you saved the common files
commondir = ../common/


all : lab2-1 lab2-2 lab2-3 lab2-4 lab2-5 lab2-7 

lab2-1:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-1.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-1.c -o lab2-1 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

lab2-2:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-2.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-1.c -o lab2-2 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

lab2-3:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-3.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-3.c -o lab2-3 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

lab2-4:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-4.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-4.c -o lab2-4 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

lab2-5:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-5.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-5.c -o lab2-5 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

lab2-7:  $(commondir)GL_utilities.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-7.c $(commondir)LoadTGA.c 
	gcc -std=c99 -Wall $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.o $(commondir)loadobj.c $(commondir)VectorUtils3.c lab2-7.c -o lab2-7 -I$(commondir) -I../common/Linux -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11

MicroGlut.o: MicroGlut.h 
	gcc -w -c $(commondir)Linux/MicroGlut.c


clean :
	rm lab2-1
	rm lab2-2
	rm lab2-3
	rm lab2-4
	rm lab2-5
	rm lab2-7

	
	







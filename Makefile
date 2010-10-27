MY_ARCH := $(shell uname)

FILES := MultiDimScatterPlot.cpp

all:
#ifeq(${MY_ARCH}, Darwin)
		c++ -framework GLUT -framework OpenGL -o MultiDimScatterPlot ${FILES}
#endif

clean:
	rm *.out
	rm MultiDimScatterPlot


run:
	./MultiDimScatterPlot smoothed.CSV # run with default data

	

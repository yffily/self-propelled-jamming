
cc 	= g++	# g++
cFLAG 	= -Wall -O4 # -g # 
#LFLAGS=-O3 -lgsl -lgslcblas -L/home/henkes/local/lib -L/home/henkes/local/lib -lnetcdf_c++ -lnetcdf
cLIB 	= -lm -lstdc++ -L/usr/lib/ # -lboost_system -lboost_filesystem #-L/usr/local/lib/ -L/usr/lib/
cINC 	= # -I/usr/include/  #-I/usr/local/include

cHEADERS = $(wildcard src/*.h)
cSOURCES = $(wildcard src/*.cpp)

cOBJECTS = $(patsubst %.cpp, %.o, $(cSOURCES))
cTARGET = execute # $(patsubst %.cpp, %.exe, $(cSOURCES))


# si il y a un code c++, on le compile
# If there is a C++ code (i.e. if there are files that end in .cpp), compile as C++
# Else: do nothing
ifneq ($(cSOURCES),)

# les deux regles sont differentes :
# la premiere cree chaque objet separement a partir de la source correspondante
# ie autant de commandes qu'il y a de sources
# la deuxieme cree un executable a partir de l'ensemble des objets
# Rem : on peut omettre la premiere et utiliser la regle par defaut de make pour creer les objets

%.o : %.cpp %.h
	$(cc) $(cLIB) $(cFLAG) $(cINC) -c $< -o $@

$(cTARGET) : $(cOBJECTS) 
	$(cc) $(cFLAG) $(cLIB) $(cINC) $(cOBJECTS) -o $(cTARGET)
#	rm $(cOBJECTS)

else
all :
	@echo "rien a compiler"
endif

clean :
	rm -r $(cOBJECTS) $(cTARGET)

cleano :
	rm $(cOBJECTS)


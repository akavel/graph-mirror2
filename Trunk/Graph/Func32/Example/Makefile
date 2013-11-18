FUNC32 = ..
ifeq ($(shell uname),Darwin)
BOOST = /opt/local/include
else
BOOST = /usr/include/boost
endif

Test: Example.cpp ../libFunc32.a
	g++ Example.cpp -o Example -L$(FUNC32) -L/opt/local/lib -lFunc32 -lgsl -lgslcblas -I$(FUNC32) -I$(BOOST) -gstabs -pthread -lboost_system #-fprofile-arcs -ftest-coverage

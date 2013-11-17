FUNC32 = ..
ifeq ($(shell uname),Darwin)
BOOST = /opt/local/include
else
BOOST = /usr/include/boost
endif


Test: Test.cpp ../libFunc32.a
	g++ Test.cpp -o Test -L$(FUNC32) -L/opt/local/lib -lFunc32 -lgsl -lgslcblas -I$(FUNC32) -I$(BOOST) -gstabs -pthread -lboost_system-mt  #-fprofile-arcs -ftest-coverage

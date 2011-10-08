FUNC32 = ..
BOOST = /usr/include/boost

Test: Test.cpp ../libFunc32.a
	g++ Test.cpp -o Test -L$(FUNC32) -lFunc32 -llibgsl -I$(FUNC32) -I$(SPIRIT) -I$(BOOST) -gstabs #-fprofile-arcs -ftest-coverage

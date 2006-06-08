FUNC32 = ..
SPIRIT = ../../spirit
BOOST = ../../boost

Test: Test.cpp ../libFunc32.a
	g++ Test.cpp -o Test -L$(FUNC32) -lFunc32 -I$(FUNC32) -I$(SPIRIT) -I$(BOOST) -gstabs+ #-fprofile-arcs -ftest-coverage

// UnitTest.cpp : Defines the entry point for the console application.
//
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

#include "ParserTest.h"
#include <SamException.h>

using namespace std;

int main(int argc, char* argv[])
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( ParserTest::suite() );
	runner.run();

	char c;
	cin>>c;
	return 0;


}


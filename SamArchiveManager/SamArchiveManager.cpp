// SamArchiveManager.cpp : Defines the entry point for the console application.
//

#include "Parser.h"
#include "SamException.h"
#include <iostream>
#include "StandardHeaders.h"
#include <exception>
using namespace std;

int main(int argc, char* argv[])
{

	FilterStrategy * newfilter = new RegExpFilter( "(\\d{4}[- ]){3}\\d{4}" );
	//usage example
	//haven't got time to implement the menu :(
	HImageBuilder builder ( new CompositeImageBuilder() );
	Parser::parse( "D:\\testx", builder );
	HMemoryImage image = builder->getImage();
	image->display_info( 0 );
	image->write_packed( "D:\\" );

	//parse archive, display_info ( result will be same as above )
	Parser::parse( "D:\\testx.sbc.txt", builder );
	image = builder->getImage();
	image->display_info( 0 );
	image->write_unpacked("D:\\fuckingA");



	char c;
	cin>>c;
	return 0;



}


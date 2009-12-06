#ifndef PARSERTEST_H_
#define PARSERTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <Parser.h>
#include <ImageBuilder.h>
#include <SamException.h>

using namespace CppUnit;
using namespace std;

class ParserTest : public TestFixture
{
protected:
	CPPUNIT_TEST_SUITE( ParserTest );

//	CPPUNIT_TEST_EXCEPTION( win_format_absolute_not_exist, SamException );

	CPPUNIT_TEST( win_format_absolute_exist );

//	CPPUNIT_TEST( on_root );

//	CPPUNIT_TEST( relative_up );

//	CPPUNIT_TEST( current );

	CPPUNIT_TEST_SUITE_END();

	HImageBuilder builder;

public:
	void setUp()
	{
		builder.reset( new CompositeImageBuilder() );

	}

	void win_format_absolute_not_exist()
	{
		Parser::parse( "D:\\test", builder );
	}

	void win_format_absolute_exist()
	{
		{
			Parser::parse( "D:\\testx", builder );
			HMemoryImage image = builder->getImage();
			image->display_info( 0 );
		}
		//image->write_packed( "D:\\testx.smb");
		
	}

	void on_root()
	{
		Parser::parse( "G:\\", builder );
		HMemoryImage image = builder->getImage();
		image->display_info( 0 );
	}

	void relative_up()
	{
		Parser::parse( "..", builder );
		HMemoryImage image = builder->getImage();
		image->display_info( 0 );
	}

	void current()
	{
		Parser::parse( ".", builder );
		HMemoryImage image = builder->getImage();
		image->display_info( 0 );
	}

	void tearDown()
	{
	}

};

#endif
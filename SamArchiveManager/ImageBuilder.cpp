#include "ImageBuilder.h"

#include <list>

#include "FileSysComponent.h"
#include "SamException.h"

using namespace boost::filesystem;
using namespace boost;
using namespace std;


void CompositeImageBuilder::addDirectory( const path& filename )
{
	if ( image_.get() == NULL )
	{
		string rootname = filename.filename();
		image_.reset( new Directory( rootname ) );
	}
	else
	{
		string leaf = filename.filename();
		HFileSysComponent dirtoplace( new Directory( leaf ) );

		path parent_path = filename.parent_path();
		dir_parse_and_insert ( parent_path, dirtoplace );

	}
}

void CompositeImageBuilder::addFile( const path& filename, HFileData& fdata )
{
	if ( image_.get() == NULL )
	{
		string rootname = filename.filename();
		image_.reset( new File( rootname, fdata ) );
	}
	else
	{
		string leaf = filename.filename();
		HFileSysComponent filetoplace( new File( leaf, fdata ) );

		path parent_path = filename.parent_path();
		dir_parse_and_insert ( parent_path, filetoplace );

	}
}

void CompositeImageBuilder::split_path( path filename, list<string>& separated_path )
{
	separated_path.clear();
	string leaf;
	while ( ( leaf = filename.filename() )!= "" )
	{
		if ( leaf == image_->getName() ) break;
		separated_path.insert( separated_path.begin(), leaf );
		filename = filename.parent_path();
	}

}

void CompositeImageBuilder::dir_parse_and_insert( path& pth,
											  const HFileSysComponent& toinsert )
{

	list<string> separated_path;
	split_path( pth, separated_path );

	HFileSysComponent parseComponent = image_;

	while ( ! separated_path.empty() )
	{
		parseComponent =
			parseComponent->getChild( separated_path.front() );

		if ( parseComponent.get() == NULL )
			throw SamException( SamException::INVALID_PATH );

		separated_path.erase( separated_path.begin() );
	}

	parseComponent->addChild( toinsert );


}

void CompositeImageBuilder::fromArchive( const path& pth )
{
	FileSysComponent *result;
	std::ifstream ifs( pth.string().c_str() );
	//try as text archive
	try {
        boost::archive::text_iarchive ia( ifs );
		SerializeStrategy::registerClasses( ia );
        ia >> BOOST_SERIALIZATION_NVP ( result );
	} catch(...) { }

	//try as xml archive
	try {
        boost::archive::xml_iarchive ia( ifs );
		SerializeStrategy::registerClasses( ia );
        ia >> BOOST_SERIALIZATION_NVP ( result );
	} catch(...) { }

	//try as binary archive
	try {
        boost::archive::binary_iarchive ia( ifs );
		SerializeStrategy::registerClasses( ia );
        ia >> result;
	} catch(...) { }
	image_.reset( result );
}

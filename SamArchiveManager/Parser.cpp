#include "Parser.h"
#include "SamException.h"

using namespace boost::filesystem;
using namespace std;

void Parser::parse( const string& fpath, HImageBuilder& builder )
{
	path rootpath( fpath );
	if ( !exists( rootpath ) ) 
		throw SamException( SamException::FILE_NO_EXIST );
	
	FileType type = getType( rootpath );

	switch ( type )
	{
	case directory: 
		builder->addDirectory( rootpath );
		filesys_parse( rootpath, builder );
		break;
	case archive:
		builder->fromArchive( rootpath );
		break;

	}

}

FileType Parser::getType( const path& pth )
{
	if ( is_directory(pth) ) 
		return directory;
	else
	{
		return archive;
	}


}

void Parser::filesys_parse( const path& dir_path, HImageBuilder& builder )
{
	if ( !exists( dir_path ) ) throw SamException( SamException::FILE_NO_EXIST );

	directory_iterator end_itr; 
	for ( directory_iterator itr( dir_path );
		itr != end_itr;
		++itr )
	{
		if ( is_directory(itr->status()) )
		{
			builder->addDirectory( itr->path() );
			filesys_parse( itr->path(), builder );
		}
		else if ( is_regular_file( itr->status() )  )
		{
			HFileData fdata ( new FileData( itr->path() ) );
			builder->addFile( itr->path(), fdata );
		}	
	}
}
#ifndef Parser_H_
#define Parser_H_

#include "StandardHeaders.h"
#include "ImageBuilder.h"

//enum of structures that can be parsed
enum FileType { directory, archive };

//class responsible for parsing a filesystem or archive
//this is the builder's director
class Parser
{

public:
	//the class's only public method
	//
	//parameters: - the path to the directory or archive to be parsed
	//            - the builder that will be responsible for generating
	//              it's coresponding memory image
	static void parse( const std::string& fpath, HImageBuilder& builder );

private:

	//given a path determines weather the target is a directory or an archive
	//any file given is considered an archive ( if not the builder will throw )
	static FileType getType( const boost::filesystem::path& pth ) ;

	//recursive function to parse the directory structure and 
	//pass the parsed data to the builder
	static void filesys_parse( const boost::filesystem::path& dir_path, HImageBuilder& builder );
};

#endif

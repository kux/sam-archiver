#ifndef IMAGEBUILDER_H_
#define IMAGEBUILDER_H_

#include "MemoryImage.h"
#include "FileSysComponent.h"

//forward declarations
class FileData;
class ImageBuilder;
typedef boost::shared_ptr<ImageBuilder> HImageBuilder;

//the Builder interface
//responsible for assembling a MemoryImage object
//the addFile, addDirectory and fromArchive methods will be called by the Parser ( director )
//for now the only concrete class that implements ImageBuilder is CompositeImageBuilder
//if another kind of internal representation of the MemoryImage is required another class
//must implement MemoryImage and a coresponding concrete builder should be made for it
//by implementing ImageBuilder
class ImageBuilder
{
public:
	//path is used to determines the file's or folder's place in the image
	virtual void addFile( const boost::filesystem::path& filename, HFileData& fdata ) = 0;

	virtual void addDirectory( const boost::filesystem::path& dirname ) = 0;

	virtual void fromArchive( const boost::filesystem::path& arpath ) = 0;

	//return the assambled memory image
	virtual HMemoryImage getImage()=0;

	virtual ~ImageBuilder(void){};
};

//class responsible for assembling a MemoryImage object that 
//uses composite pattern for the internal representation
class CompositeImageBuilder:public ImageBuilder
{
	HFileSysComponent image_;

public:
	CompositeImageBuilder() {}

	void addFile ( const boost::filesystem::path& filename, HFileData& fdata );

	void addDirectory( const boost::filesystem::path& dirname );

	virtual void fromArchive( const boost::filesystem::path& arpath ) ;

	HMemoryImage getImage()
	{
		//after the image is taken, reset image_ so that same builder can be used for another build
		HFileSysComponent for_return( image_ );
		HFileSysComponent empty;
		image_ = empty;
		return  HMemoryImage( for_return );
	}

private:

	void dir_parse_and_insert(boost::filesystem::path& pth, const HFileSysComponent& toinsert );

	void split_path( boost::filesystem::path filename,
		             std::list<std::string>& separated_path );



};

#endif

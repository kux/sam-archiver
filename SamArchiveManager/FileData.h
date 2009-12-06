#ifndef FDATA_H_
#define FDATA_H_

#include "StandardHeaders.h"
#include "SamException.h"

//
// Class that wraps a file's content
// it simply has an unsigned char vector that loads all the file content ( if not to big of course )
//
class FileData;
//to ease memory management we only work with shared pointers to FileData objects
typedef boost::shared_ptr<FileData> HFileData;

class FileData
{
private:

	std::vector<unsigned char> data_;

	//required for serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_NVP ( data_ );
    }

public:
	//default constructor, required by serialization
	FileData() {};

	//load the file pointed by pth 
	FileData( const boost::filesystem::path& pth )
	{
		FILE* file;
		if ( ( file = fopen( pth.string().c_str(), "rb") ) == NULL )
		{
			//unable to open file
			throw SamException( SamException::UNABLE_TO_OPEN_FILE );
		}

		fseek( file, 0, SEEK_END );
		size_t fsize = ftell( file );
		fseek( file, 0, SEEK_SET );

		if ( fsize > data_.max_size() )
		{
			//file bigger than the vector might hold
			fclose(file);
			throw SamException ( SamException::FILE_TO_BIG );
		}
		data_.resize( fsize );
		int readcount = 1;
		if (fsize!= 0) readcount = fread( &data_.at(0), fsize, 1, file );
		if ( readcount != 1 )
		{
			//unable to read file content
			fclose(file);
			throw SamException( SamException::UNABLE_TO_READ_FILE );
		}
		fclose(file);
	}

	//display content at console, for debugging purpose
	void debug_display()
	{
		for ( size_t i = 0; i<data_.size(); ++i )
			std::cout<<data_[i];
	}

	//create file at path and place content in it
	void write_file ( boost::filesystem::path& f_path )
	{
		FILE* file;
		if ( ( file = fopen( f_path.string().c_str(), "wb") ) == NULL )
		{
			throw SamException( SamException::UNABLE_TO_OPEN_FILE );
		}
		
		if ( data_.size() > 0 )fwrite( &data_.at(0), data_.size(), 1, file );
		fclose(file);
	}

};


#endif

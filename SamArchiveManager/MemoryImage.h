#ifndef MEMORYIMAGE_H_
#define MEMORYIMAGE_H_

#include "StandardHeaders.h"

//forward delcarations
class MemoryImage;
class FilterStrategy;
typedef boost::shared_ptr<MemoryImage> HMemoryImage;
class SerializeStrategy;

//interface for the memory image model
class MemoryImage
{
public:
	virtual void display_info( int level ) const = 0;
	virtual void write_packed(  const boost::filesystem::path& pwhere ) = 0;
	virtual void write_unpacked( const boost::filesystem::path& pwhere ) {};
	virtual void set_strategy( SerializeStrategy* strategy )  = 0;
	virtual std::string getName( )const = 0;
	virtual void set_filter( FilterStrategy* filter ) = 0;

	virtual ~MemoryImage(){}
private:

	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(MemoryImage)


#endif

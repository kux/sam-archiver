#ifndef SERIALIZE_STRATEGY_H_
#define SERIALIZE_STRATEGY_H_

#include "StandardHeaders.h"
#include "MemoryImage.h"

//archive's base extension
const std::string AREXTENSION="sbc";

class SerializeStrategy
{
public:
	/*
		!!!!!!!!!!!!!!!!!!!!!! 
		All concrete classes derived form MemoryImage must
		be registered here in order for the serialization mechanism to work
		!!!!!!!!!!!!!!!!!!!!!!
	*/
	template < class Archive >
	static void registerClasses(Archive & ar)
	{
		ar.register_type(static_cast<Directory *>(NULL));
		ar.register_type(static_cast<File*>(NULL));
	}
	
	virtual void writeArchive( const boost::filesystem::path& pwhere, const MemoryImage* image) = 0;	

	virtual ~SerializeStrategy(void){};
};

typedef boost::shared_ptr<SerializeStrategy> HSerializeStrategy;

class TextArchiveStrategy: public SerializeStrategy
{
public:
	void writeArchive( const boost::filesystem::path& pwhere, const MemoryImage* image )
	{
		boost::filesystem::path fullpath = pwhere / ( image->getName() + std::string(".")
			+ AREXTENSION + std::string(".txt")  );
		std::ofstream ofs( fullpath.string().c_str() );
		boost::archive::text_oarchive oar( ofs );
		registerClasses( oar );
		oar & BOOST_SERIALIZATION_NVP(image);
	}
};

class XmlArchiveStrategy: public SerializeStrategy
{

public:
	void writeArchive( const boost::filesystem::path& pwhere, const MemoryImage* image )
	{
		boost::filesystem::path fullpath = pwhere / ( image->getName() + std::string(".")
			+ AREXTENSION + std::string(".xml")  );
		std::ofstream ofs( fullpath.string().c_str() );
		boost::archive::xml_oarchive oar( ofs );
		registerClasses( oar );
		oar & BOOST_SERIALIZATION_NVP(image);
	}
};

class BinaryArchiveStrategy: public SerializeStrategy
{

public:
	void writeArchive( const boost::filesystem::path& pwhere, const MemoryImage* image )
	{
		boost::filesystem::path fullpath = pwhere / ( image->getName() + std::string(".")
			+ AREXTENSION + std::string(".bnr")  );
		std::ofstream ofs( fullpath.string().c_str() );
		boost::archive::binary_oarchive oar( ofs );
		registerClasses( oar );
		oar & BOOST_SERIALIZATION_NVP(image);
	}
};



#endif
#ifndef FILESYSCOMPONENT_H_
#define FILESYSCOMPONENT_H_

#include "StandardHeaders.h"

#include "MemoryImage.h"
#include "FileData.h"
#include "SamException.h"
#include "SerializeStrategy.h"
#include "FilterStrategy.h"

class FileSysComponent;
//to ease memory management we only work with shared pointers to FileSysComponents
typedef boost::shared_ptr< FileSysComponent > HFileSysComponent;

class FileSysComponent: public MemoryImage
{
private:

	std::string name_;
	FileSysComponent* parent_;
	static HSerializeStrategy serStr_;
protected:
	static HFilterStrategy filter_;
	
private:
	//friend class required by serialization
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		//the BOOST_SERIALIZATION_NVP macro is required for generating xml tags
		//if the archive is an xml_oarchive
		ar & BOOST_SERIALIZATION_NVP( boost::serialization::base_object<MemoryImage>(*this) );
        ar & BOOST_SERIALIZATION_NVP( name_ );
		ar & BOOST_SERIALIZATION_NVP(parent_);
    }

	//constructors, destructor
public:

	FileSysComponent():parent_(NULL){};
	FileSysComponent( std::string name ):name_(name), parent_(NULL){}
	virtual ~FileSysComponent(){}

	//from MemoryImage
public:
	virtual void display_info( int level ) const = 0;
	virtual void write_unpacked( const boost::filesystem::path &pwhere ) = 0;
	void set_strategy( SerializeStrategy* strategy )
	{
		serStr_.reset( strategy );
	}

	void set_filter( FilterStrategy* filter )
	{
		filter_.reset( filter );
	}

	void write_packed(  const boost::filesystem::path& pwhere ) 
	{
		serStr_->writeArchive( pwhere, this );
	}

	//declared by FileSysComponent
public:
	virtual void addChild( HFileSysComponent child ) ;
	virtual HFileSysComponent getChild( const std::string& name ) const =0;
	std::string getName() const { return name_; }
	boost::filesystem::path getPath() const;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(FileSysComponent)


class File: public FileSysComponent
{
private:

	HFileData fdata_;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_NVP ( boost::serialization::base_object<FileSysComponent>(*this) );
		ar & BOOST_SERIALIZATION_NVP ( fdata_ );
    }

	//constructors
public:
	File(){}
	File( std::string name, const HFileData& fdata ):
	  FileSysComponent( name ), fdata_(fdata) {}

	//from memory image
public:
	void display_info( int level ) const ;

	void write_unpacked( const boost::filesystem::path &pwhere )
	{
		boost::filesystem::path f_pth = pwhere / getPath();
		fdata_->write_file( f_pth );
	}

public:
	//method not supported, just throw exception if client calls ( should never happen )
	void addChild( HFileSysComponent name )
	{
		throw SamException( SamException::METHOD_NOT_SUPPORTED );
	}

	//method not supported, just throw exception if client calls ( should never happen )
	HFileSysComponent getChild( const std::string& name ) const
	{
		throw SamException( SamException::METHOD_NOT_SUPPORTED );
	}

};

class Directory: public FileSysComponent
{
private:

	std::list< HFileSysComponent > children_;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_NVP ( boost::serialization::base_object<FileSysComponent>(*this) );
		//if we have a filter, we only serialize children who pass the filter
		if ( filter_.get() != NULL )
		{
			std::list<HFileSysComponent> plist;
			for ( std::list<HFileSysComponent>::const_iterator i = children_.begin();
				i != children_.end();
				++i )
			{
				if ( filter_->applyFilter ( (*i)->getName() ) ) plist.push_back( *i );
			}
			
			ar & BOOST_SERIALIZATION_NVP ( plist );
		}
		else
		{
			ar & BOOST_SERIALIZATION_NVP ( children_ );
		}
    }

public:
	Directory(){}
	Directory( std::string name ):
	  FileSysComponent( name ) {}

public:
	void display_info( int level ) const ;
	virtual void write_unpacked( const boost::filesystem::path &pwhere );

public:
	void addChild( HFileSysComponent child );
	HFileSysComponent getChild( const std::string& name ) const;

};

#endif

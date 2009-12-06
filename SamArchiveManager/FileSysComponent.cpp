#include "FileSysComponent.h"
#include <iostream>

using namespace std;
using namespace boost::filesystem;

HSerializeStrategy FileSysComponent::serStr_( new TextArchiveStrategy() );
HFilterStrategy FileSysComponent::filter_ /*( new RegExpFilter("*") )*/;

void FileSysComponent::addChild( HFileSysComponent child )
{
	child->parent_= this;
}


path FileSysComponent::getPath() const
{
	path pth( name_ );
	FileSysComponent *prs = parent_;
	while ( prs != NULL )
	{
		pth = path( prs->getName() ) /  pth;
		prs = prs->parent_;
	}
	return pth;
}

HFileSysComponent Directory::getChild( const string& name ) const
{
	list< HFileSysComponent >::const_iterator end = children_.end();
	for ( list<HFileSysComponent>::const_iterator i = children_.begin();
		  i != end;
		  ++i )
		if ( (*i)->getName() == name ) return (*i);

	return HFileSysComponent();
}

void Directory::addChild( HFileSysComponent child )
{
	FileSysComponent::addChild( child );

	HFileSysComponent check = getChild( child->getName() );
	if ( check.get() != NULL )
		throw SamException( SamException::INVALID_PATH );

	children_.push_back( child );
}

void Directory::write_unpacked( const boost::filesystem::path &pwhere )
{
	path dir_pth = pwhere / getPath();
	create_directory( dir_pth );
	for ( list<HFileSysComponent>::iterator i = children_.begin();
		i != children_.end();
		++i )
	{
		(*i)->write_unpacked( pwhere );
	}
}

void File::display_info( int level ) const
{
	cout<<endl;
	for ( int i = 0; i < level; ++i ) cout<<"   ";
	cout<<"file:  "<<getName()<<endl;
	for ( int i = 0; i < level; ++i ) cout<<"   ";
	cout<<"file path: "<<getPath()<<endl;
#ifdef _DEBUG
	for ( int i = 0; i < level; ++i ) cout<<"   ";
	cout<<"----->>>";
	fdata_->debug_display();
	cout<<endl;
#endif

}


void Directory::display_info ( int level ) const
{
	cout<<endl;
	for ( int i = 0; i < level; ++i ) cout<<"   ";
	cout<<"dir:  "<<getName()<<endl;
	for ( int i = 0; i < level; ++i ) cout<<"   ";
	cout<<"dir path: "<<getPath()<<endl;

	for ( list<HFileSysComponent>::const_iterator i = children_.begin(); i != children_.end(); ++i )
		(*i)->display_info( level + 1 );
}



#ifndef FILTERSTRATEGY_H_
#define FILTERSTRATEGY_H_

#include "StandardHeaders.h"

class FilterStrategy;
typedef boost::shared_ptr<FilterStrategy> HFilterStrategy;

//filename filtering interface
class FilterStrategy
{
public:
	virtual bool applyFilter( const std::string& fname ) = 0;
	virtual ~FilterStrategy(){};
};

class RegExpFilter: public FilterStrategy
{
	boost::regex expression_;
public:
	RegExpFilter( const std::string& filter ):expression_( filter.c_str() )
	{
	}

	bool applyFilter( const std::string& fname ) 
	{ 
		return boost::regex_match( fname.c_str(), expression_ );
	}
};

#endif
#ifndef SamException_H_
#define SamException_H_

#include "StandardHeaders.h"

class SamException
{
private:
	static bool isInitialized_ ;
	static std::map< int, std::string > SamExceptionMap_;

	int exCode_;

public:
	static const int FILE_NO_EXIST;
	static const int INVALID_PATH;
	static const int METHOD_NOT_SUPPORTED;
	static const int UNABLE_TO_OPEN_FILE;
	static const int FILE_TO_BIG;
	static const int UNABLE_TO_READ_FILE;

	SamException(int exCode) throw()
		:exCode_(exCode)
	{
		if ( ! isInitialized_ )
		{
			SamExceptionMap_[FILE_NO_EXIST] = "Provided width and length are bigger than the maximum values allowed";
			SamExceptionMap_[INVALID_PATH] = "The path provided does not fit the currently constructed image";
			SamExceptionMap_[METHOD_NOT_SUPPORTED] = "That method is not callable from that object";
			SamExceptionMap_[UNABLE_TO_OPEN_FILE] = "Unable to open file";
			SamExceptionMap_[FILE_TO_BIG] = "File to big to archive";
			SamExceptionMap_[UNABLE_TO_READ_FILE] = "Unable to read file";

			isInitialized_ = true;
		}

	}

	std::string getErrorMsg()
	{
		return SamExceptionMap_[ exCode_ ];
	}
	~SamException() {};
};

#endif /* SamException_H_ */

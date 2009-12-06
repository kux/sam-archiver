#include "SamException.h"

bool SamException::isInitialized_ = false;
std::map<int , std::string> SamException::SamExceptionMap_;

const int SamException::FILE_NO_EXIST = 0 ;
const int SamException::INVALID_PATH = 1;
const int SamException::METHOD_NOT_SUPPORTED = 2;
const int SamException::UNABLE_TO_OPEN_FILE = 3;
const int SamException::FILE_TO_BIG = 4;
const int SamException::UNABLE_TO_READ_FILE = 5;
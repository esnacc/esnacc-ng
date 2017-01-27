// FILE: snaccexcept.cpp
//
//
#include "snaccexcept.h"

#include <string.h>
#include <stdio.h>

namespace SNACC
{
const char * ConstraintErrorStringList[ SEQ_OF_SIZE_VALUE_RANGE + 1 ] = {
    "INTEGER VALUE CONSTRAINT ERROR :: Integer value is not within Upper and Lower constrained Bounds!!\n",
    "INTEGER VALUE CONSTRAINT ERROR :: Integer value is not equal to single value constraint!!\n",
    "STRING SIZE CONSTRAINT ERROR :: String size is not equal to single value size constraint!!\n",
    "STRING SIZE CONSTRAINT ERROR :: String size is not within Upper and Lower constrained Bounds!!\n",
    "STRING ALPHA CONSTRAINT ERROR :: String contents not within designated permitted alphabet!!\n",
    "STRING ALPHA CONSTRAINT ERROR :: String contents not within designated types normal alphabet!!\n",
    "WIDE STRING SIZE CONSTRAINT ERROR :: String size is not equal to single value size constraint!!\n",
    "WIDE STRING SIZE CONSTRAINT ERROR :: String size is not within Upper and Lower constrained Bounds!!\n",
    "WIDE STRING ALPHA CONSTRAINT ERROR :: String contents not within designated permitted alphabet!!\n",
    "WIDE STRING ALPHA CONSTRAINT ERROR :: String contents not within designated types normal alphabet!!\n",
    "OCTET STRING CONSTRAINT ERROR :: Octet String size is not equal to single value size constraint!!\n",
    "OCTET STRING CONSTRAINT ERROR :: Octet String size is not within Upper and Lower constrained Bounds!!\n",
    "BIT STRING CONSTRAINT ERROR :: Bit String size is not equal to single value size constraint!!\n",
    "BIT STRING CONSTRAINT ERROR :: Bit String size is not within Upper and Lower constrained Bounds!!\n",
    "SET OF CONSTRAINT ERROR :: Set Of element count is not equal to single value size constraint!!\n",
    "SET OF CONSTRAINT ERROR :: Set Of element count is not within Upper and Lower constrained Bounds!!\n",
    "SEQ OF CONSTRAINT ERROR :: Seq Of element count is not equal to single value size constraint!!\n",
    "SEQ OF CONSTRAINT ERROR :: Seq Of element count is not within Upper and Lower constrained Bounds!!\n"
};
}

using namespace SNACC;

SnaccException::SnaccException(long errorCode) throw()
{
   m_errorCode = errorCode;
   stackPos = -1;
   memset(&stack[0], 0, sizeof(CallStack)*STACK_DEPTH);
}

SnaccException::SnaccException(const char *file, long line_number,
                               const char *function, const char *whatStrIn,
                               long errorCode) throw()
{

   memset(&stack[0], 0, sizeof(CallStack)*STACK_DEPTH);

   try
   {
      if (whatStrIn != NULL)
         this->m_whatStr = whatStrIn;
   }
   catch (...)
   {
      // do nothing
   }

   m_errorCode = errorCode;
   stackPos = 0;
   stack[0].file = file;
   stack[0].line_number = line_number;
   stack[0].function = function;
}

SnaccException::~SnaccException() throw()
{
   m_errorCode = DEFAULT_ERROR_CODE;
   stackPos = -1;
}

SnaccException & SnaccException::operator=(const SnaccException &o)
{
   try
   {
      stackPos = o.stackPos;
      memcpy(stack, o.stack, sizeof(stack));
      m_errorCode = o.m_errorCode;

      m_whatStr = o.m_whatStr;
   }
   catch (...)
   {
      // do nothing
   }

   return *this;
}


void SnaccException::push(const char *file, long line_number,
                          const char *function) throw()
{
   if (stackPos < STACK_DEPTH - 1)
   {
      stackPos++;
      stack[stackPos].file = file;
      stack[stackPos].line_number = line_number;
      stack[stackPos].function = function;
   }
}

const char * SnaccException::what() const throw()
{
   return m_whatStr.c_str();
}

void SnaccException::getCallStack(std::ostream &os) const
{
   int i = 0;
   const char *ptr = NULL;

   for(; i <= stackPos; i++)
   {
#ifdef WIN32
      if ((ptr=strrchr(stack[i].file, '\\')) == NULL)
#else
      if ((ptr=strrchr(stack[i].file,  '/')) == NULL)
#endif
         ptr =         stack[i].file;
      else
         ptr++;

      os << ptr;
      os << "\t" << stack[i].line_number;

      if (stack[i].function)
         os << "\t" << stack[i].function << "\n";
      else
         os << "\t" << "\n";

   }
}

FileException::FileException(const char *filename, enum FileErrType errType,
                             const char *file, long line_number,
                             const char *function)  throw() :
SnaccException(file, line_number, function, NULL, FILE_IO_ERROR)
{
   switch (errType)
   {
      case OPEN:
         strcpy(whatStr,"Error opening file: ");
         break;
      case CREATE:
         strcpy(whatStr,"Error creating file: ");
         break;
      case READ:
         strcpy(whatStr,"Error reading file: ");
         break;
      case WRITE:
         strcpy(whatStr,"Error writing file: ");
         break;
   }
   strcat(whatStr, filename);
}

const char * FileException::what() const throw()
{
   return &whatStr[0];
}


MemoryException::MemoryException(long memorySize, const char *variable,
                                 const char *file, long line_number,
                                 const char *function) throw() :
SnaccException(file, line_number, function, "MemoryException", MEMORY_ERROR)
{
   sprintf(memoryInfo, "Error allocating %ld bytes for ", memorySize);
   int memUsed = strlen(memoryInfo);
   int len2copy = strlen(variable);
   if (len2copy > (128 - memUsed - 1))
       len2copy = 128 - memUsed - 1;
   memcpy(&memoryInfo[memUsed], variable, len2copy);
   memoryInfo[memUsed + len2copy] = '\0';
}

const char * MemoryException::what() const throw()
{
   return &memoryInfo[0];
}


InvalidTagException::InvalidTagException(const char *type, long tagId,
                                         const char *file, long line_number,
                                         const char *function) throw() :
SnaccException(file, line_number, function, "InvalidTagException", INVALID_TAG)
{
   sprintf(wrongTagInfo,"Tag [%ld] is invalid for type ", tagId);
   int memUsed = strlen(wrongTagInfo);
   int len2copy = strlen(type);
   if (len2copy > (128 - memUsed - 1))
       len2copy = 128 - memUsed - 1;
   memcpy(&wrongTagInfo[memUsed], type, len2copy);
   wrongTagInfo[memUsed + len2copy] = '\0';
}
const char * InvalidTagException::what() const throw()
{
   return &wrongTagInfo[0];
}



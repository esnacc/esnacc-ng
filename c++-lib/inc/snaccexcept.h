// FILE: asnexcept.h
//
#ifndef _SNACCEXCEPT_H_
#define _SNACCEXCEPT_H_

#ifdef WIN32
#pragma warning(disable: 4514)
#pragma warning(push,3)
#include <iostream>
#pragma warning(pop)
#else
#include <iostream>
#endif

#include "asn-config.h"

#define STACK_DEPTH 20

#define STACK_ENTRY __FILE__,__LINE__,_func

#define FUNC(_funcStr) const char *_func = _funcStr;

#define EXCEPT(_what, _err) SNACC::SnaccException(STACK_ENTRY, _what, _err)
#define SNACC_EXCEPT(_what) SNACC::SnaccException(STACK_ENTRY, _what) 
#define SNACC_MEMORY_EXCEPT(s,v) SNACC::MemoryException(s,v,STACK_ENTRY)
#define DEFAULT_ERROR_CODE -256

_BEGIN_SNACC_NAMESPACE

// SNACC ERROR CODES

#define ERRBASE 6000

#define SNACC_OK       0
#define MEMORY_ERROR  (ERRBASE + 1) // Memory allocation failed
#define BOUNDS_ERROR  (ERRBASE + 2) // Length of object is invalid
#define INVALID_TAG   (ERRBASE + 3) // Invalid ASN.1 Tag for object
#define INVALID_ANY   (ERRBASE + 4) // Invalid ANY
#define DECODE_ERROR  (ERRBASE + 5) // Decoded past end of data
#define RESTRICTED_TYPE_ERROR (ERRBASE + 6) // Restricted type check failed
#define INTEGER_ERROR (ERRBASE + 7) // Integer related errors
#define PARAMETER_ERROR (ERRBASE + 8) // Invalid parameter
#define OID_ERROR     (ERRBASE + 9) // OID related error
#define FILE_IO_ERROR (ERRBASE + 10) // File error
#define BUFFER_ERROR  (ERRBASE + 11) // Buffer error
#define ENCODE_ERROR  (ERRBASE + 12) // General encode error
#define CONSTRAINT_ERROR (ERRBASE + 13) //Constraint out of range

enum ConstraintErrorCode{
    INTEGER_VALUE_RANGE = 0,
    INTEGER_SINGLE_VALUE,
    STRING_SIZE_SINGLE_VALUE,
    STRING_SIZE_VALUE_RANGE,
    STRING_PERMITTED_ALPHA,
    STRING_NORMAL_ALPHA,
    WIDE_STRING_SIZE_SINGLE_VALUE,
    WIDE_STRING_SIZE_VALUE_RANGE,
    WIDE_STRING_PERMITTED_ALPHA,
    WIDE_STRING_NORMAL_ALPHA,
    OCTS_SIZE_SINGLE_VALUE,
    OCTS_SIZE_VALUE_RANGE,
    BITS_SIZE_SINGLE_VALUE,
    BITS_SIZE_VALUE_RANGE,
    SET_OF_SIZE_SINGLE_VALUE,
    SET_OF_SIZE_VALUE_RANGE,
    SEQ_OF_SIZE_SINGLE_VALUE,
    SEQ_OF_SIZE_VALUE_RANGE
};

extern const char *ConstraintErrorStringList[];

typedef struct _CallStack
{
   const char *file;
   const char *function;
   long        line_number;
} CallStack;


class SNACCDLL_API SnaccException: public std::exception
{
public:
   SnaccException(long errorCode=SNACC_OK) throw();
   SnaccException(const char *file, long line_number, const char *function=NULL,
    const char *whatStr=NULL, long errorCode=DEFAULT_ERROR_CODE) throw();
   virtual const char * what() const throw();
   void push(const char *file, long line_number, const char *function=NULL) throw();
   virtual ~SnaccException() throw();

   virtual void getCallStack(std::ostream &os) const;
   const CallStack * getCallStack(void) const { return stack; }
   SnaccException & operator=(const SnaccException &o);
   short getStackPos() { return stackPos; }
   long  m_errorCode;

protected:
    // first element in array is populated by the constructor.  All other
	// elements in the array are populated by push().
	//
   short stackPos;
   CallStack stack[STACK_DEPTH];
   std::string m_whatStr;
};

class SNACCDLL_API InvalidTagException : public SnaccException
{
public:
   InvalidTagException(const char *type, long tagId, const char *file, long line_number,
      const char *function) throw();
   virtual const char * what(void) const throw();
private:
   char wrongTagInfo[128];
};

class SNACCDLL_API FileException : public SnaccException
{
public:
   enum FileErrType {OPEN=1, CREATE, READ, WRITE};
   FileException(const char *fileName, enum FileErrType errType, const char *file, 
      long line_number, const char *function) throw();

   virtual const char * what(void) const throw();
private:
   char whatStr[512];
};

class SNACCDLL_API MemoryException : public SnaccException
{
public:
   MemoryException(long memorySize, const char *variable, const char *file, 
      long line_number, const char *function) throw();
   const char *what() const throw();
private:
   char memoryInfo[128];
};

class SNACCDLL_API BoundsException : public SnaccException
{
public:
   BoundsException(const char *whatStr, const char *file, long line_number,
      const char *function) throw() :SnaccException(file, line_number, function, whatStr, 
      BOUNDS_ERROR) {};
};

class SNACCDLL_API DecodeException : public SnaccException
{
public:
   DecodeException(const char *file, long line_number, const char *function) throw()
      :SnaccException(file, line_number, function, "Decode past end of data",
      DECODE_ERROR) {};

};

class SNACCDLL_API ParameterException : public SnaccException
{
public:
   ParameterException(const char *whatStr, const char *file, long line_number, const char *function) throw()
      :SnaccException(file, line_number, function, whatStr,
      PARAMETER_ERROR) {};
};

class SNACCDLL_API OidException : public SnaccException
{
public:
   OidException (const char *whatStr, const char *file, long line_number, const char *function) throw()
      :SnaccException(file, line_number, function, whatStr,
      OID_ERROR) {};
};

class SNACCDLL_API BufferException : public SnaccException
{
public:
   BufferException (const char *whatStr, const char *file, long line_number, const char *function) throw()
      :SnaccException(file, line_number, function, whatStr,
      BUFFER_ERROR) {};
};

class SNACCDLL_API ConstraintException : public SnaccException
{
public:
   ConstraintException (const char *whatStr, const char *file, long line_number, const char *function) throw()
      :SnaccException(file, line_number, function, whatStr,
      CONSTRAINT_ERROR) {};
};

_END_SNACC_NAMESPACE
#endif // _SNACCEXCEPT_H_

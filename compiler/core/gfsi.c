#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(WIN32) || defined(WIN16)
#pragma warning(disable: 4115)
#include <io.h>
#include <direct.h>
#include <time.h>
#include <windows.h>
#include <winbase.h>
#else
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#endif



#ifndef WIN32

static char *striEnd(const char *string1, const char *string2);

#endif



typedef void * GFSI_HANDLE; /* Generic File System Interface (GFSI) */



// GFSI_GetFirstFile:
// PURPOSE: Returns first file in a path
//

char *GFSI_GetFirstFile(GFSI_HANDLE *gfsi_handle, const char *path,
						const char *extension)
{

   char *fn = NULL;

#ifdef WIN32
   char real_path[256];
   WIN32_FIND_DATA findData;
   *gfsi_handle = NULL;

   if (extension == NULL)
      sprintf(real_path,"%s\\*.*",path);
   else
      sprintf(real_path,"%s\\*.%s", path, extension);

   do
   {
      if (fn == NULL)
         *gfsi_handle = (GFSI_HANDLE) FindFirstFile(real_path, &findData);
      else
         FindNextFile((HANDLE) *gfsi_handle, &findData);
      if (*gfsi_handle != NULL && (int) *gfsi_handle != -1)
      {
          if(fn != NULL)
              free(fn);
          fn = strdup(findData.cFileName);
      }
   } while ((gfsi_handle != INVALID_HANDLE_VALUE) &&
            (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY));

#else
   struct dirent *dirEntry = NULL;
   *gfsi_handle = NULL;
   *gfsi_handle = (GFSI_HANDLE) opendir(path);
   if (*gfsi_handle != NULL)
   {
      if (extension != NULL)
      {
         while ((dirEntry = readdir( (DIR *) *gfsi_handle)) != NULL)
         {
            fn = striEnd(dirEntry->d_name, extension);
            if (fn != NULL) 
            {
               fn = strdup(dirEntry->d_name);
               break;
            }
         }
      }
      else
      {
         while ((dirEntry = readdir( (DIR *) *gfsi_handle)) != NULL)
         {
			fn = dirEntry->d_name;
            if ((fn != NULL) && (strcmp(fn,".") != 0) && 
				(strcmp(fn, "..") != 0))
			{
               fn = strdup(dirEntry->d_name);
			   break;
			}
         }
      }
   }
#endif
   return fn;
} // END OF FUNCTION GFSI_GetFirstFile

// GFSI_GetNextFile:
// PURPOSE: Returns next file in a path
//
char *GFSI_GetNextFile( GFSI_HANDLE *gfsi_handle, const char *extension)
{
    char *fn = NULL;
#ifdef WIN32
    WIN32_FIND_DATA findData;
    char bDone = FALSE;
    extension = extension;
   // NOTE: We ignore the extension paramater for the WIN32 Case
   do
   {
      if (FindNextFile((HANDLE) *gfsi_handle, &findData))
	  {
		  if (findData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			  fn = strdup(findData.cFileName);
	  }
	  else
	  {
		  bDone = TRUE;
	  }
   } while (!bDone && (fn == NULL));
#else
   struct dirent *dirEntry = NULL;
   if (extension != NULL)
   {
      while ((dirEntry = readdir( (DIR *) *gfsi_handle)) != NULL)
      {
         fn = striEnd(dirEntry->d_name, extension);
         if (fn != NULL)
         {
            fn = strdup(dirEntry->d_name);
            break;
         }
      }
   }
   else
   {
      if ((dirEntry = readdir( (DIR *) *gfsi_handle)) != NULL)
      {
         fn = strdup(dirEntry->d_name);
      }
   }
#endif
   return fn;
} // END OF FUNCTION GFSI_GetNextFile



// GFSI_Close:
//
void GFSI_Close( GFSI_HANDLE *gfsi_handle )
{
#ifdef WIN32
   FindClose((HANDLE) *gfsi_handle);
#else
   if (*gfsi_handle != NULL)
   {
      closedir((DIR *) *gfsi_handle);
   }
#endif
} // END OF FUNCTION GFSI_Close



long findFileSize(char *fileName)
{
    struct stat buf;
    int result;
    /*  Get data associated with "stat.c": */
    result = stat(fileName, &buf);
    /*  Check if statistics are valid: */
    if (result != 0)
        return 0;
    else
        return(buf.st_size);
}


#ifndef WIN32
static char *striEnd(const char *string1, const char *string2)
{
/* The striEnd function performs a case-insensitive comparison of the two 
strings in reverse (starting from the ends).  The function returns a pointer 
to the occurrence of string2 within string1, only if it occurs at the end of 
string1.  This function returns NULL if string2 does not occur at the end of 
string1 or if string1 or string2 is of 0 length.  For example:

    string1 = "The quick brown fox jumped over the lazy dog"
    string2 = "Lazy Dog"
    result = "lazy dog"  (in string1)
*/
    long len1, len2, x;
	char c1, c2;

	if ((string1 == NULL) || (string2 == NULL))
		return NULL;
    len1 = strlen(string1);
    len2 = strlen(string2);
    if ((len1 == 0) || (len2 == 0) || (len2 > len1))
        return NULL;
    x = 1;
	do
	{
		c1 = string1[len1 - x];
		if ((c1 >= 'A') && (c1 <= 'Z'))
			c1 += 'a' - 'A';
		c2 = string2[len2 - x];
		if ((c2 >= 'A') && (c2 <= 'Z'))
			c2 += 'a' - 'A';
	}
    while ((c1 == c2) && (len2 > x++));
    if (c1 == c2)
        return (char *)&string1[len1 - len2];
	else
		return NULL;
} /* end of striEnd() */
#endif
// EOF acl_FileFuncs.cpp


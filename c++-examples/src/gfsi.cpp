#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(WIN32) || defined(WIN16)
#include <io.h>
#include <direct.h>
#include <time.h>
#include <windows.h>
#include <winbase.h>
#else
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#endif
#include "asnutil.h"

typedef void * GFSI_HANDLE; /* Generic File System Interface (GFSI) */

// GFSI_GetFirstFile:
// PURPOSE: Returns first file in a path
//
char *GFSI_GetFirstFile(GFSI_HANDLE *gfsi_handle, char *path, char *extension)
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
          fn = strdup(findData.cFileName);
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
            fn = strstr(dirEntry->d_name, extension);
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
char *GFSI_GetNextFile( GFSI_HANDLE *gfsi_handle, char *extension)
{
    char *fn = NULL;

#ifdef WIN32
    WIN32_FIND_DATA findData;
    bool bDone = false;

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
		  bDone = true;
	  }

   } while (!bDone && (fn == NULL));
#else
   struct dirent *dirEntry = NULL;
   if (extension != NULL)
   {
      while ((dirEntry = readdir( (DIR *) *gfsi_handle)) != NULL)
      {
         fn = strstr(dirEntry->d_name, extension);
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


// EOF acl_FileFuncs.cpp

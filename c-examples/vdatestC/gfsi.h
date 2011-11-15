#ifdef WIN32
#pragma warning(disable: 4305 4309 4101)
#endif

typedef void * GFSI_HANDLE; /* Generic File System Interface (GFSI) */
char *         GFSI_GetFirstFile(GFSI_HANDLE *gfsi_handle, char *path,
                        char *extension);
char *         GFSI_GetNextFile(GFSI_HANDLE *gfsi_handle, char *extension);
void           GFSI_Close(GFSI_HANDLE *gfsi_handle);

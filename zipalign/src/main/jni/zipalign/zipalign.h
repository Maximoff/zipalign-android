#ifndef LIB_ZIPALIGN_H
#define LIB_ZIPALIGN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
  Unless specified, all functions below return 1 on success and 0 on failure.
*/

/*
  in_filename: the input zip/apk filename
  out_filename: the output zip/apk filename
  alignment: alignment in bytes, for example, 4 provides 32-bit alignment
  force: if 1 is given and the target file exists, overwrite it. Use 0 if you do not wish to overwrite the target.
  pageAlignSharedLibs: Align .so files to 4096 and other files to alignTo, or all files to alignTo if false..
  
  Returns 0 on success.
*/
int alignZip(const char *in_filename, const char *out_filename,
             int alignment, int force, bool pageAlignSharedLibs);

/*
  filename: the filename to check for alignment
  alignment: alignment in bytes, for example, 4 provides 32-bit alignment
  pageAlignSharedLibs: Align .so files to 4096 and other files to alignTo, or all files to alignTo if false..
  
  Returns 0 on success.
*/
int isAlignedZip(const char *filename, int alignment, bool pageAlignSharedLibs);


#ifdef __cplusplus
}
#endif
#endif
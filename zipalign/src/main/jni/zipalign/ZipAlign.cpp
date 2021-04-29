/*
 * Zip alignment tool
 */
#include "ZipFile.h"

#include <stdlib.h>
#include <stdio.h>

using namespace android;

static int getAlignment(bool pageAlignSharedLibs, int defaultAlignment,
    ZipEntry* pEntry) {

    static const int kPageAlignment = 4096;

    if (!pageAlignSharedLibs) {
        return defaultAlignment;
    }

    const char* ext = strrchr(pEntry->getFileName(), '.');
    if (ext && strcmp(ext, ".so") == 0) {
        return kPageAlignment;
    }

    return defaultAlignment;
}

/*
 * Copy all entries from "pZin" to "pZout", aligning as needed.
 */
static int copyAndAlign(ZipFile *pZin, ZipFile *pZout, int alignment, bool pageAlignSharedLibs) {
    int numEntries = pZin->getNumEntries();
    ZipEntry *pEntry;
    int bias = 0;
    status_t status;

    for (int i = 0; i < numEntries; i++) {
        ZipEntry *pNewEntry;
        int padding = 0;

        pEntry = pZin->getEntryByIndex(i);
        if (pEntry == nullptr) {
            fprintf(stderr, "ERROR: unable to retrieve entry %d\n", i);
            return 1;
        }

        if (pEntry->isCompressed()) {
            /* copy the entry without padding */
            //printf("--- %s: orig at %ld len=%ld (compressed)\n",
            //    pEntry->getFileName(), (long) pEntry->getFileOffset(),
            //    (long) pEntry->getUncompressedLen());

        } else {
            /*
             * Copy the entry, adjusting as required.  We assume that the
             * file position in the new file will be equal to the file
             * position in the original.
             */
            long newOffset = pEntry->getFileOffset() + bias;
            const int alignTo = getAlignment(pageAlignSharedLibs, alignment, pEntry);
            padding = (alignTo - (newOffset % alignTo)) % alignTo;

            //printf("--- %s: orig at %ld(+%d) len=%ld, adding pad=%d\n",
            //    pEntry->getFileName(), (long) pEntry->getFileOffset(),
            //    bias, (long) pEntry->getUncompressedLen(), padding);
        }

        status = pZout->add(pZin, pEntry, padding, &pNewEntry);
        if (status != NO_ERROR)
            return 1;
        bias += padding;
        //printf(" added '%s' at %ld (pad=%d)\n",
        //    pNewEntry->getFileName(), (long) pNewEntry->getFileOffset(),
        //    padding);
    }

    return 0;
}

/*
 * Process a file.  We open the input and output files, failing if the
 * output file exists and "force" wasn't specified.
 */
static int process(const char *inFileName, const char *outFileName,
                   int alignment, bool force, bool pageAlignSharedLibs) {
    ZipFile zin, zout;

    //printf("PROCESS: align=%d in='%s' out='%s' force=%d\n",
    //    alignment, inFileName, outFileName, force);

    /* this mode isn't supported -- do a trivial check */
    if (strcmp(inFileName, outFileName) == 0) {
        fprintf(stderr, "Input and output can't be same file\n");
        return 1;
    }

    /* don't overwrite existing unless given permission */
    if (!force && access(outFileName, F_OK) == 0) {
        fprintf(stderr, "Output file '%s' exists\n", outFileName);
        return 1;
    }

    if (zin.open(inFileName, ZipFile::kOpenReadOnly) != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' as zip archive\n", inFileName);
        return 1;
    }
    if (zout.open(outFileName,
                  ZipFile::kOpenReadWrite | ZipFile::kOpenCreate | ZipFile::kOpenTruncate)
            != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' as zip archive\n", outFileName);
        return 1;
    }

    int result = copyAndAlign(&zin, &zout, alignment, pageAlignSharedLibs);
    if (result != 0) {
        printf("zipalign: failed rewriting '%s' to '%s'\n",
               inFileName, outFileName);
    }
    return result;
}

/*
 * Verify the alignment of a zip archive.
 */
static int verify(const char *fileName, int alignment, bool verbose, bool pageAlignSharedLibs) {
    ZipFile zipFile;
    bool foundBad = false;

    if (verbose)
        printf("Verifying alignment of %s (%d)...\n", fileName, alignment);

    if (zipFile.open(fileName, ZipFile::kOpenReadOnly) != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' for verification\n", fileName);
        return 1;
    }

    int numEntries = zipFile.getNumEntries();
    ZipEntry *pEntry;

    for (int i = 0; i < numEntries; i++) {
        pEntry = zipFile.getEntryByIndex(i);
        if (pEntry->isCompressed()) {
            if (verbose) {
                printf("%8ld %s (OK - compressed)\n",
                       (long) pEntry->getFileOffset(), pEntry->getFileName());
            }
        } else {
            long offset = pEntry->getFileOffset();
            const int alignTo = getAlignment(pageAlignSharedLibs, alignment, pEntry);
            if ((offset % alignTo) != 0) {
                if (verbose) {
                    printf("%8ld %s (BAD - %ld)\n",
                           (long) offset, pEntry->getFileName(),
                           offset % alignTo);
                }
                foundBad = true;
            } else {
                if (verbose) {
                    printf("%8ld %s (OK)\n",
                           (long) offset, pEntry->getFileName());
                }
            }
        }
    }

    if (verbose)
        printf("Verification %s\n", foundBad ? "FAILED" : "succesful");

    return foundBad ? 1 : 0;
}

/**
 *
 * Align a zip file. This function will fail if out_filename exists and force is 0
 * It is recommended that you verify the out_filename using zipalign_is_aligned() upon success
 *
 * @return 1 on success, otherwise returns 0 on failure
 *
 */
extern "C"
int alignZip(const char *in_filename, const char *out_filename, int alignment, int force, bool pageAlignSharedLibs) {
    if(!in_filename || !out_filename) {
        return 0;
    }
    return process(in_filename, out_filename, alignment, force != 0, pageAlignSharedLibs) ? 0 : 1;
}


/**
 *
 * Checks if a zip file is aligned or not
 *
 * @return 1 if aligned, otherwise returns 0 on failure or unaligned
 *
 */
extern "C"
int isAlignedZip(const char *filename, int alignment, bool pageAlignSharedLibs) {
    if(!filename) {
        return 0;
    }
    return verify(filename, alignment, false, pageAlignSharedLibs) ? 0 : 1;
}
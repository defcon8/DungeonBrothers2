#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <zlib.h>
#include "../include/cdecompress.h"

// TODO: Improve error reporting by showing these errors in the user interface
static void logZlibError(int error)
{
    switch (error)
    {
        case Z_MEM_ERROR:
            printf( "Out of memory while (de)compressing data! \n" );
            //exit(0);
            break;
        case Z_VERSION_ERROR:
            printf(  "Incompatible zlib version! \n" );
            //exit(0);
            break;
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
            printf( "Incorrect zlib compressed data! \n" );
            //exit(0);
            break;
        default:
            printf( "Unknown error while (de)compressing data! \n" );
            //exit(0);
    }
}

char* cDecompress::decompress( const std::string pString, int expectedSize )
{
    int bufferSize = expectedSize;
    int ret;
    z_stream strm;
    char *out = (char *) malloc(bufferSize);

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *) pString.data();
    strm.avail_in = pString.length();
    strm.next_out = (Bytef *) out;
    strm.avail_out = bufferSize;

    ret = inflateInit2(&strm, 15 + 32);

    if (ret != Z_OK)
    {
        logZlibError(ret);
        free(out);
        return NULL;
    }

    do
    {
        ret = inflate(&strm, Z_SYNC_FLUSH);

        switch (ret)
        {
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                logZlibError(ret);
                free(out);
                return NULL;
        }

        if (ret != Z_STREAM_END)
        {
            out = (char *) realloc(out, bufferSize * 2);

            if (!out)
            {
                inflateEnd(&strm);
                logZlibError(Z_MEM_ERROR);
                free(out);
                return NULL;
            }

            strm.next_out = (Bytef *)(out + bufferSize);
            strm.avail_out = bufferSize;
            bufferSize *= 2;
        }
    }
    while (ret != Z_STREAM_END);

    if (strm.avail_in != 0)
    {
        logZlibError(Z_DATA_ERROR);
        free(out);
        return NULL;
    }

    //const int outLength = bufferSize - strm.avail_out;
    inflateEnd(&strm);

    return out;
}

cDecompress::cDecompress(){
	
}

cDecompress::~cDecompress(){
	
}

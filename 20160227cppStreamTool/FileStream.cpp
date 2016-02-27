#include "FileStream.h"


FileStream::FileStream()
{
    file = NULL;
}

FileStream::~FileStream()
{
    close();
}

int FileStream::open(const char *filePath)
{
    this->filePath = filePath;
    if(NULL == (file = fopen(filePath, "a+"))) {
        return -1;
    }
    
    seek(STREAM_END, 0);
    fileSize = ftell(file);
    seek(STREAM_SET, 0);
    
    return 0;
}

int FileStream::close()
{
    if(file) {
        fclose(file);
        file = NULL;
    }
    return 0;
}

int FileStream::read(char *buf, int len, bool wait)
{
    if(NULL == buf) {
        seek(STREAM_CUR, len);
        return len;
    }
    return (int)fread(buf, 1, len, file);
}

int FileStream::write(const char *buf, int len)
{
    return (int)fwrite(buf, 1, len, file);
}

int FileStream::end()
{
    return fileSize == ftell(file);//feof(file);
}

int FileStream::seek(StreamPos pos, int offset)
{
    fseek(file, offset, pos);
    return 0;
}

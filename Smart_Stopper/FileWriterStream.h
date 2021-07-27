#ifndef FILEWRITERSTREAM_H
#define FILEWRITERSTREAM_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef ushort bool;
#define true 1;
#define false 0;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;// it thoes not free the original string
}


typedef struct FileStream FileStream, *fsp;


struct FileStream{

char *_buffer;
char filename[256];
//member functions in C 
fsp (*Print)(FileStream *);
fsp (*Write)(FileStream *,const char *s2);
fsp (*Flush)(FileStream *);

};
//-----------------------------------------------------------------------
// dont ever call this functions outside of this file
fsp fsp_Print(FileStream* self)
{
printf("%s",self->_buffer);
return self;
}

fsp fsp_Write(FileStream* self,const char *s2)
{
char* old=self->_buffer;
self->_buffer=concat(self->_buffer,s2);
if(old!=NULL)
    free(old);
//free s2;
return self;
}

fsp fsp_Flush(FileStream* self)
{
    FILE* file = fopen(self->filename, "a" ); // this will append the file
    //printf("%s",self->_buffer);
    fputs(self->_buffer, file);
    printf("%lu byte was writen to file %s\n",strlen(self->_buffer),self->filename);
    fclose(file);
return self;
}
//end of critical code
//-----------------------------------------------------------------------
// This is like a construct for FileStream
FileStream* MakeFileStream(const char* filename)
{
    FileStream* f= malloc(sizeof(FileStream));
    f->_buffer=malloc(1);
    strcpy(f->_buffer, "");
    strcpy(f->filename,filename);
    f->Print=fsp_Print;
    f->Write=fsp_Write;
    f->Flush=fsp_Flush;

    return f;
}
#endif

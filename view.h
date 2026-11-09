#ifndef VIEW_H
#define VIEW_H
#include"types.h"
typedef unsigned int uint;
typedef struct _TagInfo
{
    /* mp3 file info*/
    char* mp3_fname;
    FILE* fptr;
}TagInfo;

//function declaration
OperationType check_operation_type(char opt);

//filename validation
Status open_files(TagInfo *vinfo);
Status read_and_validate_args(char* argv[],TagInfo *vinfo);
Status do_view(TagInfo *vinfo);
uint get_size(unsigned char *size_buffer);


#endif



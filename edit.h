#ifndef EDIT_H
#define EDIT_H
#include"types.h"


typedef struct _EditInfo
{
   char *mp3_fname;
   FILE *fptr;

   char *tag_to_edit;
   char *newdata;

   char *temp_mp3_fname;
   FILE *temp_fptr;

}EditInfo;

OperationType check_operationtype(char opt);
Status read_and_validate_edit_args(char *argv[],EditInfo *einfo);
Status get_tag_to_edit(char e_tag,EditInfo *einfo);
Status open_edit_files(EditInfo *einfo);
void do_edit(EditInfo *einfo);
void convet_little_to_big(int size, unsigned char *new_size);
uint get_e_size(unsigned char *size_buffer);
void remove_rename(EditInfo *einfo);

#endif
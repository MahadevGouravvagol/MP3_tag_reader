#include<stdio.h>
#include<string.h>
#include"types.h"
#include"edit.h"

Status read_and_validate_edit_args(char *argv[],EditInfo *einfo)
{
    if(get_tag_to_edit(argv[2][1],einfo)==e_failure)
    {
        
        return e_failure;
    }
    char* dot=strchr(argv[4],'.');
    if(dot==NULL || strcmp(dot,".mp3")!=0)
    {
        printf("Error: mp3 filename must be .mp3\n");
        return e_failure;
    }
    einfo->newdata=argv[3];
    einfo->mp3_fname=argv[4];
    einfo->temp_mp3_fname="temp.mp3";
    /*open the files*/
    if(open_edit_files(einfo)==e_failure)
    {
        printf("Error:unable to access\n");
        return e_failure;
    }
    /* check the signature as a ID3 OR NOT*/
    char buffer[3];
    fread(buffer,3,1,einfo->fptr);
    buffer[3]=0;

    if(strcmp(buffer,"ID3")!=0)
    {
        printf("Error: signature of mp3 file doesnt match\n");
        return e_failure;
    }
    /*rewind the offset to 0 pos*/
    rewind(einfo->fptr);
    return e_success;


}

Status open_edit_files(EditInfo *einfo)
{
    einfo->fptr=fopen(einfo->mp3_fname,"rb");
    if(einfo->fptr==NULL)
    {
        return e_failure;
    }

    einfo->temp_fptr=fopen(einfo->temp_mp3_fname,"wb");
    if(einfo->temp_fptr==NULL)
    {
        return e_failure;
    }

    return e_success;
}

uint get_e_size(unsigned char *size_buffer)
{
    // convert big endiness to little
    for(int i=0;i<2;i++)
    {
       unsigned char temp = size_buffer[i];
       size_buffer[i] = size_buffer[3-i]; 
       size_buffer[3-i] = temp;
    }
    
    // get size
    uint size;
    unsigned char *ptr = (unsigned char *)&size;
    for(int i=0;i<4;i++)
    {
        ptr[i]=size_buffer[i];
    }
    return size;
}

//-------------------------------------------------------------------------------//

void convet_little_to_big(int size, unsigned char *new_size)
{
    unsigned char *ptr = (unsigned char *)&size;
    int i;
    for(i=0;i<4;i++)
    {
        new_size[i] = ptr[3-i];
    }
    //new_size[i]='\0';
}


void do_edit(EditInfo *einfo)
{
    char tag_buffer[5];
    unsigned char size_buffer[4];
    unsigned char new_size[4];
    uint size;

    // copy 10 bytes of header
    char header_buffer[10];
    fread(header_buffer,10,1,einfo->fptr);
    fwrite(header_buffer,10,1,einfo->temp_fptr);

    for(int i=0;i<6;i++)
    {
        // read nd write 4 bytes for file for tags
        fread(tag_buffer,4,1,einfo->fptr);
        fwrite(tag_buffer,4,1,einfo->temp_fptr);
        tag_buffer[4]='\0';

        // read 4 bytes for size for song einfo
        fread(size_buffer,4,1,einfo->fptr);
        size = get_e_size(size_buffer);
        convet_little_to_big(size,size_buffer);

        if(strcmp(tag_buffer,einfo->tag_to_edit) == 0)
        {
            // add new info and new size
            convet_little_to_big((strlen(einfo->newdata)+1),new_size);
            // for(int i = 0; i < 4; i++)
            // {
            //     printf("%02X ", new_size[i]);
            // }
            fwrite(new_size,1,4,einfo->temp_fptr);

            char flag_buffer[3];
            fread( flag_buffer,3,1,einfo->fptr);
            fwrite(flag_buffer,3,1,einfo->temp_fptr);

            fwrite(einfo->newdata,strlen(einfo->newdata),1,einfo->temp_fptr);

            fseek(einfo->fptr,size-1,SEEK_CUR);
            break;
        }

        fwrite(size_buffer,4,1,einfo->temp_fptr);
        
        // read 3 bytes (2 bytes for flag and 1 bytes for null char)
        char flag_buffer[3];
        fread( flag_buffer,3,1,einfo->fptr);
        fwrite(flag_buffer,3,1,einfo->temp_fptr);
        
        char info_buffer[size];
        // read size-1 bytes of song vinfo
        fread(info_buffer,size-1,1,einfo->fptr);
        fwrite(info_buffer,size-1,1,einfo->temp_fptr);
    }
    char data;
    while(fread(&data,1,1,einfo->fptr) == 1)
    {
        fwrite(&data,1,1,einfo->temp_fptr);
    }
    printf("[ success ] edited\n");
    fclose(einfo->fptr);
    fclose(einfo->temp_fptr);
    remove_rename(einfo);
    return;
}




   
Status get_tag_to_edit(char e_tag,EditInfo *einfo)
{
    switch(e_tag)
    {
        case 't':
            einfo->tag_to_edit="TIT2";
            break;
        case 'a':
            einfo->tag_to_edit="TPE1";
            break;
        case 'A':
            einfo->tag_to_edit="TALB";
            break;
        case 'c':
            einfo->tag_to_edit="TCON";
            break;
        case 'y':
            einfo->tag_to_edit="TYER";
            break;
        case 'm':
            einfo->tag_to_edit="COMM";
            break;
        default:
            return e_failure;

    }
    return e_success;
    
}
void remove_rename(EditInfo *einfo)
{
    remove(einfo->mp3_fname);
    rename(einfo->temp_mp3_fname,einfo->mp3_fname);
}
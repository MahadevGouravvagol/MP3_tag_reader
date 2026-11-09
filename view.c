#include<stdio.h>
#include<string.h>
#include"view.h"
#include"types.h"



static const char* tag[] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};
int num=1;

Status read_and_validate_args(char *argv[],TagInfo *vinfo)
{
    char *dot=strchr(argv[2],'.');
    if(dot==NULL || strcmp(dot,".mp3")!=0 )
    {
        printf("Error: file name must be .mp3\n");
        return e_failure;
    }
    vinfo->mp3_fname=argv[2];
    if(open_files(vinfo)==e_failure)
    {
        printf("Error: file was not opened\n");
        return e_failure;
    }
    char buffer[3];
    fread(buffer,3,1,vinfo->fptr);
    buffer[3]=0;
    printf("%s\n",buffer);

    /* check 1st 3bytes is ID3 or not*/
    if(strcmp(buffer,"ID3")!=0)
    {
        printf("ERROR: signature must be ID3\n");
        return e_failure;
    }

    /* set the offset to 10th pos*/
    fseek(vinfo->fptr,10,SEEK_SET);
    return e_success;


}
Status open_files(TagInfo *vinfo)
{
    vinfo->fptr=fopen(vinfo->mp3_fname,"rb");
    if(vinfo->fptr==NULL)
    {
        return e_failure;
    }
    return e_success;
}
uint get_size(unsigned char* size_buffer)
{
    /*convert big endian to little endian*/
    for(int i=0;i<2;i++)
    {
        unsigned char temp=size_buffer[i];
        size_buffer[i]=size_buffer[3-i];
        size_buffer[3-i]=temp;
    }
    uint size;
    unsigned char *ptr=(unsigned char*)&size;
    for(int i=0;i<4;i++)
    {
        ptr[i]=size_buffer[i];
    }
    return size;

}
Status do_view(TagInfo *vinfo)
{
    char tag_buffer[5];
    unsigned char size_buffer[4];


    printf("-----------------------------------------------------------\n");
    printf("SL.NO |  TAG \t|  INFORMATION\n");
    printf("-----------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
        /*read 4 bytes from file for tg*/
        fread(tag_buffer,4,1,vinfo->fptr);
        tag_buffer[4]='\0';

        /*read 4 bytes of size*/
        fread(size_buffer,4,1,vinfo->fptr);
       // printf("%s\n",tag_buffer);
       // printf("%s\n",size_buffer);

       /*convert big endian littel endian*/ 
       uint size;
       size=get_size(size_buffer);

       /* skip 3 bytes flag(2 bytes)+ startin null character of meta data*/
       fseek(vinfo->fptr,3,SEEK_CUR);

       /* read meta data from file*/
       char meta_data[size];
       
       fread(meta_data,size-1,1,vinfo->fptr);
       meta_data[size-1]='\0';


       /*compare the tag buffer with the tags*/
       for(int i=0;i<6;i++)
       {
        if(!strcmp(tag_buffer,tag[i]))
        {
            printf("%d     |  %s  \t|  %s\n",num++,tag_buffer,meta_data);
            break;
        }
       }

    }
    printf("-----------------------------------------------------------\n");
    fclose(vinfo->fptr);
    return e_success;
}

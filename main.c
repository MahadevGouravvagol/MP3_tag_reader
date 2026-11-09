#include<stdio.h>
#include"view.h"
#include"types.h"
#include"edit.h"

int main(int argc,char *argv[])
{
    TagInfo vinfo;
    EditInfo einfo;
    if(argc==3 && check_operation_type(argv[1][1])==e_view)
    {
        if(read_and_validate_args(argv, &vinfo)==e_failure)
        {
            printf("to viewinfo: ./a.out -v filename.mp3\n");
            return e_failure;
        }
        else
        {
            do_view(&vinfo);
        }
    }
    else if(argc==5 && check_operation_type(argv[1][1])==e_edit)
    {
        if(read_and_validate_edit_args(argv,&einfo)==e_failure)
        {
            printf("to edit: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
            return e_failure;
        }
        else
        {
            do_edit(&einfo);

        }
    }
       else if(check_operation_type(argv[1][1])==e_help)
   {
        printf("\n1.  -v  -->   to view mp3 file contents\n");
        printf("2.  -e  -->  to edit mp3 file contents\n");
        printf("\t2.1  -t  -->  to edit song title\n");
        printf("\t2.2  -a  -->  to edit artist name\n");
        printf("\t2.3  -A  -->  to edit album name\n");
        printf("\t2.4  -y  -->  to edit year\n");
        printf("\t2.5  -m  -->  to edit Content type / Genre\n");
        printf("\t2.6  -c  -->  to edit Comment\n\n");
   }
   else 
   {
        printf("\n\t--- Invalid input ---\n");
        printf("Usage : \n");
        printf("To vinfo : \t./a.out  -v  filename.mp3\n");
        printf("To Edit : \t./a.out  -e  -t/-a/-A/-m/-y/-c  filename.mp3\n");
        printf("To help menu : \t./a.out  --help\n\n");
   }
   

    return 0;
}
OperationType check_operation_type(char opt)
{
    if(opt=='v')
    {
        return e_view;
    }
    else if(opt=='e')
    {
        return e_edit;
    }
    else if(opt=='h')
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}
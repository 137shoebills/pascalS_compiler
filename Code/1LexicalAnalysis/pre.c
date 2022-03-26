#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main(int argc, char *args[]){
    FILE*fp;
    char *s,*sp;
    int file_size, read_size;
    if(argc != 2){
        fprintf(stdout, "usage: change filename\n");
        exit(EXIT_FAILURE);
    }
    if((fp = fopen(args[1], "rb+")) == NULL){
        fprintf(stderr, "Can't open %s\n", args[1]);
        exit(EXIT_FAILURE);
    }
    fseek(fp,0L,SEEK_END);//文件指针移动到文件末尾
    file_size = ftell(fp);
    rewind(fp);//文件指针移动到文件开头
    //为字符数组分配内存，然后读入数据
    s = (char *)calloc(file_size,1);
    read_size = fread(s,1, file_size, fp);
    fclose(fp);
    if(read_size != file_size){
        fprintf(stdout, "Can't read all massage\n");
        exit(EXIT_FAILURE);
    }

    //转换字母
    sp = s;
    while(*sp){
        if(isupper(*sp))
            *sp = tolower(*sp);
        sp++;
    }
    //先清除源文件数据，然后向文件写入字符数组数据
    if((fp = fopen(args[1], "wb+")) == NULL){
        fprintf(stderr, "Can't open %sin", args[1]);
        exit(EXIT_FAILURE);
    }
    fwrite(s,1, read_size, fp);
    fclose(fp);
    return 0;
}



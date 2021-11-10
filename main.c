#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEM_SIZE 512

char *load_bfile(const char *path, int *size){

    FILE *fp = fopen(path, "r");

    if(fp == NULL)
        return NULL;

    fseek(fp, 0L, SEEK_END);

    long buff_size = ftell(fp);
    *size = buff_size;
    char *buffer = malloc(buff_size);

    fseek(fp, 0L, SEEK_SET);

    fread(buffer, 1, buff_size, fp);

    fclose(fp);

    return buffer;

}

int find_closing_bracket(char *buffer, int buff_size,int pos){
    
    int c = 0;
    for(int i = pos; i < buff_size; i++){
        if(buffer[i] == '[')
            c++;

        if(buffer[i] == ']' && c != 0)
            return i;
        
        if(buffer[i] == ']')
            c--;
    }

    return -1;
}

char *slice_code(char *buff, int buff_size, int start, int end){
    char *f = malloc(end-start);
    strncpy(f, buff+start+1, end-start);
    f[end-start-1] = '\0';
    return f;
}

int exec_bf(char *buff, int buff_size, char *mem, int pc){

    for(int i = 0; i < buff_size; i++){
        switch (buff[i])
        {
            case '>': pc++; break;
            case '<': pc--; break;
            case '+': mem[pc]++; break;
            case '-': mem[pc]--; break;
            case '.': putchar(mem[pc]); break;
            case ',': mem[pc] = getchar(); break;
            case '[': 
                int end = find_closing_bracket(buff, buff_size, i);
                char *slice = slice_code(buff, buff_size, i, end);

                while (mem[pc] != 0)
                    exec_bf(slice, end-i, mem, pc);
                
                i = end;
                
                free(slice);
                break;
            
            default:
                break;

        }

        if(pc < 0 || pc > MEM_SIZE - 1)
            return -1;
    }
}

int main(int argc, char const *argv[])
{
    if(argc != 2){
        printf("Invalid arguments\n");
        return -1;
    }
    int size;
    char *buff = load_bfile(argv[1], &size);

    if(buff == NULL){
        printf("Failed to load file: %s\n", argv[1]);
        return -1;
    }

    char mem[MEM_SIZE] = {0};

    exec_bf(buff, size, mem, 0);

    free(buff);
    putchar('\n');

    return 0;
}
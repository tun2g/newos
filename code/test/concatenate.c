#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_READ 1

#define stdin 0
#define stdout 1
char a[255];
int main() {
    char buffer[100];
    char buffer2[100];
    int i;
    int write;
    char fileName[256];
    char fileName2[256];
    int length;
    int length2;
    int fileid;
    int fileid2;
    int len;
    int read;
    int read2;

    PrintString("Nhap do dai cua ten file 1:");
    length =ReadNum();
    PrintString("Nhap ten file:");
    ReadString(fileName,length+1);
    
    PrintString("Nhap do dai cua ten file 2: ");
    length2 =ReadNum();
    PrintString("Nhap ten file: ");
    ReadString(fileName2,length2);
    fileid2 = Open(fileName2, MODE_READ);

    read = Read(buffer, 50, fileid2);

    len = 0;

    
    fileid=Open(fileName,MODE_READWRITE);
    read2= Read(buffer2,50,fileid);

    while (buffer[len] != '\0') ++len;
    PrintString(buffer);
    PrintString(buffer2);
    Seek(-1,fileid);
    write=Write(buffer,len,fileid);
    Close(fileid);
    Close(fileid2);
    
    Halt();
}
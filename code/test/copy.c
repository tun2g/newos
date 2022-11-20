#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_READ 1

#define stdin 0
#define stdout 1
char a[255];
int main() {
    char buffer[100];
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
    int lenTxt;
    PrintString("Nhap do dai cua ten file 1:");
    length =ReadNum();
    PrintString("Nhap ten file:");
    ReadString(fileName,length+1);
    
    PrintString("Nhap do dai cua ten file 2: ");
    length2 =ReadNum();
    PrintString("Nhap ten file: ");
    ReadString(fileName2,length2+1);
    fileid2 = Open(fileName2, MODE_READ);

    lenTxt=Seek(-1,fileid2);
    Seek(0,fileid2);
    read = Read(buffer,lenTxt, fileid2);

    PrintString("Noi dung: ");
    PrintString(buffer);
    PrintString("\n");
    
    CreateFile(fileName);
    fileid=Open(fileName,MODE_READWRITE);
    write=Write(buffer,lenTxt,fileid);
    Close(fileid);
    Close(fileid2);
    
    Halt();
}
#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_READ 1

#define stdin 0
#define stdout 1

int main() {
    char buffer[100];
    int i;
    int write;
    char fileName[256];
    int length;
    int fileid;
    int len;
    int read;
    PrintString("Nhap do dai cua ten file:");
    length =ReadNum();
    PrintString("Nhap ten file:");
    ReadString(fileName,length);
    fileid = Open(fileName, MODE_READ);

    read = Read(buffer, 50, fileid);

    len = 0;
    while (buffer[len] != '\0') ++len;
    PrintString("Noi dung: ");
    PrintString(buffer);
    PrintString("\n");
    Close(fileid);
    Halt();
}
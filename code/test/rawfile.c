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
    //cat
    // PrintString("Nhap do dai cua ten file:");
    // length =ReadNum();
    // PrintString("Nhap ten file:");
    // ReadString(fileName,length);
    // fileid = Open(fileName, MODE_READ);

    // read = Read(buffer, 50, fileid);

    // len = 0;
    // while (buffer[len] != '\0') ++len;
    // PrintString("Noi dung: ");
    // PrintString(buffer);
    // PrintString("\n");
    // Close(fileid);
    
    // Remove("tung.txt");

    
}
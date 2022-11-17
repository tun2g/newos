#include "syscall.h"

#define MODE_READWRITE 0
#define MODE_READ 1

#define stdin 0
#define stdout 1

int main() {
    char buffer[100];
    int i;
    int write;

    int fileid = Open("tungne.txt", MODE_READWRITE);
    int read = Read(buffer, 50, fileid);
    int len = 0;
    while (buffer[len] != '\0') ++len;

    PrintString("Read ");
    PrintNum(len);
    PrintString(" characters: ");
    PrintString(buffer);
    PrintString("\n");
    // Write to the same file
    Seek(0,fileid);
    write = Write("hi", 2, fileid);
    PrintString("Write ");
    PrintNum(write);
    PrintString(" characters: ");
    PrintString(buffer);
    PrintString("\n");
    Close(fileid);
    
    Remove("tung.txt");

    Halt();
}
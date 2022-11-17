#include "syscall.h"

int main(){
    char filename[256];
    int length;

    PrintString("Nhap do dai cua ten file:");
    length =ReadNum();
    PrintString("Nhap ten file:");
    ReadString(filename,length);

    if(CreateFile(filename) == 0 ){
        PrintString("File: ");
        PrintString(filename);
        PrintString(" duoc tao thanh cong. \n");
    }else{

        PrintString(" Tao file that bai. \n");
    }

    Halt();
}
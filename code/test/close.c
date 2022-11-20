#include "syscall.h"

int main() {
    char fileName[] = "trinhtrinh";
    int id;

    if ((id = Open(fileName, 0)) != -1) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" duoc mo thanh cong!\n");
        PrintString("Id: ");
        PrintNum(id);
        PrintString("\n");

    } else
        PrintString("Mo file that bai \n");
    

    if(Close(id)==0)
        PrintString("Dong file thanh cong! \n");
    else
        PrintString("Dong file that bai! \n");
    
    
    

    Halt();

}
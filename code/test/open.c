#include "syscall.h"

int main() {
    char fileName[] = "tung";
    int length, id;
    int i;

    for (i = 0; i < 20; i++) {
        if ((id = Open(fileName, 0)) != -1) {
            PrintString("File ");
            PrintString(fileName);
            PrintString(" duoc mo thanh cong!\n");
            PrintString("Id: ");
            PrintNum(id);
            PrintString("\n");
            
        } else
            PrintString("Mo file that bai \n");
    }

    Halt();
}
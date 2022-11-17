#include "syscall.h"

int main() {
    char a[256];
    ReadString(a, ReadNum());

    Halt();
}
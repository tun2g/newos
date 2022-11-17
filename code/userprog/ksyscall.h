/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#define TAB ((char)9)
#define SPACE ((char)' ')

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysReadNum(){
  char* num = new char[255]; // tạo vùng đệm để lưu số nguyên

  char c = kernel->synchConsoleIn->GetChar(); // đọc kí tự đầu tiên
  if(c==EOF){
    return 0; // dừng nếu đây là kí tự dừng
  }
  int i=0,z=1;
  while(c!=((char)10)){ //đọc những kí tự tiếp theo đến khi gặp kí tự dừng

    if(i==0 && (c== SPACE||c==TAB) ){ 
     c = kernel->synchConsoleIn->GetChar();
      continue;
    }
    else if(i== 0 && c=='-') // trường hợp số âm
      z=-1;
    else
        num[i++]=c; // lưu vào vùng đệm

    c = kernel->synchConsoleIn->GetChar(); //đọc kí tự tiếp theo
  }
  int s=0;
  int LENGTH =strlen(num);
  DEBUG(dbgSys,"LENGTH="<<LENGTH);
  
  for(int i=0;i<LENGTH;i++){ // chuyển từ string sang int
     if(num[i]<'0' || num[i]>'9') // trường hợp nhập không phải dạng số
      return 0;

    s=s*10+(num[i]-'0');    
  }

  s*=z; 
  bool check_range = true ;
//trường hợp nhập số ngoài giớí hạn của int
  if(LENGTH>10)
    check_range=false;

  if(LENGTH ==10){ // nhập số dương >=2147483648
    if(strcmp(num,"2147483648")>=0  && z == 1)
      check_range=false;

    if(strcmp(num,"2147483648")> 0  && z == (-1))//nhập số âm < -2147483648
      check_range=false;

  }

  if (check_range==false){
    delete[] num;
    return 0;
  }
  
  delete[] num;
  return s;
}

void SysPrintNum(int number){
  char * _num =new char [12];
  if(number==0)
     kernel->synchConsoleOut->PutChar('0'); 
  
 if(number == INT32_MIN){ // trường hợp số bé nhất
  kernel->synchConsoleOut->PutChar('-'); 
    for(int i=0;i<10;i++){
      kernel->synchConsoleOut->PutChar("2147483648"[i]);// in số bé nhất ra màn hình
    }
    kernel->synchConsoleOut->PutChar('\n'); // in kí tự xuống dòng
    return;
 }
  if(number<0){ // trường hợp số âm
    kernel->synchConsoleOut->PutChar('-');
    number = -number;
  }
  

  int n =0;
  while(number>0){ //ghi từng kí tự của số ra vùng đệm
    _num[n++]=((char)(number%10+'0'));
    number = number/10;
  }
  for(int i=n-1;i>=0;i--){ // in từng kí tự ra màn hình
    kernel->synchConsoleOut->PutChar(_num[i]);
  }
}

char SysReadChar() { 
    return kernel->synchConsoleIn->GetChar();//đọc ký tự từ bàn phím 
  }

void SysPrintChar(char character) {
    kernel->synchConsoleOut->PutChar(character);//in ra kí tự
}

int SysRandomNum() { return random(); }

char* SysReadString(int length) {
    char* buffer = new char[length + 1];
    for (int i = 0; i < length; i++) {
      // đọc từng ký tự từ bàn phím
        buffer[i] = SysReadChar();
    }
    buffer[length] = '\0';

    DEBUG(dbgSys,"String ="<<buffer);
    return buffer;
}
char* ReadFromMem(int addr,int convert_length=-1)
{
    int length = 0;
    bool stop = false;
    char* str;

    do {
        int oneChar;
        kernel->machine->ReadMem(addr + length, 1, &oneChar);
        length++;
        // if convert_length == -1, we use '\0' to terminate the process
        // otherwise, we use convert_length to terminate the process
        stop = ((oneChar == '\0' && convert_length == -1) ||
                length == convert_length);
    } while (!stop);

    str = new char[length];
    for (int i = 0; i < length; i++) {
        int oneChar;
        kernel->machine->ReadMem(addr + i, 1,
                                 &oneChar);  // copy characters to kernel space
        str[i] = (unsigned char)oneChar;
    }
    return str;
}
void SysPrintString(char * str) {
    for (int i = 0; i < int(strlen(str)); i++) {
        kernel->synchConsoleOut->PutChar(str[i]); // đưa từng phần tử của mảng ra màn hình
    }
}
//-----------------------------------
int SysCreateFile(char * filename){
  int result;
  int length = strlen(filename);

  DEBUG(dbgSys, filename);
  DEBUG(dbgSys, length);

  if(length ==0){
    DEBUG(dbgSys,"Create file fail 1 \n");
    result =-1;
  }
  else if(filename == NULL){
    DEBUG(dbgSys,"Create file fail 2 \n");
    result = -1;
  }
  else{
      if (kernel->fileSystem->Create(filename)) {
        result = 0;
      }else{
        result = -1;
      }
  }
  return result;
}

int SysOpen(char*fileName,int type){

  DEBUG(dbgSys, "SysOpen \n");
  if(type  !=0 && type != 1){
      return -1;
  }
  
  int id = kernel->fileSystem->Open(fileName,type);

  DEBUG(dbgSys, "id = "<<id);

  return id;

}
int SysClose(int id) { return kernel->fileSystem->Close(id); }

int SysRead(char* buffer, int charCount, int fileId) {
    if (fileId == 0) {
        return kernel->synchConsoleIn->GetString(buffer, charCount);
    }
    return kernel->fileSystem->Read(buffer, charCount, fileId);
}

int SysWrite(char* buffer, int charCount, int fileId) {
    if (fileId == 1) {
        return kernel->synchConsoleOut->PutString(buffer, charCount);
    }
    return kernel->fileSystem->Write(buffer, charCount, fileId);
}

int SysSeek(int seekPos, int fileId) {
    if (fileId <= 1) {
        DEBUG(dbgSys, "\nCan't seek in console");
        return -1;
    }
    return kernel->fileSystem->Seek(seekPos, fileId);
}
int SysRemove(char* name) {
    return kernel->fileSystem->Remove(name);
}

#endif /* ! __USERPROG_KSYSCALL_H__ */

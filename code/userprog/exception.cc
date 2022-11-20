// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------
void inc_PC_register(){ // tăng thanh ghi PC
	DEBUG(dbgSys,"\n Increase PC register.\n");
	// registers[PrevPCReg] = registers[PCReg];	
	kernel->machine->WriteRegister(PrevPCReg,
				 kernel->machine->ReadRegister(PCReg));
   //  registers[PCReg] = registers[NextPCReg];
	kernel->machine->WriteRegister(PCReg, 
				kernel->machine->ReadRegister(PCReg) + 4);
    // registers[NextPCReg] = pcAfter;
	kernel->machine->WriteRegister(NextPCReg, 
				kernel->machine->ReadRegister(PCReg)+4);

}

void WriteToMem(char* str, int addr, int convert_length = -1) {
    int length = (convert_length == -1 ? strlen(str) : convert_length);
    for (int i = 0; i < length; i++) {
        kernel->machine->WriteMem(addr + i, 1,
                                  str[i]);  
    }
    kernel->machine->WriteMem(addr + length, 1, '\0');
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	
	case NoException: //Trường hợp không có exception
		kernel->interrupt->setStatus(SystemMode); 
		DEBUG(dbgSys, "Return to system mode\n");
		break;
	// Các trường hợp exception lỗi
	case PageFaultException:
	case ReadOnlyException:
	case BusErrorException:
	case AddressErrorException:
	case OverflowException:
	case NumExceptionTypes:
		cerr<<"Error:"<<which<<"\n"; // Thông báo ra màn hình
		SysHalt();
	case SyscallException:
		switch (type)
		{
		
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			
			return inc_PC_register();
		case SC_ReadNum:
		{
			int result = SysReadNum(); // SysReadNum(): đọc số được nhập màn hình		
			DEBUG(dbgSys, "Result = " << result<< "\n");
			kernel->machine->WriteRegister(2, (int)result); // ghi vào thanh ghi số 2
			return inc_PC_register();
		}
		case SC_PrintNum:{

			int number = kernel->machine->ReadRegister(4);// Đọc số ở thanh ghi số 4
    		SysPrintNum(number); //SysPrintNum(number): in số ra ngoài màn hình
			return inc_PC_register();
			
		}
		//đọc ký tự
		case SC_ReadChar:{
			char result = SysReadChar();
    		kernel->machine->WriteRegister(2, (int)result);//trả kí tự về thanh ghi số 2
    		return inc_PC_register();
		}	
		case SC_PrintChar:{
			char character = (char)kernel->machine->ReadRegister(4);// nhận character từ thanh ghi 4
    		SysPrintChar(character);
    		return inc_PC_register();
		}	
		case SC_RandomNum:{
			int result;
    		result = SysRandomNum();//trả về con số được random
    		kernel->machine->WriteRegister(2, result);//đưa kết quả vào thanh ghi số 2
    		return inc_PC_register();
		}
		case SC_ReadString:{
			int memPtr = kernel->machine->ReadRegister(4);  // đọc tham số là địa chỉ của c_string từ thanh ghi số 4
    		int length = kernel->machine->ReadRegister(5);  // đọc độ dài của chuỗi từ thanh ghi số 5
			//kiểm tra tràn bộ nhớ
    		if (length > 255) {  
       			 DEBUG(dbgSys, "String length:" << 255);
      		     SysHalt();
    		}
    		char* buffer = SysReadString(length);//đọc chuỗi từ bàn phím
    		WriteToMem(buffer, memPtr);//chuyển vùng nhớ system space vào vùng nhớ của người dùng 
    		delete[] buffer;
   		 	return inc_PC_register();;
		}
		case SC_PrintString:{
			int addr = kernel->machine->ReadRegister(4);  // read address of C-string

			char*str=ReadFromMem(addr);  
					// function() 
						//read value of each element in C-string from memory
					// return char*

			SysPrintString(str); //Print String  
			delete []str;
			return inc_PC_register();
		}
		//----------------------------
		case SC_CreateFile:{
			int addr = kernel->machine->ReadRegister(4);
			
			char*str =  ReadFromMem(addr);

			int result = SysCreateFile(str);

			kernel->machine->WriteRegister(2, result);
			

			delete[]str;
			return inc_PC_register();

		}
		case SC_Open:
		{
			int addr = kernel->machine->ReadRegister(4);
			
			char*fileName =  ReadFromMem(addr);

			int type = kernel->machine->ReadRegister(5);

		

			int id = SysOpen(fileName,type);

			kernel->machine->WriteRegister(2, id);

			delete[] fileName;

			return inc_PC_register();

		}
		case SC_Close:{
			int id = kernel->machine->ReadRegister(4);
   			 kernel->machine->WriteRegister(2, SysClose(id));

    		return inc_PC_register();
		}

		case SC_Read:{
			int virtAddr = kernel->machine->ReadRegister(4);
			int charCount = kernel->machine->ReadRegister(5);
			char* buffer = ReadFromMem(virtAddr, charCount);
			int fileId = kernel->machine->ReadRegister(6);


			kernel->machine->WriteRegister(2, SysRead(buffer, charCount, fileId));
			WriteToMem(buffer, virtAddr, charCount);

    		delete[] buffer;
			return inc_PC_register();
		}
		case SC_Write:{
			int virtAddr = kernel->machine->ReadRegister(4);
			int charCount = kernel->machine->ReadRegister(5);
			char* buffer = ReadFromMem(virtAddr, charCount);
			int fileId = kernel->machine->ReadRegister(6);


			kernel->machine->WriteRegister(2, SysWrite(buffer, charCount, fileId));
			WriteToMem(buffer, virtAddr, charCount);

			delete[] buffer;
			return inc_PC_register();
		}
		case SC_Seek:{
			int seekPos = kernel->machine->ReadRegister(4);
			int fileId = kernel->machine->ReadRegister(5);

			kernel->machine->WriteRegister(2, SysSeek(seekPos, fileId));
			return inc_PC_register();		
		}
		case SC_Remove:{
			int addr = kernel->machine->ReadRegister(4);
			
			char*fileName =  ReadFromMem(addr);

			kernel->machine->WriteRegister(2, SysRemove(fileName));
			delete[] fileName;
			return inc_PC_register();

		}
		//------------------
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}

}

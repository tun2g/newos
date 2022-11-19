// filesys.h 
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system. 
//	The "STUB" version just re-defines the Nachos file system 
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of 
//	a disk simulator.  The disk is simulated using the native UNIX 
//	file system (in a file named "DISK"). 
//
//	In the "real" implementation, there are two key data structures used 
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.  
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized. 
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"

#define FILE_DESCRIPTORS 20
#define MODE_READWRITE 0
#define MODE_READ 1
#define MODE_WRITE 2
#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {

	private:
		OpenFile** openFile;
		int *fileType;
 	 public:

   FileSystem() {
		openFile = new OpenFile*[FILE_DESCRIPTORS];
		fileType = new int[FILE_DESCRIPTORS];
		fileType[0]= 0;
		fileType[1]= 1;

		
	}

	~FileSystem(){
		  for (int i = 0; i < FILE_DESCRIPTORS; i++) {
            if (openFile[i]) delete openFile[i];
        }
        delete[] openFile;
        delete[] fileType;
	}

	int findFreeSlot(){
		
		int freeSlot =-1;
		for(int i=2;i<FILE_DESCRIPTORS;i++){
			if(openFile[i]==NULL){
				freeSlot = i;
				break;
			}
		}
		return freeSlot;
	}

	int Open(char*fileName, int type){
		int freeSlot =-1,fileDescriptor = -1;
		freeSlot = findFreeSlot();
		if(freeSlot == -1){
			return -1;
		}

		if(type == 0){
			fileDescriptor = OpenForReadWrite(fileName,FALSE);
			
		}
		if(type == 1 ){
			fileDescriptor = OpenForRead(fileName,FALSE);
		}

		if(fileDescriptor==-1)
			return -1;
		openFile[freeSlot] = new OpenFile(fileDescriptor);
		fileType[freeSlot] = type;

		return freeSlot;

	}

	int Close(int idx) { 
		if (idx < 2 || idx >= FILE_DESCRIPTORS) return -1;
        if (openFile[idx]) {
            delete openFile[idx];
            openFile[idx] = NULL;
            return 0;
        }
        return -1;
	 }

    int Read(char *buffer, int charCount, int idx) {
        if (idx >= FILE_DESCRIPTORS) return -1;
        if (openFile[idx] == NULL) return -1;
        int result = openFile[idx]->Read(buffer, charCount);
        // if we cannot read enough bytes, we should return -2
        if (result != charCount) return -2;
        return result;

		
    }

    int Write(char *buffer, int charCount, int idx) {
         if (idx >= FILE_DESCRIPTORS) return -1;
        if (openFile[idx] == NULL || fileType[idx] == MODE_READ)
            return -1;
        return openFile[idx]->Write(buffer, charCount);
    }

    int Seek(int position, int idx) {
         if (idx <= 1 || idx >= FILE_DESCRIPTORS) return -1;
        if (openFile[idx] == NULL) return -1;
        // use seek(-1) to move to the end of file
        if (position == -1) position = openFile[idx]->Length();
        if (position < 0 || position > openFile[idx]->Length()) return -1;
        return openFile[idx]->Seek(position);
		// return 0;
    }

    bool Create(char *name) {
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

    OpenFile* Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  return new OpenFile(fileDescriptor);
      }
	
    bool Remove(char* name) {
 		return Unlink(name) == 0; 
	}

};

#else // FILESYS
class FileSystem {
  public:
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
					// the disk, so initialize the directory
    					// and the bitmap of free blocks.

    bool Create(char *name, int initialSize);  	
					// Create a file (UNIX creat)

    OpenFile* Open(char *name); 	// Open a file (UNIX open)

    bool Remove(char *name);  		// Delete a file (UNIX unlink)

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents

  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
};

#endif // FILESYS

#endif // FS_H

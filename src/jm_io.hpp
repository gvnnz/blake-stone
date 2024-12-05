#ifndef JM_IO_HPP
#define JM_IO_HPP

#include <cstdint>

//-------------------------------------------------------------------------
// Function Prototypes
//-------------------------------------------------------------------------
bool IO_FarRead(int handle, std::uint8_t* dest, long length);
bool IO_FarWrite(int handle, std::uint8_t* source, long length);
bool IO_ReadFile(char* filename, void* ptr);
bool IO_WriteFile(char* filename, void* ptr, long length);
long IO_LoadFile(char* filename, void* dst);
void IO_CopyFile(char* sFilename, char* dFilename);
void IO_CopyHandle(int sHandle, int dHandle, long num_bytes);

#endif

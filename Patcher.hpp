#pragma once

#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <type_traits>

#define PTR2PTR(ptrInit) *(DWORD*)(*(DWORD*)ptrInit)

#define JMP_NEAR_SIZE 5
#define JNZ_NEAR_SIZE 6
#define JGE_NEAR_SIZE 6
#define JE_NEAR_SIZE 6
#define JG_NEAR_SIZE 6
#define JA_NEAR_SIZE 6
#define CALL_NEAR_SIZE 5

#define DEF_STR(str, addr) strcpy((CHAR*)addr, (CONST CHAR*)str)

__inline void BuildIndirection(DWORD dwAddress, BYTE* opCode, DWORD opCodeSize, void* dwFuncAddr)
{
    DWORD
        dwOldProtect,
        dwInstructionSize = (opCodeSize + sizeof(DWORD)),
        dwDistance = (((DWORD)dwFuncAddr - dwAddress) - dwInstructionSize);

    BYTE
        * indirection = (BYTE*)malloc(opCodeSize + sizeof(DWORD));

    // Build OpCode Instruction
    fflush(stdin);
    memcpy((BYTE*)&indirection[0], opCode, opCodeSize);

    // Build Distance of Instruction
    memcpy((BYTE*)&indirection[opCodeSize], &dwDistance, sizeof(DWORD));

    // Write Instruction
    VirtualProtect((LPVOID)dwAddress, dwInstructionSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    memcpy((DWORD*)dwAddress, indirection, dwInstructionSize);

    VirtualProtect((LPVOID)dwAddress, dwInstructionSize, dwOldProtect, &dwOldProtect);
}

__inline void FillWithNop(DWORD dwAddress, DWORD dwSize)
{
    DWORD
        dwOldProtect;

    BYTE
        * nop = (BYTE*)malloc(dwSize);

    VirtualProtect((LPVOID)dwAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    for (DWORD i = 0; i < dwSize; i++)
        nop[i] = 0x90;

    memcpy((DWORD*)dwAddress, &nop[0], dwSize);

    VirtualProtect((LPVOID)dwAddress, dwSize, dwOldProtect, &dwOldProtect);
}

__inline void JMP_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jmp = 0xE9;

    BuildIndirection(dwAddress, &jmp, 1, dwFuncAddr);
}

__inline void JMP_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    JMP_NEAR(dwAddress, dwFuncAddr);

    FillWithNop(dwAddress + JMP_NEAR_SIZE, dwNopedSize);
}

__inline void JGE_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jge[2] = { 0x0F, 0x8D };

    BuildIndirection(dwAddress, jge, 2, dwFuncAddr);
}

__inline void JGE_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    JGE_NEAR(dwAddress, dwFuncAddr);

    FillWithNop(dwAddress + JGE_NEAR_SIZE, dwNopedSize);
}


__inline void JG_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    BYTE
        jg[2] = { 0x0F, 0x8F };

    BuildIndirection(dwAddress, jg, 2, dwFuncAddr);
}
__inline void JG_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jg[2] = { 0x0F, 0x8F };

    BuildIndirection(dwAddress, jg, 2, dwFuncAddr);
}

__inline void JE_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jg[2] = { 0x0F, 0x84 };

    BuildIndirection(dwAddress, jg, 2, dwFuncAddr);
}

__inline void JE_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    JE_NEAR(dwAddress, dwFuncAddr);
    FillWithNop(dwAddress + JE_NEAR_SIZE, dwNopedSize);
}

__inline void JA_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jg[2] = { 0x0F, 0x87 };

    BuildIndirection(dwAddress, jg, 2, dwFuncAddr);
}

__inline void JA_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    JA_NEAR(dwAddress, dwFuncAddr);

    FillWithNop(dwAddress + JA_NEAR_SIZE, dwNopedSize);
}


__inline void JNZ_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        jnz[2] = { 0x0F, 0x85 };

    BuildIndirection(dwAddress, jnz, 2, dwFuncAddr);
}

__inline void JNZ_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    JNZ_NEAR(dwAddress, dwFuncAddr);

    FillWithNop(dwAddress + JNZ_NEAR_SIZE, dwNopedSize);
}

__inline void CALL_NEAR(DWORD dwAddress, void* dwFuncAddr)
{
    BYTE
        call = 0xE8;

    BuildIndirection(dwAddress, &call, 1, dwFuncAddr);
}

__inline void CALL_NEAR(DWORD dwAddress, void* dwFuncAddr, DWORD dwNopedSize)
{
    CALL_NEAR(dwAddress, dwFuncAddr);

    FillWithNop(dwAddress + CALL_NEAR_SIZE, dwNopedSize);
}

template <typename T>
void SetValue(unsigned int address, T value)
{
    static_assert(std::is_fundamental<T>::value, "Need to be a fundamental type");

    DWORD oldProt; \
        VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProt);

    *(T*)(address) = value;

    VirtualProtect((void*)address, sizeof(T), oldProt, &oldProt);
}

template<typename TRetn, typename... Args>
TRetn thisCall(int address, Args... args)
{
    return (*(TRetn(__thiscall*)(Args...)) address)(args...);
}

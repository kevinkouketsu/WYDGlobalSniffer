#include "Hooks.hpp"
#include "Patcher.hpp"
#define NOMINMAX
#include <Windows.h>
#include <format>
#include <iostream>

static size_t gBaseAddress = 0;

size_t getReturnAddress(size_t offset)
{
    return gBaseAddress + offset;
}

__declspec(naked) void NKD_ObjectManager_OnPacketEvent()
{
    __asm
    {
        PUSH 00h
        PUSH DWORD PTR SS:[EBP + 0Ch]

        CALL HKD_OnPacketEvent
        ADD ESP,08h

        CMP DWORD PTR DS : [ESI + 024h] , 07531h

        MOV EAX,gBaseAddress
        ADD EAX,011DBCCh
        PUSH EAX

        RETN
    }
}

__declspec(naked) void NKD_CPSocket_SendOneMessage()
{
    static int ecxVal = 0;
    static int esiVal = 0;
    static int retn = 0;
    __asm
    {
        MOV ecxVal, ECX
        MOV esiVal, ESI

        PUSH 01h
        PUSH DWORD PTR SS:[EBP + 08h]

        CALL HKD_OnPacketEvent
        ADD ESP,08h

        PUSH 0C7C19h
        CALL getReturnAddress
        ADD ESP, 04h

        MOV retn,EAX
        MOV EAX, DWORD PTR SS : [EBP + 0Ch]
        PUSH esiVal
        MOV ESI, ecxVal

        JMP retn
    }
}

void DoPatchs(size_t addr)
{
    JMP_NEAR(addr + 0x11DBC5, NKD_ObjectManager_OnPacketEvent, 2);
    JMP_NEAR(addr + 0x0C7C13, NKD_CPSocket_SendOneMessage, 1);

    std::cout << std::format("Patches done into {:06X}\n", addr);
    gBaseAddress = addr;
}
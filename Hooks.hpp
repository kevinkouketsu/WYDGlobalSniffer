#pragma once

void NKD_ObjectManager_OnPacketEvent();

enum class Direction
{
    Rx = 0,
    Tx = 1
};

void HKD_OnPacketEvent(char* szBuffer, Direction direction);
void DoPatchs(size_t addr);
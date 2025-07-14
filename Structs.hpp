#pragma once

struct MSG_STANDARD
{
    MSG_STANDARD() = default;
    MSG_STANDARD(unsigned short size, unsigned short type)
        : Size{ size }
        , Type{ type }
        , KeyWord{ 0 }
        , CheckSum{ 0 }
        , ID{ 0 }
        , Tick{ 0 }
    {
    }

    unsigned short Size;
    char KeyWord;
    char CheckSum;
    unsigned short Type;
    unsigned short ID;
    unsigned int Tick;
};

struct MSG_Action
{
    MSG_STANDARD Header;
    short PosX;
    short PosY;
    int Effect;
    int Speed;
    char Route[24];
    unsigned short TargetX;
    unsigned short TargetY;
};

struct MSG_UseItem
{
    MSG_STANDARD Header;
    int SourType;
    int SourPos;
    int DestType;
    int DestPos;
    unsigned short GridX;
    unsigned short GridY;
    unsigned short ItemID;
};
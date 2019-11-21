#include "keytable.h"

KeyTable::KeyTable()
{
        keyMap.insert(2 , "1");
        keyMap.insert(3 , "2");
        keyMap.insert(4 , "3");
        keyMap.insert(5 , "4");
        keyMap.insert(6 , "5");
        keyMap.insert(7 , "6");
        keyMap.insert(8 , "7");
        keyMap.insert(9 , "8");
        keyMap.insert(10 , "9");
        keyMap.insert(11 , "0");

        keyMap.insert(13 , "-");
        keyMap.insert(14 , "CLR");

        keyMap.insert(16 , "Q");
        keyMap.insert(17 , "W");
        keyMap.insert(18 , "E");
        keyMap.insert(19 , "R");
        keyMap.insert(20 , "T");
        keyMap.insert(21 , "Y");
        keyMap.insert(22 , "U");
        keyMap.insert(23 , "I");
        keyMap.insert(24 , "O");
        keyMap.insert(25 , "P");

        keyMap.insert(28 , "EXEC");

        keyMap.insert(30 , "A");
        keyMap.insert(31 , "S");
        keyMap.insert(32 , "D");
        keyMap.insert(33 , "F");
        keyMap.insert(34 , "G");
        keyMap.insert(35 , "H");
        keyMap.insert(36 , "J");
        keyMap.insert(37 , "K");
        keyMap.insert(38 , "L");

        keyMap.insert(44 , "Z");
        keyMap.insert(45 , "X");
        keyMap.insert(46 , "C");
        keyMap.insert(47 , "V");
        keyMap.insert(48 , "B");
        keyMap.insert(49 , "N");
        keyMap.insert(50 , "M");

        keyMap.insert(52 , ".");
        keyMap.insert(53 , "/");
        keyMap.insert(57 , " ");

        keyMap.insert(59 , "L1");
        keyMap.insert(60 , "L2");
        keyMap.insert(61 , "L3");
        keyMap.insert(62 , "L4");
        keyMap.insert(63 , "L5");
        keyMap.insert(64 , "L6");

        keyMap.insert(65 , "R1");
        keyMap.insert(66 , "R2");
        keyMap.insert(67 , "R3");
        keyMap.insert(68 , "R4");
        keyMap.insert(87 , "R5");
        keyMap.insert(88 , "R6");

        keyMap.insert(104 , "PREV");
        keyMap.insert(109 , "NEXT");
        keyMap.insert(111 , "DEL");

        ctrlMap.insert(59 , "INIT");
        ctrlMap.insert(60 , "RTE");
        ctrlMap.insert(61 , "CLB");
        ctrlMap.insert(62 , "CRZ");
        ctrlMap.insert(63 , "DES");
        ctrlMap.insert(64 , "MENU");

        ctrlMap.insert(65 , "LEGS");
        ctrlMap.insert(66 , "DEP");
        ctrlMap.insert(67 , "HOLD");
        ctrlMap.insert(68 , "PROG");
        ctrlMap.insert(87 , "N1");
        ctrlMap.insert(88 , "FIX");
}

QString KeyTable::nameForCode(uint code, bool ctrl)
{
        return ctrl ? ctrlMap.value(code) : keyMap.value(code);
}

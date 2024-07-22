@echo off

:: Build SHA256 Library
cl /c sha256.c
lib /OUT:sha256.lib sha256.obj

:: Build BIP39 Library
cl /c bip39.c
lib /OUT:bip39.lib bip39.obj

:: Build Main Program
cl /I"." enigma.c /link bip39.lib sha256.lib

:: Delete Library and Object Files
DEL sha256.lib
DEL bip39.lib
DEL sha256.obj
DEL bip39.obj
DEL enigma.obj


# ICE-BOOTLOADER

A simple bootloader being made as a learning experience. The road map is as follows:

- move from USB transport to Ethernet (RII PHY) with basic TCP/IP (LWiP?) \[TODO\]
- setup basic commands (PING, READ_FLASH, WRITE_FLASH, ERASE_FLASH, BOOT) \[TODO\]
- setup communications stack (simple protocol for transport, FSM for command decode, modular) \[WIP\]
- setup USB peripheral for communications using ST middleware \[DONE\]
- setup flash programming interface \[DONE\]
- setup cycle counter and delay interface (DWT) \[DONE\]
- setup basic peripherals (LED) \[DONE\]


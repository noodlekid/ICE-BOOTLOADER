# ICE-BOOTLOADER

A simple bootloader being made as a learning experience. The road map is as follows:
- add posix style interface over uart/usb/eth for configuring ip stack. \[TODO\]
- move from USB transport to Ethernet (RII PHY) with basic TCP/IP (LWiP?) \[TODO\]
- setup basic commands (PING (ok), READ_FLASH (wip), WRITE_FLASH (todo), ERASE_FLASH (todo), BOOT (todo) \[WIP\]
- setup communications stack (simple protocol for transport, FSM for command decode (COBS), modular) \[DONE\]
- setup USB peripheral for communications using ST middleware \[DONE\]
- setup flash programming interface \[DONE\]
- setup cycle counter and delay interface (DWT) \[DONE\]
- setup basic peripherals (LED) \[DONE\]


# WindowsOffender, stealth information gathering tool
#POC  
I have developed a malware program using the C programming language, which includes various malicious features such as privilege escalation exploit, lsass dumper, reverse shell, keylogging, IAT hooking, DLL injection, and more. It should be noted that the program is constantly being updated, and some versions may not be fully functional.

The malware's methodology involves several steps, including UAC bypassing on Windows 10 to gain administrative privileges, injecting the entire PE image into the runtime broker for process hollowing, hooking antivirus functions, creating a socket with a C&C server, starting a reverse shell, executing arbitrary commands, dumping lsass and parsing its memory with Mimikatz, and enabling keylogging.

Furthermore, these features can be combined, such as injecting a shellcode remotely into a process's PID from a remote server. It is essential to emphasize that this program is intended for educational purposes only and must not be used for any harmful or malicious activities.

In conclusion, the malware created is capable of causing significant damage to Windows security and can operate efficiently on the latest Windows versions. It is critical to note that the use of this program for any malicious purposes is strictly prohibited and unlawful.

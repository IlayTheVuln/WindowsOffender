# WindowsOffender
#POC  
I wrote a nice malware in c that is capeable to destroy windows security. it involves a fresh privilege escelation exploit that i found, lsass dumper, reverse shell, keylogging, iat hooking, hollowing, dll injection and more!!
please note that im updating it almost daily and some of the versions wont be fully completed!
!!!!note that ive created it for educational puprposes only! DO NOT USE FOR ANY HARM!!!!

The malware metodology is:
1. UAC bypassing on win10, then becoming an admin silently.
2. Injecting our whole pe image into to runtime broker so that our process will be hollowed
3. Hooking the antivirus ReadFile, NtCreateFile and more(you can add the hooking functions in the hookerdll file)
4. creating a socket with the c&c server ad starting revese shell
5. running arbitary commands
6. dumping lsass, and parsing its memoey with mimikatz on our host machine
7. starting keylogging
8. now you can combine the options. for example you can get a process's pid and inject it a shellcode remotely-remotely(a remote process from a remote server :)
9. do not use for any harm! this is only a proof of concept and i built it for educational purposes only. beware with the malware cause it is truely dangerous and works completely flawlessly on the newest windows versions!!

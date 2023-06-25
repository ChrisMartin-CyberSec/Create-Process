# Create-Process
C++ Process Enumeration and Creation 

## Process Enumeration

Used to find the Process ID of an entered process name

**Case Insensitive**

## Process Creation

Will attempt to create the process and provide the corresponding Process ID if process not currently running

### Syntax

`.\ProcessEnum.exe <PROCESS NAME>`

```
C:\Users\%user\Documents\> .\ProcessEnum.exe notepad.exe
[-] PROCESS NOT FOUND!
[+] CREATING PROCESS...
[+] PROCESS ID: 25328
```

### Extra Tidbit
I am still developing this script out for future use in system enumeration and future Process/DLL Injection labs.

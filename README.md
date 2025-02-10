# nt_rootkit

A simple rootkit that allows a user to hide a process by unlinking it from the list of running processes, set its PID to 0x1337, or steal one process's token and write it to another.

This was compiled on Windows 11 version 23H2.
I recommend using this in test mode with PatchGuard disabled.

Add a service that starts the driver by using 
```sc.exe create "serviceName" type= kernel binPath= "\Path\To\Sysfile"```

Then start it with
```sc.exe start "serviceName"```

To interact with the driver, compile and run the provided user client. Or make your own, whatever works better.

Copyright 2024 Kyle Smith
Licensed under GPL-3.0-or-later

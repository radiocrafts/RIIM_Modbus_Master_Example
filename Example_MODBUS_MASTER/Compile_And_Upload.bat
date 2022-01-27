set PATH_TO_MAKE="C:\Program Files (x86)\GnuWin32\bin\make.exe"

make SOURCE_FILE="App_TransparentUART_DB.c"

@rem Remove unused files for clarity
@del Output\*.o Output\*.map Output\*.elf Output\*.hex


@echo;
@echo;
@echo **********************************************
@echo ALL DONE!
@echo **********************************************

pause
@echo off
echo Cleaning...

for /r %%f in (*.pch *.ilk *.pdb *.obj *.asm) do (
  if exist "%%f" (
    del /Q "%%f"
    echo Deleting %%f
  )
)

for /r %%f in (*.exe) do (
  del /Q "%%f"
  echo Deleting %%f
)

echo Done.

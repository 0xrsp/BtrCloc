@echo off
echo Cleaning...

for /r %%f in (*.pch *.ilk *.pdb *.obj *.exe) do (
  del /Q "%%f"
  echo Deleting %%f
)

echo Done.

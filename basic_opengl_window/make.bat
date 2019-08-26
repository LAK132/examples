REM Call the common make.bat but from the directory of this one
pushd %~dp0
call ..\common\make.bat %*
popd
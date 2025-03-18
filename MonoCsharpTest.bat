call .\Init.bat

@REM dotnet build -f net9.0 .\src\MonoHelloWorld\MonoHelloWorld.csproj
@REM adb shell rm -f -rR /data/local/tmp/MonoHelloWorld
@REM adb push ./src/MonoHelloWorld/bin/Debug/net9.0 /data/local/tmp/MonoHelloWorld/
@REM adb push ./libs/%MONO_PACKAGE%/runtimes/android-arm64/lib/net9.0 /data/local/tmp/MonoHelloWorld/
@REM adb shell mv -v /data/local/tmp/MonoHelloWorld/net9.0/*.* /data/local/tmp/MonoHelloWorld/
@REM adb shell rm -f -rR /data/local/tmp/MonoHelloWorld/net9.0
@REM adb shell chmod 755 /data/local/tmp/MonoHelloWorld/*.*
@REM adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/system/lib64:/apex/com.android.runtime/lib64/bionic:/data/local/tmp/Mono ^
@REM MONO_LOG_LEVEL=debug ^
@REM MONO_LOG_MASK=all ^
@REM MONO_EXECUTE_ASSEMBLY=/data/local/tmp/MonoHelloWorld/MonoHelloWorld.dll ^
@REM DOTNET_ENTRYPOINT_PATH=MonoHelloWorld.Program:Main ^
@REM /data/local/tmp/Mono/MonoRuntime

dotnet build -f net9.0-android35.0 .\src\MonoHelloWorld\MonoHelloWorld.csproj
adb shell rm -f -rR /data/local/tmp/MonoHelloWorld_Android
adb push ./src/MonoHelloWorld/bin/Debug/net9.0-android35.0 /data/local/tmp/MonoHelloWorld_Android/
adb push ./libs/%MONO_PACKAGE%/runtimes/android-arm64/lib/net9.0 /data/local/tmp/MonoHelloWorld_Android/
adb shell mv -v /data/local/tmp/MonoHelloWorld_Android/net9.0/*.* /data/local/tmp/MonoHelloWorld_Android/
adb shell rm -f -rR /data/local/tmp/MonoHelloWorld_Android/net9.0
adb shell chmod 755 /data/local/tmp/MonoHelloWorld_Android/*.*
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/system/lib64:/apex/com.android.runtime/lib64/bionic:/data/local/tmp/Mono ^
MONO_LOG_LEVEL=debug ^
MONO_LOG_MASK=all ^
MONO_EXECUTE_ASSEMBLY=/data/local/tmp/MonoHelloWorld_Android/MonoHelloWorld.dll ^
DOTNET_ENTRYPOINT_PATH=MonoHelloWorld.Program:Main ^
/data/local/tmp/Mono/MonoRuntime
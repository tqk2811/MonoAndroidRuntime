call .\Init.bat

dotnet build .\src\MonoHelloWorld\MonoHelloWorld.csproj

adb shell rm -f -rR /data/local/tmp/MonoHelloWorld
adb push ./src/MonoHelloWorld/bin/Debug/net9.0 /data/local/tmp/MonoHelloWorld/
@REM adb push ./libs/%MONO_PACKAGE%/runtimes/android-arm64/lib/net9.0 /data/local/tmp/MonoHelloWorld/
@REM adb shell mv -v /data/local/tmp/MonoHelloWorld/net9.0/*.* /data/local/tmp/MonoHelloWorld/
@REM adb shell rm -f -rR /data/local/tmp/MonoHelloWorld/net9.0

@REM adb shell rm -f -rR /data/local/tmp/MonoHelloWorld_Android
@REM adb push ./src/MonoHelloWorld/bin/Debug/net9.0-android35.0 /data/local/tmp/MonoHelloWorld_Android/


@REM adb shell chmod 755 /data/local/tmp/MonoHelloWorld/*.*
adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/system/lib64:/apex/com.android.runtime/lib64/bionic:/data/local/tmp/Mono:/data/local/tmp/Mono/net9.0 ^
MONO_LOG_LEVEL=debug ^
MONO_LOG_MASK=all ^
MONO_PATH=/data/local/tmp/Mono ^
/data/local/tmp/Mono/MonoRuntime /data/local/tmp/MonoHelloWorld/MonoHelloWorld.dll
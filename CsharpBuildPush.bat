dotnet build -f %CS_TARGET_FRAMEWORK% .\src\MonoHelloWorld\MonoHelloWorld.csproj
adb shell rm -f -rR %PUSH_DIR%
adb push ./src/MonoHelloWorld/bin/Debug/%CS_TARGET_FRAMEWORK% %PUSH_DIR%/
adb push ./libs/%MONO_PACKAGE%/runtimes/android-arm64/lib/net9.0 %PUSH_DIR%/
adb shell mv -v %PUSH_DIR%/net9.0/*.* %PUSH_DIR%/
adb shell rm -f -rR %PUSH_DIR%/net9.0
adb shell chmod 755 %PUSH_DIR%/*.*
adb shell ^
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/system/lib64:/apex/com.android.runtime/lib64/bionic:/data/local/tmp/Mono ^
MONO_LOG_LEVEL=debug ^
MONO_LOG_MASK=all ^
MONO_LIB_NATIVE_PATH=/data/local/tmp/Mono ^
MONO_EXECUTE_ASSEMBLY=%PUSH_DIR%/MonoHelloWorld.dll ^
DOTNET_ENTRYPOINT_PATH=MonoHelloWorld.Program:Main ^
%MONO_RUNTIME%
call .\Init.bat
rmdir /S /Q .\build
cmake -S . -B ./build ^
    -G "Ninja" ^
    -DANDROID_NDK=%ANDROID_NDK_HOME% ^
    -DCMAKE_ANDROID_NDK=%ANDROID_NDK_HOME% ^
    -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK_HOME%/build/cmake/android.toolchain.cmake ^
    -DMONO_PACKAGE=%MONO_PACKAGE% ^
    -DCMAKE_SYSTEM_VERSION=21 ^
    -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a ^
    -DANDROID_ABI=arm64-v8a ^
    -DCMAKE_SYSTEM_NAME=Android ^
    -DANDROID_PLATFORM=android-21 ^
    -DCMAKE_C_COMPILER=clang ^
    -DCMAKE_CXX_COMPILER=clang++ ^
    -DCMAKE_ANDROID_STL_TYPE=c++_static ^
    -DCMAKE_BUILD_TYPE=Release

ninja -C ./build

if exist ./build/MonoRuntime (
adb push ./build/MonoRuntime /data/local/tmp/Mono/MonoRuntime
adb shell chmod 755 /data/local/tmp/Mono/MonoRuntime

adb shell LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/Mono ^
MONO_PATH=/data/local/tmp/Mono ^
/data/local/tmp/Mono/MonoRuntime
)

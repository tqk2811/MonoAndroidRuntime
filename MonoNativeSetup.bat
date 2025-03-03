call .\Init.bat

set MONO_PACKAGE_PATH=.\libs\%MONO_PACKAGE%.%MONO_PACKAGE_VERSION%.nupkg
if not exist .\libs\%MONO_PACKAGE%.%MONO_PACKAGE_VERSION%.nupkg (
    curl -L -o %MONO_PACKAGE_PATH% https://api.nuget.org/v3-flatcontainer/%MONO_PACKAGE%/%MONO_PACKAGE_VERSION%/%MONO_PACKAGE%.%MONO_PACKAGE_VERSION%.nupkg
)

set MONO_PACKAGE_EXTRACT_PATH=.\libs\%MONO_PACKAGE%
if not exist %MONO_PACKAGE_EXTRACT_PATH% (
    powershell -Command "Expand-Archive -Path '%MONO_PACKAGE_PATH%' -DestinationPath '%MONO_PACKAGE_EXTRACT_PATH%' -Force"
)

set MONO_PACKAGE_LIB_PATH=./libs/%MONO_PACKAGE%/runtimes/android-arm64
if exist %MONO_PACKAGE_LIB_PATH% (
    adb shell rm -f -rR /data/local/tmp/Mono
    adb push %MONO_PACKAGE_LIB_PATH%/native/ /data/local/tmp/Mono/
    adb shell chmod 755 /data/local/tmp/Mono/*.*
)
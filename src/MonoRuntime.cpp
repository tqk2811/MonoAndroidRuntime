#include <mono/jit/jit.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/image.h>
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/class.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/debug-helpers.h>
#include <iostream>
#include <stdio.h>
#pragma comment(lib, "libmonosgen-2.0.so")

int main(int argc, char *argv[])
{
    std::cout << "Args:" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "\t" << argv[i] << std::endl;
    }

    const char *MONO_LIB_NATIVE_PATH = getenv("MONO_LIB_NATIVE_PATH");
    if (!MONO_LIB_NATIVE_PATH)
        MONO_LIB_NATIVE_PATH = "/data/local/tmp/Mono";
    const char *MONO_LIB_DOTNET_PATH = getenv("MONO_LIB_DOTNET_PATH");
    if (!MONO_LIB_DOTNET_PATH)
        MONO_LIB_DOTNET_PATH = "/data/local/tmp/Mono/net9.0";

    std::cout << "🔹 Set mono_set_assemblies_path(\"/system/lib64\") " << std::endl;
    mono_set_assemblies_path("/system/lib64");
    std::cout << "🔹 Set mono_set_assemblies_path(\"/apex/com.android.runtime/lib64/bionic\")" << std::endl;
    mono_set_assemblies_path("/apex/com.android.runtime/lib64/bionic");

    std::cout << "🔹 Set MONO_LIB_NATIVE_PATH mono_set_assemblies_path(\"" << MONO_LIB_NATIVE_PATH << "\")" << std::endl;
    mono_set_assemblies_path(MONO_LIB_NATIVE_PATH);

    std::cout << "🔹 Set MONO_LIB_DOTNET_PATH mono_set_assemblies_path(\"" << MONO_LIB_DOTNET_PATH << "\")" << std::endl;
    mono_set_assemblies_path(MONO_LIB_DOTNET_PATH);

    mono_jit_parse_options(argc, (char **)argv);
    mono_debug_init(MONO_DEBUG_FORMAT_MONO);
    // mono_jit_set_aot_only(true);

    std::cout << "🔹 mono_jit_init " << std::endl;
    MonoDomain *domain = mono_jit_init("MonoRuntime");

    const char *MONO_EXECUTE_ASSEMBLY = getenv("MONO_EXECUTE_ASSEMBLY");
    if (!MONO_EXECUTE_ASSEMBLY)
    {
        std::cout << "env MONO_EXECUTE_ASSEMBLY is missing" << std::endl;
        return 1;
    }
    std::cout << "🔹 mono_domain_assembly_open(\"" << MONO_EXECUTE_ASSEMBLY << "\")" << std::endl;
    MonoAssembly *assembly = mono_domain_assembly_open(domain, MONO_EXECUTE_ASSEMBLY);
    if (!assembly)
    {
        std::cerr << "❌ Error: mono_domain_assembly_open failed " << std::endl;
        return 1;
    }

    std::cout << "🔹 mono_assembly_get_image" << std::endl;
    MonoImage *image = mono_assembly_get_image(assembly);
    if (!image)
    {
        std::cerr << "❌ Error: mono_assembly_get_image failed\n";
        return 1;
    }

    std::cout << "🔹 mono_image_get_entry_point " << std::endl;
    uint32_t entry_token = mono_image_get_entry_point(image);
    if (!entry_token)
    {
        std::cerr << "❌ mono_image_get_entry_point failed\n";
        return 1;
    }

    // 🔹 Lấy method EntryPoint từ Token
    std::cout << "🔹 mono_get_method " << std::endl;
    MonoMethod *entry_method = mono_get_method(image, entry_token, NULL);
    if (!entry_method)
    {
        std::cerr << "❌ mono_get_method from entry_point_token failed\n";
        return 1;
    }

    // 🔹 Gọi entry method
    std::cout << "🔹 mono_runtime_invoke\n";
    MonoObject *exc = NULL;
    mono_runtime_invoke(entry_method, NULL, NULL, &exc);
    if (exc)
    {
        MonoString *exc_str = mono_object_to_string(exc, NULL);
        const char *msg = mono_string_to_utf8(exc_str);
        std::cerr << "❌ Exception: " << msg << std::endl;
        mono_free((void *)msg);
    }

    // 🔹 Dọn dẹp Mono runtime
    std::cout << "🔹 mono_jit_cleanup\n";
    mono_jit_cleanup(domain);
    return 0;
}
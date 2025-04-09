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

MonoMethod *GetEntryPoint(MonoImage *image)
{
    std::cerr << "🔹 mono_image_get_entry_point " << std::endl;
    uint32_t entry_token = mono_image_get_entry_point(image);
    if (entry_token)
    {
        // 🔹 Lấy method EntryPoint từ Token
        std::cerr << "🔹 mono_get_method " << std::endl;
        MonoMethod *entry_method = mono_get_method(image, entry_token, NULL);
        if (entry_method)
        {
            return entry_method;
        }
        else
        {
            std::cerr << "❌ mono_get_method from entry_point_token failed\n";
        }
    }
    else
    {
        std::cerr << "❌ mono_image_get_entry_point failed\n";
    }

    const char *DOTNET_ENTRYPOINT_PATH = getenv("DOTNET_ENTRYPOINT_PATH");
    if (DOTNET_ENTRYPOINT_PATH)
    {
        std::cerr << "🔹 mono_method_desc_new(\"" << DOTNET_ENTRYPOINT_PATH << "\")" << std::endl;
        MonoMethodDesc *desc = mono_method_desc_new(DOTNET_ENTRYPOINT_PATH, false);
        if (desc)
        {
            std::cout << "🔹 mono_method_desc_search_in_image" << std::endl;
            MonoMethod *method = mono_method_desc_search_in_image(desc, image);
            if(!method)
            {
                std::cerr << "❌ mono_method_desc_search_in_image failed\n";
            }
            mono_method_desc_free(desc);
            return method;
        }
        else
        {
            std::cerr << "❌ mono_method_desc_new failed\n";
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    std::cerr << "Args:" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cerr << "\t" << argv[i] << std::endl;
    }

    const char *MONO_LIB_NATIVE_PATH = getenv("MONO_LIB_NATIVE_PATH");
    if (!MONO_LIB_NATIVE_PATH)
        MONO_LIB_NATIVE_PATH = "/data/local/tmp/Mono";

    std::cerr << "🔹 Set MONO_LIB_NATIVE_PATH mono_set_assemblies_path(\"" << MONO_LIB_NATIVE_PATH << "\")" << std::endl;
    mono_set_assemblies_path(MONO_LIB_NATIVE_PATH); // set one time, only for mono native

    mono_jit_parse_options(argc, (char **)argv);
    mono_debug_init(MONO_DEBUG_FORMAT_MONO);
    // mono_jit_set_aot_only(true);

    std::cerr << "🔹 mono_jit_init " << std::endl;
    MonoDomain *domain = mono_jit_init("MonoRuntime");
    
    const char *MONO_EXECUTE_ASSEMBLY = getenv("MONO_EXECUTE_ASSEMBLY");
    if (!MONO_EXECUTE_ASSEMBLY)
    {
        std::cerr << "env MONO_EXECUTE_ASSEMBLY is missing" << std::endl;
        return 1;
    }
    std::cerr << "🔹 mono_domain_assembly_open(\"" << MONO_EXECUTE_ASSEMBLY << "\")" << std::endl;
    MonoAssembly *assembly = mono_domain_assembly_open(domain, MONO_EXECUTE_ASSEMBLY);
    if (!assembly)
    {
        std::cerr << "❌ Error: mono_domain_assembly_open failed " << std::endl;
        return 1;
    }

    std::cerr << "🔹 mono_assembly_get_image" << std::endl;
    MonoImage *image = mono_assembly_get_image(assembly);
    if (!image)
    {
        std::cerr << "❌ Error: mono_assembly_get_image failed\n";
        return 1;
    }

    MonoMethod *entry_method = GetEntryPoint(image);
    if (!entry_method)
    {
        std::cerr << "❌ GetEntryPoint failed\n";
        return 1;
    }

    // 🔹 Gọi entry method
    std::cerr << "🔹 mono_runtime_invoke\n";
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
    std::cerr << "🔹 mono_jit_cleanup\n";
    mono_jit_cleanup(domain);
    return 0;
}
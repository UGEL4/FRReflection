target("Test")
    source_file_list = {"$(projectdir)/Src/**.cpp", "$(projectdir)/Test/**.cpp"}
    set_kind("binary")
    if (is_os("windows")) then 
        add_syslinks("advapi32", "user32", "shell32", "Ole32", {public = true})
    end
    add_cxflags(project_cxflags, {public = true, force = true})
    add_defines("UNICODE")
    --add_defines("GPU_USE_VULKAN")
    --add_defines("GPU_USE_D3D12")
    --add_includedirs(include_dir_list)
    add_files(source_file_list)
target_end()

package("veigar")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "veigar"))
    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        table.insert(configs, "-DMSGPACK_NO_BOOST")
        package:add("defines", "MSGPACK_NO_BOOST")
        import("package.tools.cmake").install(package, configs)
        os.trycp("include/veigar", package:installdir("include"))
    end)
    -- on_test(function (package)
    --     assert(package:has_cfuncs("add", {includes = "foo.h"}))
    -- end)
package_end()

add_requires('veigar')

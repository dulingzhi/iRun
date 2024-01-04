
set_project('iRun')
    set_version('1.0.0')
    add_rules("mode.debug", "mode.release")
    set_symbols('debug')
    add_requires('microsoft-detours')

    if is_mode('debug') then
        add_defines('_DEBUG')
        set_runtimes('MTd')
        add_ldflags('/subsystem:console')
    else
        add_defines('NDEBUG')
        set_runtimes('MT')
        add_ldflags('/subsystem:windows')
    end

    includes {
        'contrib',
        'src/Monitor',
        'src/Launcher'
    }


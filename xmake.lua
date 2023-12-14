
set_project('iRun')
    set_version('1.0.0')
    add_rules("mode.debug", "mode.release")
    set_symbols('debug')
    add_requires('rest_rpc', 'microsoft-detours')

    includes('src/Monitor')
    includes('src/Launcher')

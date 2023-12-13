
set_project('iRun')
    add_rules("mode.debug", "mode.release")
    add_requires('rest_rpc', 'microsoft-detours')

    includes('src/Monitor')
    includes('src/Launcher')

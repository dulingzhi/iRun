
target('Launcher')
    add_rules('win.sdk.mfc.static_app')
    set_policy("windows.manifest.uac", "admin")
    set_kind('binary')
    add_files('**.cpp', '**.rc', 'compatibility.manifest')
    add_headerfiles('**.h')
    set_pcxxheader('src/pch.h')
    add_packages('rest_rpc', 'microsoft-detours')
    add_deps('Monitor')
    add_defines('UNICODE', '_UNICODE')

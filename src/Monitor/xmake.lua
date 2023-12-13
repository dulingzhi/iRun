
target("Monitor")
    set_kind("shared")
    add_files("**.cpp")
    add_headerfiles("**.h")
    set_pcxxheader('src/pch.h')
    add_packages('rest_rpc', 'microsoft-detours')
    add_includedirs('include', { public = true })
    add_links('advapi32', 'kernel32', 'ole32', 'user32')

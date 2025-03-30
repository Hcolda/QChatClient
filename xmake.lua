set_languages("c11", "cxx20")
add_requires("vcpkg::asio", "vcpkg::openssl")
target("test")
    add_rules("qt.widgetapp")
    add_files("src/**.cpp")
    add_files("src/**.ui")
    add_files("src/**.h")
    add_files("include/*.hpp")
    add_includedirs("include", "src/parser")
    add_files("*.qrc")
    add_packages("vcpkg::asio", "vcpkg::openssl")
    if is_plat("windows") then
        add_syslinks("crypt32")
    end

echo "run install_libxml_win.ps1"
echo "<SOURCE_DIR> = $($args[0])"
echo "<CMAKE_INSTALL_PREFIX> = $($args[1])"

mkdir -p "$($args[1])\\include\libxml2\\libxml" 
mkdir -p "$($args[1])\\lib"
cp "$($args[0])\\include\\libxml\\*.h" "$($args[1])\\include\libxml2\\libxml" 
cp "$($args[0])\\win32\\bin.msvc\\*.lib" "$($args[1])\\lib" 
cp "$($args[0])\\win32\\bin.msvc\\*.dll" "$($args[1])\\lib" 
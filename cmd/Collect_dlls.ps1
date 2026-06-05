#完整版本
# Get-ChildItem .\3rdparty-vc14-64\ -Filter "*.dll" -Recurse|Where-Object{$_.FullName -notmatch '\\(qt5|vtk)'}|Copy-Item -Destination dlls -Force

#简短别名版本
# 参数说明：
# gci= Get-ChildItem
# -r= -Recurse
# ?= Where-Object
# \\(qt|debug|release)\\= 匹配路径中包含 qt、debug 或 release 的文件夹
# %= ForEach-Object
# cp= Copy-Item
# -Fo= -Force
gci ".\3rdparty-vc14-64\" -r *.dll | ? { $_.FullName -notmatch '\\(qt|vtk)' } | cp -Destination "dlls" -Fo
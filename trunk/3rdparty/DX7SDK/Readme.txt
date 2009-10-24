DirectX 简装版使用方法:

1.将 include 目录下的所有文件拷贝到 VC++ 所在目录下的 include 目录下,并覆盖原有文件.

2.将 lib 目录下的所有文件拷贝到 VC++ 所在目录下的 lib 目录下,并覆盖原有文件.

3.在工程的 Project\Setthings\link\ 的 Object/library modules 中加入 ddraw.lib

4.将 ddutil.cpp 拷贝到 工程目录下.

5.再工程文件中加入语句: #include "ddutil.cpp"
# CNDrv 开发环境配置

1. 安装 CNToolkit

2. 按照实际安装路径设置 NEUWARE_HOME 环境变量（可以写入到 ~/.bashrc 中永久生效）

   ```
   export NEUWARE_HOME=/usr/local/neuware
   ```

3. 编译

   ```
   ./build.sh
   ```

## vscode扩展配置（clangd）

1. 安装 vscode 扩展 clangd

2. 安装 clang，先查询软件包中的 clangd 版本

   ```
   sudo apt update
   sudo apt search clangd
   sudo apt install clangd-10
   ```

3. 在 clangd 扩展的**远程**设置中修改 Clangd:Path 为 /usr/bin/clangd-10

   （如果安装时没有指定 clangd 版本，则设置为 /usr/bin/clangd 即可）

4. 使用 ./build.sh 编译生成 compile_commands.json 文件

5. 重启 clangd 插件即可


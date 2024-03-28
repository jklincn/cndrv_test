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

3. 在 clangd 扩展的**远程**设置中修改 Clangd:Path 为 /usr/bin/clangd

   （如果没有 /usr/bin/clangd，则进行软链接）

   ```
   sudo ln -s /usr/bin/clangd-10 /usr/bin/clangd
   ```

4. 使用 bear 生成编译数据库 compile_commands.json

   ```
   sudo apt install bear
   bear ./build.sh (低版本bear)
   bear -- ./build.sh (高版本bear)
   ```

5. 重启 clangd 插件即可


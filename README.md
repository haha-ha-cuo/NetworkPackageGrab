# NetworkPackageGrab

**项目概述**

- **名称**: NetworkPackageGrab
- **简介**: 一个用于抓取、解析与展示网络数据包（PCAP）的桌面应用程序。项目以 C++/Win32（Visual Studio）为主，包含网络数据解析、统计与可视化展示的模块化实现，适合作为教学演示或二次开发基础。

**主要特性**

- **数据采集与解析**: 支持读取并解析标准 PCAP 文件并生成会话/协议统计信息。
- **模块化代码组织**: 功能按 `Common`、`Core`、`Data`、`NetWork`、`Render`、`Statistics`、`Storage` 等模块划分，便于维护与扩展。
- **示例数据与快速验证**: 仓库 `Output/` 目录内包含多个示例 `.pcap` 文件，用于快速验证与调试。

**快速开始（开发者）**

- **打开工程**: 使用 Visual Studio 打开解决方案 [NetworkGrabPackage/NetworkGrabPackage.sln](NetworkGrabPackage/NetworkGrabPackage.sln)。
- **平台与配置**: 请选择 `x64` 平台，并在 `Debug` 或 `Release` 配置下构建。
- **构建**: 直接在 Visual Studio 中点击“生成解决方案”。
- **运行**: 在 VS 中启动 `NetworkGrabPackage` 项目，或直接运行生成的可执行文件（通常在 [NetworkGrabPackage/x64/Debug](NetworkGrabPackage/x64/Debug) 或相应 Release 文件夹）。

**依赖说明**

- 项目内包含部分第三方头文件与源码（位于 `Include/` 与 `Lib/`），常见依赖包括：
	- Npcap/WinPcap（抓包驱动与头文件）
	- cpr（HTTP 客户端库）
	- OpenSSL（加密）
	- cpprestsdk（可选的 REST 支持）
- 若构建或运行时出现缺失头或库的错误，请在项目属性中检查并补充 `VC++ 目录` 下的“包含目录”和“库目录”。

**代码结构（概要）**

- `NetworkGrabPackage/`: 主程序源码。
	- `Main.cpp`: 程序入口（参见 [NetworkGrabPackage/Main.cpp](NetworkGrabPackage/Main.cpp)）。
	- 功能模块目录：`Common/`、`Core/`、`Data/`、`NetWork/`、`Render/`、`Statistics/`、`Storage/`。
- `Include/`: 各类第三方头文件与嵌入式依赖（`pcap.h`、`zlib.h` 等）。
- `Lib/`: 可能包含预编译依赖或平台相关库。
- `Output/`: 示例与测试用的 `.pcap` 文件集合，便于快速验证功能。

**使用示例**

- 在程序中选择或导入一个 `.pcap` 文件（例如仓库内的 `[Output/2025-12-24_16-39-44-468.pcap](Output/2025-12-24_16-39-44-468.pcap)`），程序应解析并展示协议分布、会话详情与流量统计。

**调试提示**

- 构建失败时：检查 `Include/` 与 `Lib/` 是否在项目属性中正确配置。
- Npcap 依赖：请确保目标系统已安装 Npcap，且版本兼容（管理员安装推荐）。
- 运行时无法读取 PCAP：确认文件格式（pcap/pcapng）与文件权限，确保文件未被其他进程占用。

**开发与贡献指南**

- 功能扩展建议：
	- 将解析器与统计模块拆分为独立库，便于单元测试与复用；
	- 增加命令行工具模式用于批处理与自动化分析；
	- 添加单元测试与持续集成（如 GitHub Actions）以保证代码质量。
- 提交代码：请在本地通过 Debug 构建并基本验证功能后，发起 PR 并附带变更说明与复现步骤。

**许可与作者**

- 许可：当前仓库未包含明确的 `LICENSE` 文件。请在项目根目录添加合适的许可文件（例如 MIT、Apache-2.0 等），并在此处注明。
- 作者/维护者信息：请补充联系方式或维护者信息以便协作。

---

如果你希望我把某部分进一步细化（例如：详细的 Visual Studio 配置步骤、如何编译/更新依赖库，或添加命令行批处理示例），告诉我需要补充的章节，我会继续完善 README。 

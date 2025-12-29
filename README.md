# NetworkPackageGrab

NetworkPackageGrab 是一个 Windows 控制台程序，用于在本机网卡上抓取网络数据包（基于 Npcap/WinPcap 的 libpcap API），并对抓到的流量做基础解析与统计展示。本项目以 Visual Studio C++ 工程形式组织，适合作为网络抓包/协议解析的学习工程或二次开发骨架。

## 目录

- 功能一览
- 环境要求
- 快速开始（开发者）
- 使用说明（程序内菜单）
- 核心流程
- 目录结构
- 依赖与运行时
- 二次开发
- 已知限制
- 常见问题
- 许可

## 功能一览

- 实时抓包：选择网卡 + 输入过滤表达式（BPF），实时抓取并在控制台滚动展示解析结果
- 自动落盘：抓到的数据包会自动以 `.pcap` 形式写入 `NetworkGrabPackage/Output/`，文件名带时间戳
- 协议解析：内置 Ethernet / IPv4 / TCP / UDP 的基础解析（当前以 IPv4 为主）
- HTTP/HTTPS 分析：从离线 `.pcap` 中提取 TCP Payload，识别并展示 HTTP 报文；对 HTTPS 尝试解析 TLS ClientHello 的 SNI（域名）
- 流量统计：遍历 `NetworkGrabPackage/Output/` 中的 `.pcap` 文件，统计源/目的 IP、协议分布，并按目的 IP 的 A/B/C/D/E/L 类别汇总

## 环境要求

- Windows 10/11
- Visual Studio 2022（或 2019）+ “使用 C++ 的桌面开发”
- Windows SDK 10
- Npcap（推荐安装并勾选 *WinPcap API-compatible Mode*）

## 快速开始（开发者）

### 1) 打开与构建

- 用 Visual Studio 打开解决方案：[NetworkGrabPackage.sln](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage.sln)
- 推荐配置：`x64` + `Debug`（工程对 x64 配置了 `Include/` 与 `Lib/x64` 的搜索路径）
- 点击“生成解决方案”

### 2) 运行

- 在 VS 中设置启动项目为 `NetworkGrabPackage`，点击运行
- 若直接运行可执行文件：通常位于 `NetworkGrabPackage/NetworkGrabPackage/x64/{Debug|Release}/`

### 3)（可选）命令行构建

在 “x64 Native Tools Command Prompt for VS” 中运行：

```bat
msbuild NetworkGrabPackage\NetworkGrabPackage.sln /m /p:Configuration=Debug /p:Platform=x64
```

## 使用说明（程序内菜单）

入口在 [Main.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Main.cpp)，启动后会进入主页菜单（见 [HomePage.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Render/HomePage.cpp)）。

### 1) 抓包测试（实时抓包）

路径：主页 → 抓包测试 →（输入过滤表达式 + 选择网卡）→ 开始抓包

- 过滤表达式来自 `pcap_compile`（BPF 语法），例如：
  - `tcp port 80`
  - `udp port 53`
  - `port 443`
  - `ip`
- 抓包过程中按 `q` 或 `Esc` 停止
- 抓到的数据包会自动写入 `NetworkGrabPackage/Output/`（相对路径使用 `../Output`）

#### BPF 过滤表达式速查

- 端口/协议
  - `port 443`：任意协议的 443 端口（TCP/UDP 都会匹配）
  - `tcp port 80`：仅 TCP 80
  - `udp port 53`：仅 UDP 53
- 地址
  - `host 192.168.1.10`：源或目的为该主机
  - `src host 192.168.1.10` / `dst host 192.168.1.10`：指定方向
  - `net 192.168.1.0/24`：指定网段
- 组合
  - `tcp and (port 80 or port 443)`
  - `udp and not port 53`

### 2) 流量分析（离线统计）

路径：主页 → 流量分析

- 程序会遍历 `NetworkGrabPackage/Output/` 下的所有 `.pcap` 文件进行统计
- 当前实现：从每个包中提取 IPv4 源/目的 IP、协议号，并对目的 IP 进行 A/B/C/D/E/L 分类汇总展示

### 3) http解析（离线 HTTP/HTTPS）

路径：主页 → http解析

- 自动列出 `../Output/`（或回退尝试 `./Output/`）下的 `.pcap` 文件供选择
- 对每个 TCP 包的 payload：
  - 能识别 HTTP：展示 Request/Response 行、Headers、Body 概览
  - 不能识别 HTTP：尝试从 TLS ClientHello 中提取 SNI（域名）

## 输出文件说明

- 输出目录：`NetworkGrabPackage/Output/`
- 写盘时机：实时抓包每抓到一个包就会写出一个 `.pcap`
- 文件命名：`YYYY-MM-DD_HH-MM-SS-mmm.pcap`（毫秒来自 pcap 时间戳的微秒字段折算）

## 核心流程

本项目的控制流由“页面栈 + 页面 display()”驱动：

- 入口：`main()` → `Application::StartApplication()` → `HomePage::display()`
  - [Main.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Main.cpp)
  - [Application.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Core/Application.cpp)
  - [Pages.hpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Render/Pages.hpp)

### 实时抓包路径

- 选择网卡：`DevicePage` 通过 `NetworkInterface` 枚举设备并让用户选择
  - [DevicePage.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Render/DevicePage.cpp)
  - [NetworkInterface.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/NetWork/NetworkInterface.cpp)
- 开始抓包：`CapturePage` 启动线程调用 `PacketCapture::startCapture(device, filter)`
  - [CapturePage.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Render/CapturePage.cpp)
  - [PacketCapture.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Core/PacketCapture.cpp)
- 设置过滤器：`PacketFilter::setFilter` → `pcap_compile` → `pcap_setfilter`
  - [PacketFilter.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Core/PacketFilter.cpp)
- 单包解析：抓到一个包后由 `PacketFactory::createPacket` 创建对应协议对象并调用 `Parse()`
  - [PacketFactory.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Data/PacketFactory.cpp)
  - [TCP.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Data/TCP.cpp)、[UDP.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Data/UDP.cpp)、[IP.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Data/IP.cpp)
- 写盘：每抓到一个包就写出一个 `.pcap` 文件到 `../Output/`
  - [PacketCapture::dumpPacket](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Core/PacketCapture.cpp)

### 离线分析路径

- 流量分析：遍历 `Output/` 下 `.pcap` → `pcap_open_offline` → 解析 IPv4 → 生成报表
  - [TrafficStatistics.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Statistics/TrafficStatistics.cpp)
- HTTP/HTTPS：遍历选定 `.pcap` → 找 TCP → 尝试按 HTTP 解析 payload；否则尝试解析 TLS ClientHello SNI
  - [HttpPage.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Render/HttpPage.cpp)
  - [HTTP.cpp](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/Data/HTTP.cpp)

## 目录结构

主工程位于 `NetworkGrabPackage/NetworkGrabPackage/`：

- `Core/`：抓包与过滤（`pcap_open_live`、应用 BPF、写入 `.pcap`）
- `NetWork/`：网卡枚举与子网掩码获取（`pcap_findalldevs`）
- `Data/`：协议数据结构与解析（Ethernet / IP(v4) / TCP / UDP / HTTP）
- `Render/`：控制台交互与页面（主页、选择菜单、HTTP 分析页等）
- `Statistics/`：离线统计与报告（遍历 Output 目录并生成统计表）

工程根目录下的 `NetworkGrabPackage/` 还包含：

- `Include/`：第三方头文件（pcap、openssl、curl、cpr、cpprest 等）
- `Lib/x64/`：第三方库文件（`wpcap.lib`、`Packet.lib`、`zlibd.lib`、`cpr.lib`、`libcurl-d.lib`、`libssl.lib` 等）
- `Output/`：抓包写盘与离线分析的默认目录（仓库内自带部分示例 `.pcap`）

## 依赖与运行时

- 构建期依赖
  - 本仓库已内置第三方头文件（`NetworkGrabPackage/Include/`）与 x64 库（`NetworkGrabPackage/Lib/x64/`），工程在 x64 配置下会自动引用这些路径
- 运行期依赖
  - 抓包与离线解析都依赖 `wpcap`/`Packet` 的运行时 DLL，通常由安装 Npcap 提供
  - 抓取部分可能需要管理员权限（与网卡驱动/系统策略有关）

### 工程链接库（x64）

工程在 x64 下默认链接（见 [NetworkGrabPackage.vcxproj](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/NetworkGrabPackage/NetworkGrabPackage.vcxproj)）：

- `wpcap.lib` / `Packet.lib`：抓包与离线 pcap 读取
- `Ws2_32.lib`：网络相关基础库
- `zlibd.lib`：压缩相关（当前 HTTP 解析中仅识别编码类型，未做解压）
- `cpr.lib` / `libcurl-d.lib`：HTTP 客户端相关（当前源码中未直接调用 cpr，库仍保留在工程依赖中）

## 二次开发

- 新增协议解析
  - 在 `NetworkGrabPackage/NetworkGrabPackage/Data/` 增加新的 `Packet` 子类并实现 `Parse()/display()/showSummary()`
  - 在 `Common/Types.hpp` 扩展 `PacketType` 枚举与 `PacketTypeToString`
  - 在 `Data/PacketFactory.cpp` 中补充识别逻辑与工厂创建逻辑
- 调整抓包过滤与输出
  - 抓包过滤表达式由 `Core/PacketFilter.cpp` 应用（BPF）；可在 `Render/DevicePage.cpp` 调整输入提示/默认值
  - 抓包写盘路径在 `Core/PacketCapture.cpp` 中生成（当前为相对 `../Output/`）
- 扩展统计报表
  - `Statistics/TrafficStatistics.cpp` 当前从 `.pcap` 中抽取 IPv4 地址与协议号，可在此基础上增加端口、流量字节数、TopN 等

## 已知限制

- IPv6：工厂层对 IPv6 有简单识别，但 `IP` 解析器当前只支持 IPv4
- HTTP：当前基于 TCP 单包 payload 解析，未做 TCP 流重组；分片/跨包的 HTTP 报文可能解析不到
- 写盘策略：实时抓包会为每个抓到的数据包写出一个 `.pcap` 文件，长时间抓包会产生大量小文件

## 常见问题

- 运行时报 “找不到 wpcap.dll / Packet.dll”
  - 安装 Npcap，并确保系统路径中能找到其 DLL（通常安装后自动处理）
- “Could not open device …”
  - 确认已安装 Npcap；尝试以管理员权限运行；选择正确的网卡（虚拟网卡/禁用网卡可能不可用）
- “Could not compile filter …”
  - 输入的是 BPF 表达式而不是纯端口号；例如把 `80` 改为 `tcp port 80` 或 `port 80`
- http解析页看不到文件
  - 把要分析的 `.pcap` 放到 `NetworkGrabPackage/Output/`；或确认运行时工作目录使得 `../Output/` 路径可达
- 输出目录为空但仓库里有示例 pcap
  - 仓库示例位于 [Output/](file:///f:/github/NetworkPackageGrab/NetworkGrabPackage/Output/)；运行时请确保程序工作目录能正确解析 `../Output/`

## 许可

当前仓库未包含 `LICENSE` 文件；默认视为保留所有权利（All Rights Reserved）。如需开源发布，请补充许可证并同步第三方依赖的许可说明。

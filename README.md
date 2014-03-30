Quicker
=======

    用于快速启动文件夹、注册表编辑器以及其它常用命令。

Power
=====

    控制主机进入待机或者休眠。

    语法：

        Power.exe [-suspend | -hibernate]

    示例：

        进入待机：

            Power -suspend

        进入休眠：

            Power -hibernate

SendBreak
=========

    向指定的控制台窗口发送 Ctrl+C 命令。

    语法：

        SendBreak.exe -pidfile <pidfile> | -pid <pid> | -title <title>

    示例：

        指定 pid 文件：

            SendBreak -pidfile "D:\3party\Apache2.2.14\logs\httpd.pid"

        指定 pid：

            SendBreak -pid 1320

        指定窗口标题：

            SendBreak -title "Apache HTTPd:80"

Speaker
=======

    控制主机音响设备静音。

    语法：

        Speaker.exe [-on | -off]

    示例：

        静音：

            Speaker -off

        取消静音：

            Speaker -on

        静音状态切换：

            Speaker

WinMin
======

    当前窗口最小化。

    语法：

        WinMin.exe

WinRect
=======

    对桌面上符合指定条件的可见、可调尺寸的窗口统一调整尺寸。

    语法：

        WinRect.exe [-wc <WindowClassName>] [-width <width> | -ex <width>] [-height <height> | -ey <height>] [-nolimit]

    示例：

        将 QQ 聊天窗口指定为特定尺寸：

            WinRect -wc TXGuiFoundation -width 900 -height 600

        将所有资源管理器窗口加宽、加高，且不受屏幕边界限制：

            WinRect -wc ExploreWClass -ex 100 -ey 60 -nolimit

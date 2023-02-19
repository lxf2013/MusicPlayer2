# MusicPlayer2
一个Rainmeter插件，基于[MusicPlayer](https://github.com/df32/Rainmeter_Plugins/blob/master/Doc/MusicPlayer_1.2.txt)开发，在原来播放控制的基础上增加歌词，专辑搜索与下载，歌词解析与控制功能

使用了filesystem，所以C++标准最低要达到c++17

## 快速开始
```

==========================Manager，管理整个插件的配置，播放器信息的获取，歌词，专辑的获取=====================
[PlayerCenter]
Measure=Plugin
Plugin=MusicPlayer2.dll
Type=Manager

; QQ音乐为 QQMusic
; 酷我音乐为 KwMusic
; 酷狗音乐为 KgMusic
; 百度音乐为 BaiduMusic
PlayerName=QQMusic


=======================歌曲信息获取与显示============================
; 获得歌手名和歌曲名
[Track]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter
PlayerType=Track

; 歌曲信息显示
[Show]
Meter=String
MeasureName=Track
StringEffect=SHADOW
StringStyle=BOLD
FontSize=15
FontColor=255.255.255


===============================歌词获取与显示================================
; 获取当前歌词，歌词会自动联网下载，缓存在本地，下一次直接从本地打开
[Lyric0]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter

; lrc_6 当前播放歌词的第前6句歌词
; lrc_5 当前播放歌词的第前5句歌词
; lrc_4 当前播放歌词的第前4句歌词
; lrc_3 当前播放歌词的第前3句歌词
; lrc_2 当前播放歌词的第前2句歌词
; lrc_1 当前播放歌词的第前1句歌词
; lrc_0 当前播放的歌词
; lrc1 当前播放歌词的第后1句歌词
; lrc2 当前播放歌词的第后3句歌词
; lrc3 当前播放歌词的第后3句歌词
; lrc4 当前播放歌词的第后4句歌词
; lrc5 当前播放歌词的第后5句歌词
; lrc6 当前播放歌词的第后6句歌词
; lrc7 当前播放歌词的第后7句歌词
; lrc8 当前播放歌词的第后8句歌词
; lrc9 当前播放歌词的第后9句歌词
Type=lrc0


; 显示当前歌词
[ShowLyric0]
Meter=String
MeasureName=Lyric0
StringEffect=SHADOW
StringStyle=BOLD
FontSize=15
FontColor=2.238.251
Y=94


==================================专辑图片获取与显示===========================
; 专辑图片获取，会返回路径，图片会自动联网下载，缓存在本地，下一次直接本地读取
[Cover]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter
Type=Cover

; 专辑显示
[ShowCover]
Meter=Image
ImageName=[Cover]
W=120
Y=150
UpdateDivider=1
DynamicVariables=1
```
更详细的信息请下载发布的[皮肤](https://github.com/lxf2013/MusicPlayer2/releases/tag/MusicPlayer2)，里面有详细的示例

## Rainmeter Plugin开发
[Rainmeter SDK](https://github.com/rainmeter/rainmeter-plugin-sdk)

[Rainmeter 开发文档](https://docs.rainmeter.net/developers/plugin/cpp/)
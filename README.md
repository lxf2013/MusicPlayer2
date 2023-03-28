# MusicPlayer2
一个Rainmeter插件，基于[MusicPlayer](https://github.com/df32/Rainmeter_Plugins/tree/master/MusicPlayer)开发，在原来播放控制的基础上增加歌词，专辑搜索与下载，缓存（下载时进行相似度匹配，过滤掉错误的歌词，采用LRU缓存），歌词解析与控制，声音检测功能

## 效果图

![图片](./image.png)

## 快速开始
```ini
[Rainmeter]
; 建议值，太大导致歌词，播放状态等更新太慢
Update=100

==========================Manager，管理整个插件的配置，播放器信息的获取，歌词，专辑的获取=====================
[PlayerCenter]
Measure=Plugin
Plugin=MusicPlayer2.dll
Type=Manager

; QQ音乐为 QQMusic
; 酷我音乐为 KwMusic
; 酷狗音乐为 KgMusic
; 百度音乐为 BaiduMusic
; 网易云音乐为 CloudMusic
PlayerName=QQMusic

=======================歌曲信息获取与显示============================
; 获得歌手名和歌曲名
[Track]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter
Type=Track

; 歌曲信息显示
[Show]
Meter=String
MeasureName=Track
X=125
Y=10

===============================歌词获取与显示================================
; 获取当前歌词，歌词会自动联网下载，缓存在本地，下一次直接从本地打开
[Lyric0]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter

; lrc_0 当前播放的歌词
Type=lrc0

; 显示当前歌词
[ShowLyric0]
Meter=String
MeasureName=Lyric0
Y=40
X=125

; 获取当前歌词翻译
[TLyric0]
Measure=Plugin
Plugin=MusicPlayer2.dll

;指定管理者(必须的)
Manager=PlayerCenter

; tlrc_0 当前播放歌词的翻译
Type=tlrc0

; 显示当前歌词的翻译
[ShowTLyric0]
Meter=String
MeasureName=TLyric0
Y=70
X=125

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
DynamicVariables=1
```
更详细的信息请下载发布的[皮肤](https://github.com/lxf2013/MusicPlayer2/releases)，里面有详细的示例（**示例都是GB2312编码格式，如果打开是乱码请切换**），[使用文档](./Example/readme.md)，[示例](./Example)

## Rainmeter Plugin开发

[Rainmeter SDK](https://github.com/rainmeter/rainmeter-plugin-sdk)

[Rainmeter 开发文档](https://docs.rainmeter.net/developers/plugin/cpp/)

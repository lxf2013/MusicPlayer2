<center>MusicPlayer2使用手册</center>

## 示例文件

[快速开始(快速上手)](快速开始.ini)

[示例(所有特性)](示例.ini)

[示例-优化(对CPU进行优化)](示例-优化.ini)

歌词,播放器分离(对CPU优化的基础上分开播放器和歌词) ([启动文件](播放器，歌词分离/启动.ini) [分离的播放器](播放器，歌词分离/播放器/播放器.ini) [分离的歌词](播放器，歌词分离/歌词/歌词.ini))



## 主Measure

```ini
[PlayerCenter]
Measure=Plugin
Plugin=MusicPlayer2.dll
Type=Manager
PlayerName=QQMusic
```

管理整个插件的配置，及所有信息的更新，如歌词更新，专辑更新，所以不能将雨滴的更新时间设置太长

相关配置
|是否必须|配置|值|描述|
|:---:|:---:|:---:|:---:|
|必须|PlayerName|QQMusic（QQ音乐），KwMusic（酷我音乐），KgMusic（酷狗音乐），BaiduMusic（百度音乐），CloudMusic（网易云音乐）|指定播放器类型|
|可选|LyricOffset|-1500（默认）|歌词距开始的偏移，负数表示提前，正数表示延后，歌词不同步时可调节|
|可选|DownloadPath|download（默认）|歌词，专辑的下载路径|
|可选|Similarity|1.4（默认）|相似度，0-2之间，歌词错误或歌词找不到时可调节|
|可选|Capacity|100（默认）|缓存歌词和专辑图的最大个数|
|可选|DefaultCover||默认的专辑图，当没有专辑时显示|
|可选|DefaultLyric||默认的歌词，当没有歌词时显示|



## 播放器状态的获取

```ini
; 获得播放器状态
[Status]
Measure=Plugin
Plugin=MusicPlayer2.dll
;指定管理者(必须的)，支持相对路径如：..\PlayerCenter(上层目录下的Manager)，绝对路径如：\MusicPlayer2-3.2.0\播放器，歌词分离\PlayerCenter(\MusicPlayer2-3.2.0\播放器，歌词分离\ 目录下的Manager)，此处 PlayerCenter表示当前目录下的Manager
Manager=PlayerCenter
Type=Status
;播放时的行为(可选)
;PlayAction=[!UpdateMeter ShowStatus]
;停止后的行为(可选)
;StopAction=[!UpdateMeter ShowStatus]

; 播放器状态显示（0停止，1播放中）
[ShowStatus]
Meter=String
MeasureName=Status
```



## 歌曲信息的获取

```ini
; 获得歌手名和歌曲名
[Track]
Measure=Plugin
Plugin=MusicPlayer2.dll
;指定管理者(必须的)，支持相对路径如：..\PlayerCenter(上层目录下的Manager)，绝对路径如：\MusicPlayer2-3.2.0\播放器，歌词分离\PlayerCenter(\MusicPlayer2-3.2.0\播放器，歌词分离\ 目录下的Manager)，此处 PlayerCenter表示当前目录下的Manager
Manager=PlayerCenter
Type=Track
;Track改变时的行为(可选)
;ChangeAction=[!UpdateMeter ShowTrack]

; 歌曲信息显示
[ShowTrack]
Meter=String
MeasureName=Track
```

|Type|描述|
|:---:|:---:|
|Title|获取歌曲名|
|Artist|获取歌手名|
|Track|获取歌曲名和歌手名|



## 专辑图获取

```ini
;专辑图获取
[Cover]
Measure=Plugin
Plugin=MusicPlayer2.dll
;指定管理者(必须的)，支持相对路径如：..\PlayerCenter(上层目录下的Manager)，绝对路径如：\MusicPlayer2-3.2.0\播放器，歌词分离\PlayerCenter(\MusicPlayer2-3.2.0\播放器，歌词分离\ 目录下的Manager)，此处 PlayerCenter表示当前目录下的Manager
Manager=PlayerCenter
Type=Cover
;Cover改变时的行为(可选)
;ChangeAction=[!UpdateMeter ShowCover]

; 专辑显示
[ShowCover]
Meter=Image
ImageName=[Cover]
W=120
DynamicVariables=1
UpdateDivider=1
```



## 歌词获取

```ini
;当前播放歌词获取
[Lyric0]
Measure=Plugin
Plugin=MusicPlayer2.dll
;指定管理者(必须的)，支持相对路径如：..\PlayerCenter(上层目录下的Manager)，绝对路径如：\MusicPlayer2-3.2.0\播放器，歌词分离\PlayerCenter(\MusicPlayer2-3.2.0\播放器，歌词分离\ 目录下的Manager)，此处 PlayerCenter表示当前目录下的Manager
Manager=PlayerCenter
; lrc_0 当前播放的歌词
Type=lrc0
;lrc0改变时的行为(可选)
;ChangeAction=[!UpdateMeter ShowLyric0]


; 显示当前歌词
[ShowLyric0]
Meter=String
MeasureName=Lyric0
```
|Type|描述|
|:---:|:---:|
|lrc_6|当前播放歌词的第前6句歌词|
|lrc_5|当前播放歌词的第前5句歌词|
|lrc_4|当前播放歌词的第前4句歌词|
|lrc_3|当前播放歌词的第前3句歌词|
|lrc_2|当前播放歌词的第前2句歌词|
|lrc_1|当前播放歌词的第前1句歌词|
|lrc_0|当前播放的歌词|
|lrc1|当前播放歌词的第后1句歌词|
|lrc2|当前播放歌词的第后3句歌词|
|lrc3|当前播放歌词的第后3句歌词|
|lrc4|当前播放歌词的第后4句歌词|
|lrc5|当前播放歌词的第后5句歌词|
|lrc6|当前播放歌词的第后6句歌词|
|lrc7|当前播放歌词的第后7句歌词|
|lrc8|当前播放歌词的第后8句歌词|
|lrc9|当前播放歌词的第后9句歌词|
|tlrc_6|当前播放歌词的第前6句歌词的翻译|
|tlrc_5|当前播放歌词的第前5句歌词的翻译|
|tlrc_4|当前播放歌词的第前4句歌词的翻译|
|tlrc_3|当前播放歌词的第前3句歌词的翻译|
|tlrc_2|当前播放歌词的第前2句歌词的翻译|
|tlrc_1|当前播放歌词的第前1句歌词的翻译|
|tlrc_0|当前播放的歌词的翻译|
|tlrc1|当前播放歌词的第后1句歌词的翻译|
|tlrc2|当前播放歌词的第后3句歌词的翻译|
|tlrc3|当前播放歌词的第后3句歌词的翻译|
|tlrc4|当前播放歌词的第后4句歌词的翻译|
|tlrc5|当前播放歌词的第后5句歌词的翻译|
|tlrc6|当前播放歌词的第后6句歌词的翻译|
|tlrc7|当前播放歌词的第后7句歌词的翻译|
|tlrc8|当前播放歌词的第后8句歌词的翻译|
|tlrc9|当前播放歌词的第后9句歌词的翻译|



## 播放器控制

```ini
; 鼠标左键播放/暂停
[Stop]
Meter=String
Text=播放/暂停
LeftMouseUpAction=[!CommandMeasure PlayerCenter PlayPause]
```
|命令|描述|
|:---:|:---:|
|Open|打开播放器|
|Close|关闭播放器|
|PlayPause|播放暂停|
|Stop|停止播放|
|Previous|上一首|
|Next|下一首|
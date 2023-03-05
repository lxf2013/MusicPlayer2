
* 主Measure
>```ini
>[PlayerCenter]
>Measure=Plugin
>Plugin=MusicPlayer2.dll
>Type=Manager
>PlayerName=QQMusic
>```
>
>管理整个插件的配置，及所有信息的更新，如歌词更新，专辑更新，所以不能将更新间隔设置太长，尤其不能设置为 UpdateDivider=-1
>
>相关配置
>|是否必须|配置|值|描述|
>|:---:|:---:|:---:|:---:|
>|必须|PlayerName|QQMusic（默认），KwMusic，KgMusic，BaiduMusic|指定播放器类型|
>|可选|LyricOffset|-1500（默认）|歌词距开始的偏移，负数表示提前，正数表示延后|
>|可选|DownloadPath|download（默认）|歌词，专辑的下载路径|
>|可选|Similarity|1.6（默认）|相似度，0-2之间，可用于解决歌词搜索错误的问题|
>|可选|Capacity|100（默认）|缓存最大个数|
>|可选|DefaultCover||默认的专辑图，当没有专辑时显示|
>|可选|DefaultLyric||默认的歌词，当没有歌词时显示|
>|可选|TrackChangeAction||歌曲改变时行为|
>|可选|LyricChangeAction||歌词改变时行为|
>|可选|CoverChangeAction||专辑图改变时行为|



* 播放器信息的获取
>```ini
>; 获得歌手名和歌曲名
>[Track]
>Measure=Plugin
>Plugin=MusicPlayer2.dll
>;指定管理者(必须的)
>Manager=PlayerCenter
>Type=Track
>
>; 歌曲信息显示
>[Show]
>Meter=String
>MeasureName=Track
>```
>
>|Type|描述|
>|:---:|:---:|
>|Title|获取歌曲名|
>|Artist|获取歌手名|
>|Track|获取歌曲名和歌手名|
>|Status|播放器是否启动（1-是 0-否）|




* 专辑图获取
>```ini
>[Cover]
>Measure=Plugin
>Plugin=MusicPlayer2.dll
>;指定管理者(必须的)
>Manager=PlayerCenter
>Type=Cover
>
>; 专辑显示
>[ShowCover]
>Meter=Image
>ImageName=[Cover]
>W=120
>DynamicVariables=1
>```




* 歌词获取
>```ini
>[Lyric0]
>Measure=Plugin
>Plugin=MusicPlayer2.dll
>;指定管理者(必须的)
>Manager=PlayerCenter
>; lrc_0 当前播放的歌词
>Type=lrc0
>
>
>; 显示当前歌词
>[ShowLyric0]
>Meter=String
>MeasureName=Lyric0
>```
>|Type|描述|
>|:---:|:---:|
>|lrc_6|当前播放歌词的第前6句歌词|
>|lrc_5|当前播放歌词的第前5句歌词|
>|lrc_4|当前播放歌词的第前4句歌词|
>|lrc_3|当前播放歌词的第前3句歌词|
>|lrc_2|当前播放歌词的第前2句歌词|
>|lrc_1|当前播放歌词的第前1句歌词|
>|lrc_0|当前播放的歌词|
>|lrc1|当前播放歌词的第后1句歌词|
>|lrc2|当前播放歌词的第后3句歌词|
>|lrc3|当前播放歌词的第后3句歌词|
>|lrc4|当前播放歌词的第后4句歌词|
>|lrc5|当前播放歌词的第后5句歌词|
>|lrc6|当前播放歌词的第后6句歌词|
>|lrc7|当前播放歌词的第后7句歌词|
>|lrc8|当前播放歌词的第后8句歌词|
>|lrc9|当前播放歌词的第后9句歌词|
>|tlrc_6|当前播放歌词的第前6句歌词的翻译|
>|tlrc_5|当前播放歌词的第前5句歌词的翻译|
>|tlrc_4|当前播放歌词的第前4句歌词的翻译|
>|tlrc_3|当前播放歌词的第前3句歌词的翻译|
>|tlrc_2|当前播放歌词的第前2句歌词的翻译|
>|tlrc_1|当前播放歌词的第前1句歌词的翻译|
>|tlrc_0|当前播放的歌词的翻译|
>|tlrc1|当前播放歌词的第后1句歌词的翻译|
>|tlrc2|当前播放歌词的第后3句歌词的翻译|
>|tlrc3|当前播放歌词的第后3句歌词的翻译|
>|tlrc4|当前播放歌词的第后4句歌词的翻译|
>|tlrc5|当前播放歌词的第后5句歌词的翻译|
>|tlrc6|当前播放歌词的第后6句歌词的翻译|
>|tlrc7|当前播放歌词的第后7句歌词的翻译|
>|tlrc8|当前播放歌词的第后8句歌词的翻译|
>|tlrc9|当前播放歌词的第后9句歌词的翻译|





* 播放器控制
>```ini
>; 鼠标左键停止播放
>[Stop]
>Meter=String
>MeasureName=PlayerCenter
>LeftMouseUpAction=[!CommandMeasure PlayerCenter PlayPause]
>```
>|命令|描述|
>|:---:|:---:|
>|Open|打开播放器|
>|Close|关闭播放器|
>|PlayPause|播放暂停|
>|PlayPause|停止播放|
>|PlayPause|上一首|
>|Next|下一首|
>|VolumeUp|增大音量|
>|VolumeDown|减小音量|
>
>本插件基于MusicPlayer开发，更多MusicPlayer相关的信息请查看[MusicPlayer说明](https://github.com/df32/Rainmeter_Plugins/blob/master/Doc/MusicPlayer_1.2.txt)
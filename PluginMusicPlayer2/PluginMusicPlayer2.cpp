
#include "PluginMusicPlayer2.h"
#include "Core/Core.h"
#include "ThirdPart/RainmeterAPI.h"

static int measure_cnt = 0;

PLUGIN_EXPORT void Initialize(void** data, void* rm){
	Measure* measure = new Measure;
	*data = measure;
	measure->rm = rm;
	MemoryCount::AllocMeasure();
}

PLUGIN_EXPORT void Finalize(void* data){
	Measure* measure = (Measure*)data;
	Core::FreeData(measure);
	delete measure;
	MemoryCount::FreeMeasure();
}


PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue){
	struct Node{
		LPCWSTR type;
		Measure::MeasureType measure_type;
	};
	static Node arr_type[] = {
		{L"Title", Measure::MEASURE_TITLE},
		{L"Artist", Measure::MEASURE_ARTIST},
		{L"Track", Measure::MEASURE_TRACK},
		{L"Status", Measure::MEASURE_STATUS},
		{L"Cover", Measure::MEASURE_COVER},
		{L"Lrc_6", Measure::MEASURE_LRC_6},
		{L"Lrc_5", Measure::MEASURE_LRC_5},
		{L"Lrc_4", Measure::MEASURE_LRC_4},
		{L"Lrc_3", Measure::MEASURE_LRC_3},
		{L"Lrc_2", Measure::MEASURE_LRC_2},
		{L"Lrc_1", Measure::MEASURE_LRC_1},
		{L"Lrc0", Measure::MEASURE_LRC0},
		{L"Lrc1", Measure::MEASURE_LRC1},
		{L"Lrc2", Measure::MEASURE_LRC2},
		{L"Lrc3", Measure::MEASURE_LRC3},
		{L"Lrc4", Measure::MEASURE_LRC4},
		{L"Lrc5", Measure::MEASURE_LRC5},
		{L"Lrc6", Measure::MEASURE_LRC6},
		{L"Lrc7", Measure::MEASURE_LRC7},
		{L"Lrc8", Measure::MEASURE_LRC8},
		{L"Lrc9", Measure::MEASURE_LRC9},
		{L"TLrc_6", Measure::MEASURE_TLRC_6},
		{L"TLrc_5", Measure::MEASURE_TLRC_5},
		{L"TLrc_4", Measure::MEASURE_TLRC_4},
		{L"TLrc_3", Measure::MEASURE_TLRC_3},
		{L"TLrc_2", Measure::MEASURE_TLRC_2},
		{L"TLrc_1", Measure::MEASURE_TLRC_1},
		{L"TLrc0", Measure::MEASURE_TLRC0},
		{L"TLrc1", Measure::MEASURE_TLRC1},
		{L"TLrc2", Measure::MEASURE_TLRC2},
		{L"TLrc3", Measure::MEASURE_TLRC3},
		{L"TLrc4", Measure::MEASURE_TLRC4},
		{L"TLrc5", Measure::MEASURE_TLRC5},
		{L"TLrc6", Measure::MEASURE_TLRC6},
		{L"TLrc7", Measure::MEASURE_TLRC7},
		{L"TLrc8", Measure::MEASURE_TLRC8},
		{L"TLrc9", Measure::MEASURE_TLRC9}
	};

	Measure* measure = (Measure*)data;
	measure->rm = rm;
	measure->change_action = RmReadString(rm, L"ChangeAction", L"");
	measure->play_action = RmReadString(rm, L"PlayAction", L"");
	measure->stop_action = RmReadString(rm, L"StopAction", L"");

	LPCWSTR type = RmReadString(rm, L"Type", L"");
	if (_wcsicmp(L"Manager", type) == 0){
		measure->type = Measure::MEASURE_MANAGER;
		measure->name_space = Core::AllocData(measure);
	} else{
		for(int i=0; i<sizeof(arr_type)/sizeof(Node); ++i){
			if(_wcsicmp(arr_type[i].type, type) == 0){
				measure->type = arr_type[i].measure_type;
				break;
			}
		}
		if(measure->type != Measure::MEASURE_NONE){
			measure->name_space = Core::FlushData(measure);
		}else{
			std::wstring error = L"invalid Type: ";
			error += type;
			RmLog(rm, LOG_WARNING, error.c_str());
		}
	}
}

PLUGIN_EXPORT double Update(void* data){
	Measure* measure = (Measure*)data;
	if(measure->type == Measure::MEASURE_MANAGER){
		Core::Update(measure->name_space);
	}
	return 0.0;
}

PLUGIN_EXPORT LPCWSTR GetString(void* _data){
	Measure* measure = (Measure*)_data;
	CoreData *data = Core::GetData(measure->name_space);
	if(data == nullptr){
		return L"";
	}

	static WCHAR buffer[12];
	// LPCWSTR str;

	switch (measure->type){
		case Measure::MEASURE_TITLE:
			// {
			// 	const std::wstring &title = data->player->GetTitle();
			// 	return title.empty()? L"empty": title.c_str();
			// }
			return data->player->GetTitle().c_str();
		case Measure::MEASURE_ARTIST:
			// {
			// 	const std::wstring &artist = data->player->GetArtist();
			// 	return artist.empty()? L"empty": artist.c_str();
			// }
			return data->player->GetArtist().c_str();
		case Measure::MEASURE_TRACK:
			// {
			// 	const std::wstring &track = data->player->GetTrack();
			// 	return track.empty()? L"empty": track.c_str();
			// }
			return data->player->GetTrack().c_str();
		case Measure::MEASURE_STATUS:
			_itow_s(data->audio->GetStatus(), buffer, 10);
			return buffer;
		case Measure::MEASURE_COVER:
			return data->resource.GetCoverPath(data->player->GetTrack()).c_str();
		case Measure::MEASURE_LRC_6:
			return data->lyric.GetLyric(-6);
		case Measure::MEASURE_LRC_5:
			return data->lyric.GetLyric(-5);
		case Measure::MEASURE_LRC_4:
			return data->lyric.GetLyric(-4);
		case Measure::MEASURE_LRC_3:
			return data->lyric.GetLyric(-3);
		case Measure::MEASURE_LRC_2:
			return data->lyric.GetLyric(-2);
		case Measure::MEASURE_LRC_1:
			return data->lyric.GetLyric(-1);
		case Measure::MEASURE_LRC0:
			return data->lyric.GetLyric(0);
		case Measure::MEASURE_LRC1:
			return data->lyric.GetLyric(1);
		case Measure::MEASURE_LRC2:
			return data->lyric.GetLyric(2);
		case Measure::MEASURE_LRC3:
			return data->lyric.GetLyric(3);
		case Measure::MEASURE_LRC4:
			return data->lyric.GetLyric(4);
		case Measure::MEASURE_LRC5:
			return data->lyric.GetLyric(5);
		case Measure::MEASURE_LRC6:
			return data->lyric.GetLyric(6);
		case Measure::MEASURE_LRC7:
			return data->lyric.GetLyric(7);
		case Measure::MEASURE_LRC8:
			return data->lyric.GetLyric(8);
		case Measure::MEASURE_LRC9:
			return data->lyric.GetLyric(9);
		case Measure::MEASURE_TLRC_6:
			return data->lyric.GetTLyric(-6);
		case Measure::MEASURE_TLRC_5:
			return data->lyric.GetTLyric(-5);
		case Measure::MEASURE_TLRC_4:
			return data->lyric.GetTLyric(-4);
		case Measure::MEASURE_TLRC_3:
			return data->lyric.GetTLyric(-3);
		case Measure::MEASURE_TLRC_2:
			return data->lyric.GetTLyric(-2);
		case Measure::MEASURE_TLRC_1:
			return data->lyric.GetTLyric(-1);
		case Measure::MEASURE_TLRC0:
			return data->lyric.GetTLyric(0);
		case Measure::MEASURE_TLRC1:
			return data->lyric.GetTLyric(1);
		case Measure::MEASURE_TLRC2:
			return data->lyric.GetTLyric(2);
		case Measure::MEASURE_TLRC3:
			return data->lyric.GetTLyric(3);
		case Measure::MEASURE_TLRC4:
			return data->lyric.GetTLyric(4);
		case Measure::MEASURE_TLRC5:
			return data->lyric.GetTLyric(5);
		case Measure::MEASURE_TLRC6:
			return data->lyric.GetTLyric(6);
		case Measure::MEASURE_TLRC7:
			return data->lyric.GetTLyric(7);
		case Measure::MEASURE_TLRC8:
			return data->lyric.GetTLyric(8);
		case Measure::MEASURE_TLRC9:
			return data->lyric.GetTLyric(9);
	}

	return nullptr;
}


PLUGIN_EXPORT void ExecuteBang(void* _data, LPCWSTR args){
	Measure* measure = (Measure*)_data;
	CoreData *data = Core::GetData(measure->name_space);
	if(data == nullptr){
		return;
	}
	
	if (!_wcsnicmp(args, L"Open",4)){
		data->player->OpenPlayer();
	}else if (data->player->GetStatus()){
		if (!_wcsnicmp(args, L"Close",5)){
			data->player->ClosePlayer();
		}else if (!_wcsicmp(args, L"PlayPause")){
			data->player->PlayPause();
			data->lyric.StopOrStart();
		}else if (!_wcsicmp(args, L"Stop")){
			data->player->Stop();
			data->lyric.Stop();
		}else if (!_wcsicmp(args, L"Previous")){
			data->player->Previous();
			data->lyric.Start();
		}else if (!_wcsicmp(args, L"Next")){
			data->player->Next();
			data->lyric.Start();
		}
	}else{
		RmLog(measure->rm, LOG_WARNING, L"Player isn't running");
	}
}

#pragma once

#include <string>

struct Measure{
public:
	enum MeasureType{
		MEASURE_NONE,
		MEASURE_MANAGER,
		MEASURE_TITLE,
		MEASURE_ARTIST,
		MEASURE_TRACK,
		MEASURE_STATUS,
		MEASURE_COVER,
		MEASURE_LRC_6,
		MEASURE_LRC_5,
		MEASURE_LRC_4,
		MEASURE_LRC_3,
		MEASURE_LRC_2,
		MEASURE_LRC_1,
		MEASURE_LRC0,
		MEASURE_LRC1,
		MEASURE_LRC2,
		MEASURE_LRC3,
		MEASURE_LRC4,
		MEASURE_LRC5,
		MEASURE_LRC6,
		MEASURE_LRC7,
		MEASURE_LRC8,
		MEASURE_LRC9,
		MEASURE_TLRC_6,
		MEASURE_TLRC_5,
		MEASURE_TLRC_4,
		MEASURE_TLRC_3,
		MEASURE_TLRC_2,
		MEASURE_TLRC_1,
		MEASURE_TLRC0,
		MEASURE_TLRC1,
		MEASURE_TLRC2,
		MEASURE_TLRC3,
		MEASURE_TLRC4,
		MEASURE_TLRC5,
		MEASURE_TLRC6,
		MEASURE_TLRC7,
		MEASURE_TLRC8,
		MEASURE_TLRC9,
	};
public:
	MeasureType  type = MEASURE_NONE;
	void        *rm = nullptr;
	std::wstring name_space;
	std::wstring change_action;
	std::wstring play_action;
	std::wstring stop_action;
};

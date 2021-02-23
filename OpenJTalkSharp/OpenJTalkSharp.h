// OpenJTalkSharp.h

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"

using namespace System;

namespace OpenJTalkSharp {

	public ref class OpenJTalk
	{
		// TODO: このクラスの、ユーザーのメソッドをここに追加してください。
	private:
		Mecab *mecab;
		NJD *njd;
		JPCommon *jpcommon;
		HTS_Engine *engine;

	public:
		System::String ^mecab_path;
		System::String ^udic_path;
		System::String ^voice_path;
		int	sampling_rate;
		int	fperiod;
		double alpha;
		int stage;
		double beta;
		int audio_buff_size;
		double uv_threshold;
		double gv_weight_mgc;
		double gv_weight_lf0;
		double gv_weight_lpf;
		
		HTS_Boolean use_lpf;
		HTS_Boolean use_log_gain;

		OpenJTalk();

		void initialize();
		
		void clear();
		
		void load();

		int talk(String^ text);

		int synthesis(String ^text, String ^wavefile);
	};
}

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
	protected:
		Mecab *mecab;
		NJD *njd;
		JPCommon *jpcommon;
		HTS_Engine *engine;

		void clear();

		//現状、未使用
		double gv_weight_lf0;
		double gv_weight_lpf;
		//現状、未使用
		HTS_Boolean use_lpf;
		HTS_Boolean use_log_gain;

	public:
		
		OpenJTalk(System::String^ voice_path, System::String^ mecab_path);
		OpenJTalk(System::String ^voice_path, System::String^ mecab_path, System::String^ udic_path);
		~OpenJTalk();
		!OpenJTalk();

		property int sampling_rate {
			void set(int val);
		}

		property int fperiod {
			void set(int val);
		}

		property int audio_buff_size {
			void set(int val);
		}

		property double alpha {
			void set(double val);
		}

		property double beta {
			void set(double val);
		}
		
		property double uv_threshold {
			void set(double val);
		}

		property double gv_weight_mgc {
			void set(double val);
		}

		property double speed {
			void set(double val);
		}

		property double half_tone {
			void set(double val);
		}

		int talk(String^ text);

		int synthesis(String ^text, String ^wavefile);

	};
}

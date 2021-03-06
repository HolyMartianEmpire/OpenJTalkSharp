// これは メイン DLL ファイルです。


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <atlstr.h>


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

#include <vcclr.h>
#include <msclr/marshal.h>

#include "OpenJTalkSharp.h"

using namespace System::Text;
using namespace System::Runtime::InteropServices;

namespace OpenJTalkSharp{

	OpenJTalk::OpenJTalk(System::String^ voice_path, System::String^ mecab_path, System::String^ udic_path)
	{
		char* dn_mecab = (mecab_path == nullptr) ? NULL : (char*)Marshal::StringToHGlobalAnsi(mecab_path).ToPointer();
		char* dn_udic = (udic_path == nullptr) ? NULL : (char*)Marshal::StringToHGlobalAnsi(udic_path).ToPointer();
		char* fn_voice = (voice_path == nullptr) ? NULL : (char*)Marshal::StringToHGlobalAnsi(voice_path).ToPointer();

		mecab = (Mecab *)calloc(1,sizeof(Mecab));
		njd   = (NJD *)calloc(1,sizeof(NJD));
		jpcommon = (JPCommon *)calloc(1,sizeof(JPCommon));
		engine   = (HTS_Engine *)calloc(1,sizeof(HTS_Engine));

		Mecab_initialize(mecab);
		NJD_initialize(njd);
		JPCommon_initialize(jpcommon);
		HTS_Engine_initialize(engine);

		Mecab_load(mecab, dn_mecab, dn_udic);

		HTS_Engine_load(engine, &fn_voice, 1);

		Marshal::FreeHGlobal((IntPtr)fn_voice);
		Marshal::FreeHGlobal((IntPtr)dn_mecab);
		Marshal::FreeHGlobal((IntPtr)dn_udic);
		
		sampling_rate = 16000;
		fperiod = 80;
		alpha = 0.42;
		beta = 0.0;
		audio_buff_size = 0;
		uv_threshold = 0.5;
		gv_weight_mgc = 1.0;
		gv_weight_lf0 = 1.0;
		gv_weight_lpf = 1.0;
		use_log_gain = FALSE;
		use_lpf = FALSE;
		speed = 1.0;
		half_tone = 0;
	}

	OpenJTalk::OpenJTalk(System::String^ voice_path, System::String^ mecab_path) : OpenJTalk(voice_path, mecab_path, nullptr)
	{
		//特別なものなし
	}

	OpenJTalk::~OpenJTalk() 
	{
		this->!OpenJTalk();
	}

	OpenJTalk::!OpenJTalk()
	{
		clear();
	}
	
	void OpenJTalk::sampling_rate::set(int val)
	{
		HTS_Engine_set_sampling_frequency(engine, val);
	}

	void OpenJTalk::fperiod::set(int val)
	{
		HTS_Engine_set_fperiod(engine, val);
	}

	void OpenJTalk::audio_buff_size::set(int val)
	{
		HTS_Engine_set_audio_buff_size(engine, val);
	}
	
	void OpenJTalk::alpha::set(double val)
	{
		HTS_Engine_set_alpha(engine, val);
	}

	void OpenJTalk::beta::set(double val)
	{
		HTS_Engine_set_beta(engine, val);
	}
	
	void OpenJTalk::uv_threshold::set(double val)
	{
		HTS_Engine_set_msd_threshold(engine, 1, val);
	}

	void OpenJTalk::gv_weight_mgc::set(double val)
	{
		HTS_Engine_set_gv_weight(engine, 1, val);
	}

	void OpenJTalk::speed::set(double val)
	{
		HTS_Engine_set_speed(engine, val);
	}

	void OpenJTalk::half_tone::set(double val)
	{
		HTS_Engine_add_half_tone(engine, val);
	}

	int OpenJTalk::talk(String^ text) {
		return synthesis(text, nullptr);
	}

	int OpenJTalk::synthesis(String ^text,String ^wavefile) {
		
		array<Byte>^ p = Encoding::UTF8->GetBytes(text);
		pin_ptr<Byte> pp = &p[0];
		char* txt = (char*)pp;

		char* buff = new char[10 * 1024];	//10MB
		int	time = 0;
		
		text2mecab(buff, txt);						//Mecab用に文字を変更
		Mecab_analysis(mecab, buff);				//Mecabで解析
		mecab2njd(njd, Mecab_get_feature(mecab),
			Mecab_get_size(mecab));					//Mecab解析結果でNJD似変換

		njd_set_pronunciation(njd);					//発音を設定
		njd_set_digit(njd);							//数字の発音？
		njd_set_accent_phrase(njd);					//アクセント記号の設定
		njd_set_accent_type(njd);					//アクセントタイプの設定
		njd_set_unvoiced_vowel(njd);				//母音の設定
		njd_set_long_vowel(njd);					//長音記号
		njd2jpcommon(jpcommon, njd);				//?
		JPCommon_make_label(jpcommon);				//?
		
		if (JPCommon_get_label_size(jpcommon) > 2) {
			if (wavefile != nullptr) {
				HTS_Engine_set_audio_buff_size(engine, 0);
			}
			else {
				int rate = HTS_Engine_get_sampling_frequency(engine);
				HTS_Engine_set_audio_buff_size(engine, rate);
			}

			//合成
			HTS_Engine_synthesize_from_strings
				(engine,
					JPCommon_get_label_feature(jpcommon),
					JPCommon_get_label_size(jpcommon));
			
			time = (int)Math::Ceiling((double)engine->gss.total_nsample /engine->condition.sampling_frequency);
			if( wavefile != nullptr ){
				char *fname = (char *)Marshal::StringToHGlobalAnsi(wavefile).ToPointer();
				FILE* wavfp;

				fopen_s(&wavfp,fname, "wb");
				if( wavfp != NULL ){
					HTS_Engine_save_riff(engine, wavfp);
					fclose(wavfp);
				}

				Marshal::FreeHGlobal((IntPtr)fname);
			}

			HTS_Engine_refresh(engine);
		}
		
		JPCommon_refresh(jpcommon);
		NJD_refresh(njd);
		Mecab_refresh(mecab);
		
		return time;
	}

	void OpenJTalk::clear() {
		
		Mecab_clear(mecab);
		NJD_clear(njd);
		JPCommon_clear(jpcommon);
		HTS_Engine_clear(engine);

		return ;
	}
}


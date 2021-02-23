using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenJTalkSharp;

namespace NJTalk
{
    class Program
    {
        static void Main(string[] args)
        {
            String base_path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            String dictionary = base_path + @"\dic";
            String voice_data = base_path + @"\voice\nitech_jp_atr503_m001.htsvoice";
            String wave_path = base_path + @"\test.wav";

            if (args.Length < 1)
                return;

            OpenJTalk ojt = new OpenJTalk()
            {
                mecab_path = dictionary,
                voice_path = voice_data,
                alpha = 0.1,
                beta = 0.1,
                fperiod = 80,
                sampling_rate = 16000,
                uv_threshold = 0,
            };

            ojt.initialize();
            ojt.load();
            //ojt.talk(args[0]);
            ojt.synthesis(args[0], wave_path);
            ojt.clear();
        }
    }
}

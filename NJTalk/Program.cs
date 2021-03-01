using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using OpenJTalkSharp;

namespace NJTalk
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
                return;

            OpenJTalk ojt = new OpenJTalk()
            {
                mecab_path = ConfigurationManager.AppSettings["dictionary"],
                voice_path = ConfigurationManager.AppSettings["voice"],
                alpha = double.Parse(ConfigurationManager.AppSettings["alpha"]),
                beta = double.Parse(ConfigurationManager.AppSettings["beta"]),
                fperiod = int.Parse(ConfigurationManager.AppSettings["fperiod"]),
                sampling_rate = int.Parse(ConfigurationManager.AppSettings["sampling_rate"]),
                uv_threshold = double.Parse(ConfigurationManager.AppSettings["uv_threshold"]),
            };

            //設定値反映
            ojt.load();

            if( args.Length >= 2)
            {
                var wave_path = args[1];
                ojt.synthesis(args[0], wave_path);
            }
            else
            {
                ojt.talk(args[0]);
            }
        }
    }
}

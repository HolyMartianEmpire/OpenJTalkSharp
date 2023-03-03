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

            var voice_path = ConfigurationManager.AppSettings["voice"];
            var mecab_path = ConfigurationManager.AppSettings["dictionary"];
            var udic_path = ConfigurationManager.AppSettings["user_dic"];

            using (OpenJTalk ojt = new OpenJTalk(voice_path, mecab_path, udic_path))
            {
                ojt.alpha = double.Parse(ConfigurationManager.AppSettings["alpha"]);
                ojt.beta = double.Parse(ConfigurationManager.AppSettings["beta"]);
                ojt.fperiod = int.Parse(ConfigurationManager.AppSettings["fperiod"]);
                ojt.sampling_rate = int.Parse(ConfigurationManager.AppSettings["sampling_rate"]);
                ojt.uv_threshold = double.Parse(ConfigurationManager.AppSettings["uv_threshold"]);

                if (args.Length >= 2)
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
}

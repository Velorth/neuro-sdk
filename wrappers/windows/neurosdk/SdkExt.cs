namespace Neuro
{
    internal class SdkExt
    {
#if AnyCPU
        private const string Platform = "x86";
#elif X64
        private const string Platform = "x64";
#endif

#if DEBUG
        public const string LibName = "sdk-extensions-" + Platform + "d.dll";
#else
        public const string LibName = "sdk-extensions-" + Platform + ".dll";
#endif
    }
}

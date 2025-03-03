#if ANDROID
using Android.Util;
#endif
namespace MonoHelloWorld
{
    public static class Program
    {
        public static void Main()
        {
            // Console.WriteLine("Hello Mono World");
            // Console.ReadLine();
#if ANDROID
            Log.Info("MonoAndroid", "✅ Hello từ Mono trên Android!");
#endif
        }
    }
}
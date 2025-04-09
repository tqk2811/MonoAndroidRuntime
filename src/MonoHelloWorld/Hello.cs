#if ANDROID
using Android.Media.Projection;
using Android.Runtime;
using Android.Util;
using Android.OS;
using Android.App;
using Android.Content;
using Android.Icu.Text;
using Android.Icu.Util;
#endif
using System;
using System.Runtime.InteropServices;
namespace MonoHelloWorld
{
    public static class Program
    {
        public static void Main()
        {
#if ANDROID
            //var vibrator = (Vibrator)Application.Context.GetSystemService(Context.VibratorManagerService)!;
            //vibrator.Vibrate(VibrationEffect.CreateOneShot(1000, VibrationEffect.DefaultAmplitude));

            IntPtr powerManagerClass = JNIEnv.FindClass("android/os/PowerManager");
            IntPtr methodId = JNIEnv.GetMethodID(powerManagerClass, "goToSleep", "(J)V");
            JNIEnv.CallVoidMethod(powerManagerClass, methodId, new JValue(0));
#endif
            Console.WriteLine("Hello Mono World");
        }
    }
}
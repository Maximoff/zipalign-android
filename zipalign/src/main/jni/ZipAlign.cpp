#include "ZipAlign.h"
#include <zipalign/zipalign.h>
#include <cstdlib>

extern "C"
JNIEXPORT jboolean JNICALL Java_ru_maximoff_zipalign_ZipAligner_align
(JNIEnv *env, jclass cls, jstring str1, jstring str2, jint int1, jboolean boolean1) {
    const char *s1 = env->GetStringUTFChars(str1, nullptr);
    const char *s2 = env->GetStringUTFChars(str2, nullptr);
    const int alignNum = (int)int1;
    const bool pageAlignSharedLibs = (bool)boolean1;
    if (alignZip(s1, s2, alignNum, 1, pageAlignSharedLibs) != 1) return JNI_FALSE;
    else return JNI_TRUE;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_ru_maximoff_zipalign_ZipAligner_isAligned
(JNIEnv *env, jclass cls, jstring str, jint int1, jboolean boolean1) {
    const char *s = env->GetStringUTFChars(str, nullptr);
    const int alignNum = (int)int1;
    const bool pageAlignSharedLibs = (bool)boolean1;
    if(isAlignedZip(s, alignNum, pageAlignSharedLibs) == 1) return JNI_TRUE;
    else return JNI_FALSE;
}

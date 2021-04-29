#include <jni.h>

#ifndef _Included_ru_maximoff_zipalign_ZipAligner
#define _Included_ru_maximoff_zipalign_ZipAligner
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ru_maximoff_zipalign_ZipAligner
 * Method:    align
 * Signature: (Ljava/lang/String;Ljava/lang/String;IZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ru_maximoff_zipalign_ZipAligner_align
(JNIEnv *, jclass, jstring, jstring, jint, jboolean);

/*
 * Class:     ru_maximoff_zipalign_ZipAligner
 * Method:    isAligned
 * Signature: (Ljava/lang/String;IZ)Z
 */
JNIEXPORT jboolean JNICALL Java_ru_maximoff_zipalign_ZipAligner_isAligned
(JNIEnv *, jclass, jstring, jint, jboolean);

#ifdef __cplusplus
}
#endif
#endif

LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  
LOCAL_MODULE := zipalign

LOCAL_SRC_FILES :=  ZipAlign.cpp \
	zipalign/ZipAlign.cpp \
	zipalign/ZipEntry.cpp \
	zipalign/ZipFile.cpp \
	zipalign/android/native/src/utils/SharedBuffer.cpp \
	zipalign/android/native/src/utils/ZipUtils.cpp \
	zipalign/android/native/src/utils/VectorImpl.cpp
	
LOCAL_LDLIBS := -llog -lz

LOCAL_C_INCLUDES += $(LOCAL_PATH)/zipalign \
	$(LOCAL_PATH)/zipalign/android/base/include \
	$(LOCAL_PATH)/zipalign/android/core/include \
	$(LOCAL_PATH)/zipalign/android/native/include \

LOCAL_CFLAGS := -DSILENT -DRARDLL -I$(LOCAL_PATH)/rar -fexceptions -Os -ffunction-sections -fdata-sections -fvisibility=hidden -w -Wl,--gc-sections

include $(BUILD_SHARED_LIBRARY)

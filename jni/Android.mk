LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

SRC=src/src
#MAGIC=/sdcard/usr/share/mime/magic
# put the magic file here, or any other place one chooses
MAGIC=/data/local/tmp/file_all_magic

LOCAL_MODULE := file

# include all .c files
FILE_LIST := $(wildcard $(LOCAL_PATH)/$(SRC)/*.c)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%) wrapper.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/$(SRC)

LOCAL_CFLAGS += -DHAVE_CONFIG_H -DTRACE -DMAGIC='"$(MAGIC)"'
LOCAL_LDLIBS := -lz

include $(BUILD_EXECUTABLE)

LOCAL_PATH := $(call my-dir)
TOP_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

# libclient.so
LOCAL_MODULE:= my_client
LOCAL_MODULE_FILENAME:=libmy_client
LOCAL_SRC_FILES:= \
    $(LOCAL_PATH)/../src/client.cpp \
    $(LOCAL_PATH)/../src/common.cpp \
    $(LOCAL_PATH)/../src/common.h

LOCAL_CFLAGS := -DHAVE_CONFIG_H -fvisibility=hidden

LOCAL_CPPFLAGS := \
    -std=c++11 \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden

# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)


# libserver.so
LOCAL_MODULE:= my_server
LOCAL_MODULE_FILENAME:=libmy_server
LOCAL_SRC_FILES:= \
    # $(LOCAL_PATH)/../src/hello.cpp \
    $(LOCAL_PATH)/../src/main.cpp \
    $(LOCAL_PATH)/../src/main.h \
    $(LOCAL_PATH)/../src/common.cpp \
    $(LOCAL_PATH)/../src/common.h \
    $(LOCAL_PATH)/../src/block_server.cpp \
    $(LOCAL_PATH)/../src/block_server.h \
    $(LOCAL_PATH)/../src/nonblock_server.cpp \
    $(LOCAL_PATH)/../src/nonblock_server.h \
    $(LOCAL_PATH)/../src/select_server.cpp \
    $(LOCAL_PATH)/../src/select_server.h \
    $(LOCAL_PATH)/../src/poll_server.cpp \
    $(LOCAL_PATH)/../src/poll_server.h \
    $(LOCAL_PATH)/../src/epoll_server.cpp \
    $(LOCAL_PATH)/../src/epoll_server.h \
    $(LOCAL_PATH)/../src/aio_server.cpp \
    $(LOCAL_PATH)/../src/aio_server.h


LOCAL_CFLAGS := -DHAVE_CONFIG_H -fvisibility=hidden

LOCAL_CPPFLAGS := \
    -std=c++11 \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden

# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)
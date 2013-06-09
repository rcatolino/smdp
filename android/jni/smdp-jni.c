#include <android/log.h>
#include <jni.h>
#include <stdio.h>

#include "../../src/smdp.h"

#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, "SMDP", __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "SMDP", __VA_ARGS__)

#define MAKE_C_STR(cstr, jstr) const char *cstr= (*env)->GetStringUTFChars(env, jstr, NULL);\
                               cstr = cstr ? cstr : ""
#define TRY(retval, errmsg) if (retval == -1) {\
                              LOG_ERROR(errmsg);\
                              return JNI_FALSE;\
                            }
#define N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))
static jobject app;
static struct service_t service;
static int socket;

static jboolean smdp_create_service(JNIEnv *env, jobject this,
                         jstring jid, jstring jprotocol, jstring jaddress, jstring jport) {
  int ret = JNI_TRUE;
  MAKE_C_STR(id, jid);
  MAKE_C_STR(protocol, jprotocol);
  MAKE_C_STR(address, jaddress);
  MAKE_C_STR(port, jport);
  if (create_service(&service, id, protocol, address, port) == -1) {
    LOG_ERROR("Error on create_service\n");
    ret = JNI_FALSE;
  }

  (*env)->ReleaseStringUTFChars(env, jid, id);
  (*env)->ReleaseStringUTFChars(env, jprotocol, protocol);
  (*env)->ReleaseStringUTFChars(env, jaddress, address);
  (*env)->ReleaseStringUTFChars(env, jport, port);
  return ret;
}

static jboolean smdp_start_broadcast_server(JNIEnv* env, jobject this) {
  socket = start_broadcast_server();
  TRY(socket,"Failed to create server\n");
  return JNI_TRUE;
}

static jboolean smdp_send_query(JNIEnv* env, jobject this) {
  int ret = send_query(socket, &service);
  TRY(ret, "Failed to send query\n");
  return JNI_TRUE;
}

static jboolean smdp_wait_for_answer(JNIEnv* env, jobject this) {
  int ret = wait_for_answer(socket, &service);
  TRY(ret, "Failed to wait for answer\n");
  LOG_DEBUG("answer recieved : %s %s://%s:%s\n",
            service.id, service.protocol, service.address, service.port);
  return JNI_TRUE;
}

static jboolean smdp_wait_for_query(JNIEnv* env, jobject this) {
  int ret = wait_for_query(socket, &service);
  TRY(ret, "Failed to wait\n");
  return JNI_TRUE;
}

static jboolean smdp_send_service_broadcast(JNIEnv* env, jobject this) {
  int ret = send_service_broadcast(socket, &service);
  TRY(ret, "Failed send broadcast message\n");
  return JNI_TRUE;
}

static jstring smdp_get_service_field(JNIEnv* env, jobject this, int field) {
  char *value;
  switch (field) {
    case 0 :
      value = service.id;
      break;
    case 1 :
      value = service.protocol;
      break;
    case 2 :
      value = service.address;
      break;
    case 3 :
      value = service.port;
      break;
    default :
      LOG_ERROR("Invalid field\n");
      return NULL;
      break;
  }

  return (*env)->NewStringUTF(env, value);
}

static jboolean native_class_finalize(JNIEnv* env, jobject this) {
  stop_broadcast_server(socket);
  delete_service(&service);
}

static JNINativeMethod native_methods[] = {
  { "nativeFinalize", "()Z", (void *) native_class_finalize},
  { "getServiceField", "(I)Ljava/lang/String;", (void *) smdp_get_service_field},
  { "sendServiceBroadcast", "()Z", (void *) smdp_send_service_broadcast},
  { "sendQuery", "()Z", (void *) smdp_send_query},
  { "waitForAnswer", "()Z", (void *) smdp_wait_for_answer},
  { "waitForQuery", "()Z", (void *) smdp_wait_for_query},
  { "startBroadcastServer", "()Z", (void *) smdp_start_broadcast_server},
  { "createService",
    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z",
    (void *) smdp_create_service},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = NULL;

  if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    LOG_ERROR("Could not retrieve JNIEnv");
    return 0;
  }

  jclass class = (*env)->FindClass(env, "com/rcatolino/smdp/Smdp");
  (*env)->RegisterNatives(env, class, native_methods, N_ELEMENTS(native_methods));

  return JNI_VERSION_1_4;
}

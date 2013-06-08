#include <stdio.h>

#include "smdp.h"

int main(int argc, char * argv[]) {
  struct service_t service;
  int socket;

  if (create_service(&service, "smdp-test", "tcp", "192.168.1.4", "5252") == -1) {
    printf("Error on create_service\n");
    return -1;
  }

  socket = start_broadcast_server();
  if (socket == -1) {
    printf("fail\n");
    return -1;
  }

  send_service_broadcast(socket, &service);
  printf("query 1 :\n");
  wait_for_query(socket, &service);
  printf("\nquery 2 :\n");
  wait_for_query(socket, &service);

  return 0;
}

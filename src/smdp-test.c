#include <stdio.h>

#include "smdp.h"

int main(int argc, char * argv[]) {
  struct service_t service;
  char * serviceid;
  int socket;

  if (argc > 1) {
    serviceid = argv[1];
  } else {
    serviceid = "smdp-test";
  }

  if (create_service(&service, serviceid, "tcp", "192.168.1.4", "5252") == -1) {
    printf("Error on create_service\n");
    return -1;
  }

  socket = start_broadcast_server();
  if (socket == -1) {
    printf("fail\n");
    return -1;
  }

  printf("Waiting for query on %s\n", service.id);
  wait_for_query(socket, &service);
  printf("query received\n");
  send_service_broadcast(socket, &service);
  printf("service broadcast sent\n");

  return 0;
}

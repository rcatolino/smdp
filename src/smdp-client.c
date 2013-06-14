#include <stdio.h>

#include "smdp.h"

int main(int argc, char * argv[]) {
  struct service_t service;
  int socket;

  if (create_service(&service, "remote-server", "", "", "") == -1) {
    printf("Error on create_service\n");
    return -1;
  }

  socket = start_broadcast_server();
  if (socket == -1) {
    printf("fail\n");
    return -1;
  }

  send_query(socket, &service);
  printf("query sent\n");
  wait_for_answer(socket, &service);
  printf("answer recieved : %s %s://%s:%s\n",
         service.id, service.protocol, service.address, service.port);

  return 0;
}

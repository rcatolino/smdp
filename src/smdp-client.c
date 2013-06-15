#include <stdio.h>

#include "smdp.h"

int main(int argc, char * argv[]) {
  struct service_t service;
  int socket;

  if (create_service(&service, "smdp-test", NULL, NULL, NULL)== -1) {
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
  if (wait_for_answer(socket, &service, 2000) == 0) {
    printf("no answer\n");
    return 0;
  }

  printf("answer recieved : %s %s://%s:%s\n",
         service.id, service.protocol, service.address, service.port);

  return 0;
}

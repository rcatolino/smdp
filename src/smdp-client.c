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

  if (create_service(&service, serviceid, NULL, NULL, NULL)== -1) {
    printf("Error on create_service\n");
    return -1;
  }

  socket = start_broadcast_server();
  if (socket == -1) {
    printf("fail\n");
    return -1;
  }

  printf("Sending query to %s\n", service.id);
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

//copyright raphael catolino, 2013
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "smdp.h"

#define QUERY "__query"
#define SMDP_MSG_MAX_SIZE 64
#define SMDP_PORT 2121
#define SMDP_GROUP "225.1.0.0"

#define _assign_field(field, size) service->field = malloc(strlen(field)+1);\
                                   memcpy(service->field, field, strlen(field)+1);\
                                   size += strlen(field)+1;\

int create_service(struct service_t * service,
                   const char * id,
                   const char * protocol,
                   const char * address,
                   const char * port) {

  int total_size = 0;

  if (!service) {
    return -1;
  }

  _assign_field(id, total_size);
  _assign_field(protocol, total_size);
  _assign_field(address, total_size);
  _assign_field(port, total_size);

  if (total_size > SMDP_MSG_MAX_SIZE) {
    delete_service(service);
    memset(service, 0, sizeof(struct service_t));
    fprintf(stderr, "Service too big, reduce id size\n");
    return -1;
  }

  return 0;
}

int delete_service(struct service_t * service) {
  if (!service) {
    return -1;
  }

  free(service->id);
  free(service->protocol);
  free(service->address);
  free(service->port);
  memset(service, 0, sizeof(struct service_t));
  return 0;
}

void stop_broadcast_server(int socket) {
  close(socket);
}

int start_broadcast_server() {
  int socketd;
  struct ip_mreqn multicast_req;
  struct sockaddr_in local_addr;
  int options = 1;

  socketd = socket(AF_INET, SOCK_DGRAM, 0);

  if(socketd == -1) {
  	perror("socket ");
  	return -1;
  }

  if (setsockopt(socketd,
                 SOL_SOCKET,
                 SO_REUSEADDR,
                 &options,sizeof(options)) == -1) {
  	perror("setsockopt S0_REUSEADDR ");
  }

  memset(&local_addr, 0, sizeof(struct sockaddr_in));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(SMDP_PORT);
  local_addr.sin_addr.s_addr = INADDR_ANY;
  /*
  if (inet_pton(AF_INET, SMDP_GROUP, &local_addr.sin_addr) == -1) {
    perror("inet_pton local_addr");
    return -1;
  }
  */ // Is this even possible, or should I use INADDR_ANY here
  	// and another sockaddr_in as a sending socket? to be continued...

  // Code to receive multicast datagrams on SMDP_GROUP:SMDP_PORT :
  if (bind(socketd,
           (struct sockaddr *)&local_addr,
           sizeof(local_addr)) == -1) {
    perror("bind ");
    return -1;
  }

  if (inet_pton(AF_INET, SMDP_GROUP, &multicast_req.imr_multiaddr) == -1) {
    perror("inet_pton multicast_req");
    return -1;
  }

  multicast_req.imr_address.s_addr = INADDR_ANY;
  multicast_req.imr_ifindex = 0;
  if (setsockopt(socketd,
                 IPPROTO_IP,
                 IP_ADD_MEMBERSHIP,
                 &multicast_req, sizeof(multicast_req)) == -1) {
  	perror("setsockopt IP_ADD_MEMBERSHIP");
  }

  /* Code sending and receiving on socketd */

  return socketd;
}

static int parse_msg(char * msg, int nb_token, char *msg_parsed[]) {
  int i;
  printf("message received : %s\n", msg); //XXX: DEBUG ONLY, REMOVE!!!
  for (i=0; i < nb_token; ++i) {
    msg_parsed[i] = strtok(msg, " \n\t\r");
    msg = NULL;
    if (msg_parsed[i] == NULL) {
      // Not all token fields could be extracted.
      return -1;
    }
  }

  return 0;
}

int wait_for_query(int socket, struct service_t * service) {
  ssize_t ret;
  char buff[SMDP_MSG_MAX_SIZE+1];
  char *query[2];

  if (!service) {
    fprintf(stderr, "wait_for_query: Error, service uninitialized\n");
    return -1;
  }

  while (1) {
    memset(buff, 0, SMDP_MSG_MAX_SIZE);
    ret = recvfrom(socket, buff, SMDP_MSG_MAX_SIZE, 0, NULL, 0);
    buff[SMDP_MSG_MAX_SIZE] = '\0';
    if (ret == -1) {
      perror("recvfrom ");
      return -1;
    } else if (parse_msg(buff, 2, query) == -1) {
      fprintf(stderr, "wait_for_query: parse_msg error\n");
      continue;
    } else if (strcmp(QUERY, query[0]) != 0
               || strcmp(service->id, query[1]) != 0) {
      // Not a query, or not for us, keep on looking;
      fprintf(stderr, "Head: %s ID: %s\n", query[0], query[1]);
      continue;
    }

    return 0;
  }

  return 0;
}

static int make_service_msg(char *buff, int buff_size, struct service_t * service) {
  if (!service) {
    fprintf(stderr, "make_service_msg: Error, service uninitialized\n");
    return -1;
  }

  memset(buff, 0, buff_size);
  snprintf(buff, buff_size, "%s %s %s %s",
           service->id, service->protocol, service->address, service->port);
  return 0;
}

int send_service_broadcast(int socket, struct service_t * service) {
  ssize_t ret;
  char buff[SMDP_MSG_MAX_SIZE];
  struct sockaddr_in dest_addr;

  make_service_msg(buff, SMDP_MSG_MAX_SIZE, service);

  memset(&dest_addr, 0, sizeof(struct sockaddr_in));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(SMDP_PORT);
  //dest_addr.sin_addr.s_addr = INADDR_ANY;
  if (inet_pton(AF_INET, SMDP_GROUP, &dest_addr.sin_addr) == -1) {
    perror("inet_pton dest_addr ");
    return -1;
  }

  ret = sendto(socket, buff, strlen(buff), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  if (ret == -1) {
    perror("sendto ");
    return -1;
  }

  return 0;
}

#ifndef SMDP_H_
#define SMDP_H_

struct service_t {
  char * id;
  char * protocol;
  char * address;
  char * port;
};

int create_service(struct service_t * service,
                   const char * id,
                   const char * protocol,
                   const char * address,
                   const char * port);
int delete_service(struct service_t * service);
void stop_broadcast_server(int socket);
int start_broadcast_server();
int wait_for_query(int socket, struct service_t * service);
int send_service_broadcast(int socket, struct service_t * service);

#endif

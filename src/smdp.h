#ifndef SMDP_H_
#define SMDP_H_

#ifdef DEBUG
  #define debug(...) printf(__VA_ARGS__)
  #define derror(a) perror(a)
#else
  #define debug(...)
  #define derror(a)
#endif

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
int start_broadcast_server();
void stop_broadcast_server(int socket);
int send_service_broadcast(int socket, const struct service_t * service);
int send_query(int socket, const struct service_t * service);
int wait_for_answer(int socket, struct service_t * service);
int wait_for_query(int socket, const struct service_t * service);

#endif

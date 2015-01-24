#ifndef SMDP_H_
#define SMDP_H_

#include <netinet/ip.h>

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
/* Initialize a service data structure. This structure will be used by all
 * following calls to the api.
 *
 * When you're done, don't forget to release the allocated resources by
 * calling `delete_service`.
 *
 * If you intend to broadcast a capability you should fill all the structure
 * (or your clients won't get much infos...). If you intend to query another
 * server's capabilities, you should just fill in the `id` which must correspond
 * to the server's one. (You can still fill in the rest, but they won't be of
 * any use)
 *
 * The overall size of the data in this structure can't go over 64 bytes. Cause
 * I was really to lazy to deal with variable-size msg over the network.
 * If too much data is passed, this function will fail and return -1. If you
 * didn't allocate a service_t to back the `service` arg, this function will
 * fail and return -1.*/

int delete_service(struct service_t * service);
/* Cf `create_service` */

int start_broadcast_server(struct in_addr local_ip_address, int ifindex);
/* Call this function before calling any network related function of the API
 * (ie send_service/query and wait_for_service/query). Otherwise they will
 * fail lamentably and return -1
 * The local_ip_address parameter is used as the multicast messages source and
 * can be set to INADDR_ANY to let the system choose a default.
 * The ifindex parameter is the interface index of the interface used
 * to join the multicast group, or 0 to use the default interface.
 */

void stop_broadcast_server(int socket);
/* Call this function when you're done with the networking part.
 * (ie when you're done with the API)
 */

int send_service_broadcast(int socket, const struct service_t * service);
/* Server side! */
/* Call this function when you've received a query for your service
 * with `wait_for_query`
 */

int send_query(int socket, const struct service_t * service);
/* Client side! */
/* Call this function to ask for a service's address. If a service has
 * called `wait_for_query` with a matching service's id, it will
 * receive your query
 */

int wait_for_answer(int socket, struct service_t * service, int timeout);
/* Client side! */
/* Call this function to wait for an answer to your query from a service.
 * Since you call this after a send_query you can safely assume that no
 * such service is available after a reasonable timeout (in ms). The
 * timeout value has the same signification as the poll one
 * (negative value = infinity, 0 = return immediately ).
 * This function returns 1 when an answer is received, 0 when it times out
 * and -1 on error.
 */

int wait_for_query(int socket, const struct service_t * service);
/* Server side! */
/* Call this function to wait for a client to query your capabilities.
 * You should then answer with `send_service_broadcast`
 */

#endif

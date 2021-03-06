
/** \addtogroup netsocket */
/** @{*/
/* nsapi.h - The network socket API
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NSAPI_TYPES_H
#define NSAPI_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** Enum of standardized error codes
 *
 *  Valid error codes have negative values and may
 *  be returned by any network operation.
 *
 *  @enum nsapi_error_t
 */
typedef enum nsapi_error {
    NSAPI_ERROR_OK            =  0,        /*!< no error */
    NSAPI_ERROR_WOULD_BLOCK   = -3001,     /*!< no data is not available but call is non-blocking */
    NSAPI_ERROR_UNSUPPORTED   = -3002,     /*!< unsupported functionality */
    NSAPI_ERROR_PARAMETER     = -3003,     /*!< invalid configuration */
    NSAPI_ERROR_NO_CONNECTION = -3004,     /*!< not connected to a network */
    NSAPI_ERROR_NO_SOCKET     = -3005,     /*!< socket not available for use */
    NSAPI_ERROR_NO_ADDRESS    = -3006,     /*!< IP address is not known */
    NSAPI_ERROR_NO_MEMORY     = -3007,     /*!< memory resource not available */
    NSAPI_ERROR_NO_SSID       = -3008,     /*!< ssid not found */
    NSAPI_ERROR_DNS_FAILURE   = -3009,     /*!< DNS failed to complete successfully */
    NSAPI_ERROR_DHCP_FAILURE  = -3010,     /*!< DHCP failed to complete successfully */
    NSAPI_ERROR_AUTH_FAILURE  = -3011,     /*!< connection to access point failed */
    NSAPI_ERROR_DEVICE_ERROR  = -3012,     /*!< failure interfacing with the network processor */
} nsapi_error_t;

/** Enum of encryption types
 *
 *  The security type specifies a particular security to use when
 *  connected to a WiFi network
 */
typedef enum nsapi_security {
    NSAPI_SECURITY_NONE         = 0x0,      /*!< open access point */
    NSAPI_SECURITY_WEP          = 0x1,      /*!< phrase conforms to WEP */
    NSAPI_SECURITY_WPA          = 0x2,      /*!< phrase conforms to WPA */
    NSAPI_SECURITY_WPA2         = 0x3,      /*!< phrase conforms to WPA2 */
    NSAPI_SECURITY_WPA_WPA2     = 0x4,      /*!< phrase conforms to WPA/WPA2 */
    NSAPI_SECURITY_UNKNOWN      = 0xFF,     /*!< unknown/unsupported security in scan results */
} nsapi_security_t;

/** Maximum size of IP address representation
 */
#define NSAPI_IP_SIZE NSAPI_IPv6_SIZE

/** Maximum number of bytes for IP address
 */
#define NSAPI_IP_BYTES NSAPI_IPv6_BYTES

/** Maximum size of MAC address representation
 */
#define NSAPI_MAC_SIZE 18

/** Maximum number of bytes for MAC address
 */
#define NSAPI_MAC_BYTES 6

/** Size of IPv4 representation
 */
#define NSAPI_IPv4_SIZE 16

/** Number of bytes in IPv4 address
 */
#define NSAPI_IPv4_BYTES 4

/** Size of IPv6 representation
 */
#define NSAPI_IPv6_SIZE 40

/** Number of bytes in IPv6 address
 */
#define NSAPI_IPv6_BYTES 16

/** Enum of IP address versions
 *
 *  The IP version specifies the type of an IP address.
 *
 *  @enum nsapi_version_t
 */
typedef enum nsapi_version {
    NSAPI_IPv4,   /*!< Address is IPv4 */
    NSAPI_IPv6,   /*!< Address is IPv6 */
    NSAPI_UNSPEC  /*!< Address is unspecified */
} nsapi_version_t;

/** IP address structure for passing IP addresses by value
 */
typedef struct nsapi_addr {
    /** IP version
     *  NSAPI_IPv4 or NSAPI_IPv6 (NSAPI_UNSPEC not currently supported)
     */
    nsapi_version_t version;

    /** IP address
     *  The raw bytes of the IP address stored in big-endian format
     */
    uint8_t bytes[NSAPI_IP_BYTES];
} nsapi_addr_t;


/** Opaque handle for network sockets
 */
typedef void *nsapi_socket_t;


/** Enum of socket protocols
 *
 *  The socket protocol specifies a particular protocol to
 *  be used with a newly created socket. 
 *
 *  @enum nsapi_protocol_t
 */
typedef enum nsapi_protocol {
   NSAPI_TCP, /*!< Socket is of TCP type */
   NSAPI_UDP, /*!< Socket is of UDP type */
} nsapi_protocol_t;

/*  Enum of standardized stack option levels
 *
 *  @enum nsapi_level_t
 */
typedef enum nsapi_level {
    NSAPI_STACK,  /*!< Stack option level */
    NSAPI_SOCKET, /*!< Socket option level */
} nsapi_level_t;

/*  Enum of standardized stack options
 *
 *  These options may not be supported on all stacks, in which
 *  case NSAPI_ERROR_UNSUPPORTED may be returned from setsockopt.
 *
 *  @enum nsapi_option_t
 */
typedef enum nsapi_option {
    NSAPI_REUSEADDR, /*!< Allow bind to reuse local addresses */
    NSAPI_KEEPALIVE, /*!< Enables sending of keepalive messages */
    NSAPI_KEEPIDLE,  /*!< Sets timeout value to initiate keepalive */
    NSAPI_KEEPINTVL, /*!< Sets timeout value for keepalive */
    NSAPI_LINGER,    /*!< Keeps close from returning until queues empty */
    NSAPI_SNDBUF,    /*!< Sets send buffer size */
    NSAPI_RCVBUF,    /*!< Sets recv buffer size */
    NSAPI_BROADCAST,
} nsapi_option_t;

/** nsapi_wifi_ap structure
 *
 *  Structure representing a WiFi Access Point
 */
typedef struct nsapi_wifi_ap {
    char ssid[33]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
    uint8_t bssid[6];
    nsapi_security_t security;
    int8_t rssi;
    uint8_t channel;
} nsapi_wifi_ap_t;


/** nsapi_stack structure
 *
 *  Stack structure representing a specific instance of a stack.
 */
typedef struct nsapi_stack {
    /** Network stack operation table
     *
     *  Provides access to the underlying api of the stack. This is not
     *  flattened into the nsapi_stack to allow allocation in read-only
     *  memory.
     */
    const struct nsapi_stack_api *stack_api;

    /** Opaque handle for network stacks
     */
    void *stack;

    // Internal nsapi buffer
    unsigned _stack_buffer[16];
} nsapi_stack_t;

/** nsapi_stack_api structure
 *
 *  Common api structure for network stack operations. A network stack
 *  can provide a nsapi_stack_api structure filled out with the
 *  appropriate implementation.
 *
 *  Unsupported operations can be left as null pointers.
 */
typedef struct nsapi_stack_api
{
    /** Get the local IP address
     *
     *  @param stack    Stack handle
     *  @return         Local IP Address or null address if not connected
     */
    nsapi_addr_t (*get_ip_address)(nsapi_stack_t *stack);

    /** Translates a hostname to an IP address
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *  
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack. 
     *
     *  @param stack    Stack handle
     *  @param addr     Destination for the host IP address
     *  @param host     Hostname to resolve
     *  @param version  Address family
     *  @return         0 on success, negative error code on failure
     */
    int (*gethostbyname)(nsapi_stack_t *stack, const char *host, nsapi_addr_t *addr, nsapi_version_t version);

    /** Add a domain name server to list of servers to query
     *
     *  @param addr     Destination for the host address
     *  @return         0 on success, negative error code on failure
     */
    int (*add_dns_server)(nsapi_stack_t *stack, nsapi_addr_t addr);

    /*  Set stack-specific stack options
     *
     *  The setstackopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the stack is unmodified.
     *
     *  @param stack    Stack handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */
    int (*setstackopt)(nsapi_stack_t *stack, int level, int optname, const void *optval, unsigned optlen);

    /*  Get stack-specific stack options
     *
     *  The getstackopt allow an application to retrieve stack-specific hints
     *  from the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and optval is unmodified.
     *
     *  @param stack    Stack handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    int (*getstackopt)(nsapi_stack_t *stack, int level, int optname, void *optval, unsigned *optlen);

    /** Opens a socket
     *
     *  Creates a network socket and stores it in the specified handle.
     *  The handle must be passed to following calls on the socket.
     *
     *  A stack may have a finite number of sockets, in this case
     *  NSAPI_ERROR_NO_SOCKET is returned if no socket is available.
     *
     *  @param stack    Stack context
     *  @param socket   Destination for the handle to a newly created socket
     *  @param proto    Protocol of socket to open, NSAPI_TCP or NSAPI_UDP
     *  @return         0 on success, negative error code on failure
     */
    int (*socket_open)(nsapi_stack_t *stack, nsapi_socket_t *socket, nsapi_protocol_t proto);

    /** Close the socket
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @return         0 on success, negative error code on failure
     */
    int (*socket_close)(nsapi_stack_t *stack, nsapi_socket_t socket);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param addr     Local address to bind, may be null
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    int (*socket_bind)(nsapi_stack_t *stack, nsapi_socket_t socket, nsapi_addr_t addr, uint16_t port);

    /** Listen for connections on a TCP socket
     *
     *  Marks the socket as a passive socket that can be used to accept
     *  incoming connections.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param backlog  Number of pending connections that can be queued
     *                  simultaneously
     *  @return         0 on success, negative error code on failure
     */
    int (*socket_listen)(nsapi_stack_t *stack, nsapi_socket_t socket, int backlog);

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by the
     *  indicated address.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param addr     The address of the remote host
     *  @param port     The port of the remote host
     *  @return         0 on success, negative error code on failure
     */
    int (*socket_connect)(nsapi_stack_t *stack, nsapi_socket_t socket, nsapi_addr_t addr, uint16_t port);

    /** Accepts a connection on a TCP socket
     *
     *  The server socket must be bound and set to listen for connections.
     *  On a new connection, creates a network socket and stores it in the
     *  specified handle. The handle must be passed to following calls on
     *  the socket.
     *
     *  A stack may have a finite number of sockets, in this case
     *  NSAPI_ERROR_NO_SOCKET is returned if no socket is available.
     *
     *  This call is non-blocking. If accept would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param stack    Stack handle
     *  @param server   Socket handle to server to accept from
     *  @param socket   Destination for a handle to the newly created socket
     *  @param addr     Destination for the address of the remote host
     *  @param port     Destination for the port of the remote host
     *  @return         0 on success, negative error code on failure
     */
    int (*socket_accept)(nsapi_stack_t *stack, nsapi_socket_t server, nsapi_socket_t *socket, nsapi_addr_t *addr, uint16_t *port);

    /** Send data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes sent from the buffer.
     *
     *  This call is non-blocking. If send would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    int (*socket_send)(nsapi_stack_t *stack, nsapi_socket_t socket, const void *data, unsigned size);

    /** Receive data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes received into the buffer.
     *
     *  This call is non-blocking. If recv would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    int (*socket_recv)(nsapi_stack_t *stack, nsapi_socket_t socket, void *data, unsigned size);

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address. Returns the number of bytes
     *  sent from the buffer.
     *
     *  This call is non-blocking. If sendto would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param addr     The address of the remote host
     *  @param port     The port of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    int (*socket_sendto)(nsapi_stack_t *stack, nsapi_socket_t socket, nsapi_addr_t addr, uint16_t port, const void *data, unsigned size);

    /** Receive a packet over a UDP socket
     *
     *  Receives data and stores the source address in address if address
     *  is not NULL. Returns the number of bytes received into the buffer.
     *
     *  This call is non-blocking. If recvfrom would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param addr     Destination for the address of the remote host
     *  @param port     Destination for the port of the remote host
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    int (*socket_recvfrom)(nsapi_stack_t *stack, nsapi_socket_t socket, nsapi_addr_t *addr, uint16_t *port, void *buffer, unsigned size);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param callback Function to call on state change
     *  @param data     Argument to pass to callback
     */
    void (*socket_attach)(nsapi_stack_t *stack, nsapi_socket_t socket, void (*callback)(void *), void *data);

    /*  Set stack-specific socket options
     *
     *  The setsockopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the socket is unmodified.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    int (*setsockopt)(nsapi_stack_t *stack, nsapi_socket_t socket, int level, int optname, const void *optval, unsigned optlen);

    /*  Get stack-specific socket options
     *
     *  The getstackopt allow an application to retrieve stack-specific hints
     *  from the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and optval is unmodified.
     *
     *  @param stack    Stack handle
     *  @param socket   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    int (*getsockopt)(nsapi_stack_t *stack, nsapi_socket_t socket, int level, int optname, void *optval, unsigned *optlen);
} nsapi_stack_api_t;


#ifdef __cplusplus
}
#endif

#endif

/** @}*/

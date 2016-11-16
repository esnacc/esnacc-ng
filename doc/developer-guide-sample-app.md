Developer's Guide: Sample Communications Application in C++
===========================================================

## Contents

- [Introduction](#introduction)
- [Protocol](#protocol)
- [Implementation](#implementation)
- [Using](#using)

## Introduction

### In-Depth Application

The following application will show a sample bonjour-like discovery service,
which can be used for network discovery across various multicast domains.  The
purpose of this document will be to show the design of the protocol, as well
as a sample implementation demonstrating the power of eSNACC and ASN.1


### What you will need

It is assumed that the following are true:

- You are on a system with an installed eSNACC distribution, at least 1.8
- You have access to a c++ compiler, usually G++
- You have a multicast network available and multiple test machines (VMs?)


### Where to get help

As always, consult the mailing lists.  Specifically, `dev@lists.esnacc.org` or
`bugs@lists.esnacc.org` to report issues.


## Protocol

### Goals

- Allow for automatic discovery of network elements.
- Allow for registration of services with the discovery agents
- Allow 'hop' nodes to query for discovery status
- Exchange data on the discovery network, both unicast and multi-cast.

### High level overview

The discovery mechanism is made up of two separate but important functions.

The low-level network transmission information and a higher-level series of
state machines which coordinates services.  Discovery nodes will advertise
themselves to peer entities, and broadcast their service definitions.  They 
must do this in a host-machine agnostic way.

The network mode of transmission must support both traditional unicast 
messaging, as well as multicast group messaging.  It may be independent of
Internet Protocol version, but at least must support either ipv4 or ipv6 
addresses.

The nodes must share their information via a series of hello and status 
messages, with a simple pilot 'hello' message being the main bulk of network
traffic exchanged in the multicast group.

### ASN.1 Definition

```
NodeDiscovery DEFINITIONS AUTOMATIC TAGS ::=
BEGIN

   OperatingSystem ::= ENUMERATED
   {
      unknown,
      linux,
      windows,
      freebsd,
      openbsd,
      netbsd,
      ...
   }

   Time ::= SEQUENCE
   {
      sec INTEGER,
      nsec INTEGER
   }

   IPv4Address ::= INTEGER
   IPv6Address ::= OCTET STRING

   AddressType ::= CHOICE
   {
       v4Addr IPv4Address,
       v6Addr IPv6Address
   }

   TcpPort ::= INTEGER (1024..65535)
   UdpPort ::= INTEGER (1024..65535)

   PortChoice ::= CHOICE
   {
       tcp TcpPort,
       udp UdpPort
   }

   Host ::= SEQUENCE
   {
       addressType AddressType,
       systemHostName VisibleString OPTIONAL
   }

   NodeInformation ::= SEQUENCE
   {
       hostInformation Host,
       discoveryUUIDName VisibleString,
       operatingSystem OperatingSystem OPTIONAL,
       currentTimeSinceEpoch Time OPTIONAL,
       uptime Time,
       ...
   }

   Service ::= SEQUENCE
   {
       listenerAddress AddressType,
       portInformation PortChoice OPTIONAL,
       serviceName OCTET STRING
   }

   DiscoveryHello ::= SEQUENCE
   {
       hostDescription Host,
       discoveryUUIDName VisibleString
   }

   DiscoveryStatus ::= SEQUENCE
   {
       hostDescription NodeInformation,
       providedServices SEQUENCE OF Service OPTIONAL,
       requestedCommand OCTET STRING OPTIONAL,
       commandResponse OCTET STRING OPTIONAL,
       ...
   }

   DiscoveryMessage ::= CHOICE
   {
       hello DiscoveryHello,
       status DiscoveryStatus,
       ...
   }

END
```


### Description

The discovery agents will exchange `DiscoveryMessage` objects via the discovery
network.  The discovery network will be a multi-cast group, with group address
and udp port to be defined by the network administrator.  All `Host` 
participants will advertise their node address, which is the preferred address
to reach the that particular node.



## Implementation

The implementation shown here will be a mostly-complete C++ application, ready
to deploy.  Some details will need to be expanded to make the application more
usable.

Let's start with the basics, a database of discovered nodes.  We really only
need to decide on the best ways of storing and indexing into that data.  The
following represents a simple in-memory storage for the database.

```c++
#include <map>
#include <memory>
#include <vector>
#include <time.h>
#include <signal.h>

#include "NodeDiscovery.h"

struct NodeInfo {
    SNACC::NodeInformation node;
    SNACC::Time lastHeard;
    
    NodeInfo(SNACC::NodeInformation &n, SNACC::Time &t) : node(n),
                                                          lastHeard(t) {}
};

class NodeDatabase
{
    typedef std::shared_ptr<NodeInfo> node_t;
    std::vector<node_t> m_oNodes;
    std::map<std::string, node_t> m_oNodesByHostName;
    std::map<std::string, node_t> m_oNodesByDiscoveryUUID;

public:
    class DBAddObserverCallback
    {
    public:
        DBAddObserverCallback() {}
        virtual ~DBAddObserverCallback() {}
        
        virtual void callback(node_t node) = 0;
    };

protected:

    std::vector<DBAddObserverCallback*> m_callbacks;

    template <typename T>
    node_t RetrieveNodeInKeyedDatabase(const T &key, 
                                       const std::map<T, node_t> &db) const {
        if (db.find(key) != db.end()) {
            return db.find(key)->second;
        }
        return node_t(0);
    }

public:

    NodeDatabase() : m_oNodes(), m_oNodesByHostName(), 
                     m_oNodesByDiscoveryUUID(), m_callbacks()
    {
    // nothing
    }

    ~NodeDatabase() { }

    void AddCallback(DBAddObserverCallback *cb)
    {
        m_callbacks.push_back(cb);
    }

    node_t RetrieveNodeByHostName(const std::string &host) const
    {
        return RetrieveNodeInKeyedDatabase(host, m_oNodesByHostName);
    }

    node_t RetrieveNodeByUUID(const std::string &uuid) const
    {
        return RetrieveNodeInKeyedDatabase(uuid, m_oNodesByDiscoveryUUID);
    }
    
    void AddNode(SNACC::NodeInformation &node)
    {
        SNACC::Time currentTime;
        {
            struct timespec ts;
            if (::clock_gettime(CLOCK_MONOTONIC, &ts)) {
                perror("clock_gettime");
                raise(SIGABRT);
            }
            currentTime.sec = ts.tv_sec;
            currentTime.nsec = ts.tv_nsec;
        }
        /* first - is this node already added */
        if (node.hostInformation.systemHostName) {
            if (RetrieveNodeByHostName(*node.hostInformation.systemHostName)) {
                RetrieveNodeByHostName(*node.hostInformation.systemHostName)->
                    lastHeard = currentTime;
                return;
            }
        } 

        if (RetrieveNodeByUUID(node.discoveryUUIDName)) {
            RetrieveNodeByUUID(node.discoveryUUIDName)->lastHeard = currentTime;
            return;
        }

        if (!node.discoveryUUIDName.length()) {
            // no discovery uuid!?  bad!
            return;
        }

        /* does not exist - see if we can add it to the database */
        node_t newNode(new NodeInfo(node, currentTime));
        m_oNodes.push_back(newNode);
        
        if (node.hostInformation.systemHostName) {
            m_oNodesByHostName[*node.hostInformation.systemHostName] =
                newNode;
        }
        
        m_oNodesByDiscoveryUUID[node.discoveryUUIDName] = newNode;
        for (std::vector<DBAddObserverCallback*>::iterator c =
                 m_callbacks.begin();
             c != m_callbacks.end(); ++c) {
            (*c)->callback(newNode);
        }
    }

    void CopyNodes(std::vector<NodeInfo> &nodes)
    {
        // copies all the node information
        for (std::vector<node_t>::iterator i = m_oNodes.begin(); 
             i != m_oNodes.end(); ++i) {
            nodes.push_back(*(*i));
        }
    }
};
```

In the database implementation, we do a few things.  Whenever a node is passed
to the Add routine, if it exists we update it's last-heard time.  If not, we
will add it to the database, and call the various observer callbacks.

With that database, we can store nodes which we learn 'on-the-fly'.  We'll
need a mechanism for receiving discovery data.  Luckily, *esnacc* does provide
a way of doing this:

```c++
class NodeDiscoveryCB : public SNACC::StreambufCallback
{
    NodeDatabase *db;
public:
    NodeDiscoveryCB(NodeDatabase *d) : db(d) {}
    ~NodeDiscoveryCB(){}
    
    SNACC::StreambufCallback::Streambuf_CB_RESULT
    callback(SNACC::StreambufCallback::Streambuf_CB_STATUS status,
             std::streambuf *stream)
    {
        if (status == CB_READ_OK) {
            if (stream) {
                SNACC::AsnBuf b(stream);
                SNACC::AsnLen l;
                SNACC::DiscoveryMessage msg;
                try {
                    msg.BDec(b, l);
                } catch (SNACC::SnaccException &e) {
                    // should we abort here? for now yes
                    raise(SIGABRT);
                }
                switch (msg.choiceId) {
                default:
                    raise(SIGABRT);
                    break; // never reached
                case SNACC::DiscoveryMessage::helloCid:
                {
                    SNACC::NodeInformation node;
                    node.hostInformation = msg.hello->hostDescription;
                    node.discoveryUUIDName = msg.hello->discoveryUUIDName;
                    db->AddNode(node);
                }
                break;
                case SNACC::DiscoveryMessage::statusCid:
                {
                    // update / refresh the database
                    db->AddNode(msg.status->hostDescription);
                    // likely in response to a request so just print everything
                    std::cout << *(msg.status) << std::endl;
                }
                break;
                };
            }
        }
    }
};
```

This callback can be used in conjunction with a streambuf monitor to 
automatically receive and process messages from the discovery network.

The only thing left is to add a simple transport for the network - here we
can use socket calls and setup an IPv4 IGMP socket and add the socket to
an existing streambuf monitor:

```c++
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

NodeDiscoveryCB gCallback;

int get_multicast_socket(unsigned short port)
{
    struct sockaddr_in addr;
    int fd, nbytes;
    socklen_t  addrlen;
    struct ip_mreq mreq;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        raise(SIGABRT);
    }

    // set up addresses
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        raise(SIGABRT);
    }

    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))< 0) {
        perror("setsockopt");
        raise(SIGABRT);
    }

    return fd;
}

void JoinStreambufMonitorToPort(int fd, SNACC::StreambufMonitor *mon,
                                unsigned short p)
{
    if (fd < 0) fd = get_multicast_socket(p);
    SNACC::AsnFDBuf *fdb = new SNACC::AsnFDBuf(fd, true);
    mon->push(fdb, gCallback, false);
}

```

And tie the whole thing together with:

```c++
#include <unistd.h>

int main(int argc, char *argv[])
{
    SNACC::StreambufMonitor mon;
    int fd = get_multicast_socket(44321);
    JoinStreambufMonitorToPort(fd, &mon, 44321);
    while (1) {
        mon();
        sleep(5);

        SNACC::DiscoveryMessage msg;
        msg.choiceId = SNACC::DiscoveryMessage::helloCid;
        msg.hello = new SNACC::DiscoveryHello;
        msg.hello->hostDescription.addressType.choiceId =
            SNACC::AddressType::v4AddrCid;
        msg.hello->hostDescription.addressType.v4Addr =
            new SNACC::IPv4Address(0x01);

        std::stringstream s;
        SNACC::AsnBuf b(s.rdbuf());
        msg.BEnc(b);

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(44321);
        addr.sin_addr.s_addr = inet_addr("224.0.0.1");
        sendto(fd, s.str().c_str(), s.str().length(), 0,
               (struct sockaddr *)&addr, sizeof addr);
    }
    return 0;
}
```

The short explanation is as follows:

1. When the binary starts, it creates (a) a socket monitor, (b) a multi-cast
   socket, and (c) joins them together.
   
2. During the main loop, (b) is monitored for data.

3. If data is detected, the callback is invoked flagging read-readiness.  The
   callback attempts to decode a packet on the wire.  NOTE: no sanity checking
   is done to protect against malicious users.

4. A 5 second sleep occurs, and then a new discovery message is created and
   transmitted via the multi-cast socket to all members of the group.



## Using

Wrapping each of the C++ code sections above in a single file called 
*discovery.cpp* will let us make a simple build for us to test.


### Build

We can compile this with the following simple command:

`g++ -std=c++0x -o test discovery.cpp NodeDiscovery.cpp
     -I. -I/usr/include/cxx-lib/inc -lcxxasn1 -pthread -lrt`

That will generate the *test* binary, which can be executed on our host
machine.  It is important not to run this in production - after all it is meant
as an illustrative example.


### Executing

Using two systems on the same network, you should be able to insert a print
routine that will walk the database and dump it's contents every so often.
This should show both machines communicating.  Other enhancements to add would
be service registration and querying.

Using a simple network sniffing tool, such as *tcpdump*, we can see the traffic
going on the multicast socket:

    tcpdump: listening on any, link-type LINUX_SLL (Linux cooked), capture size 262144 bytes
    10:46:47.868779 IP (tos 0x0, ttl 1, id 26815, offset 0, flags [DF], proto UDP (17), length 45)
    	192.168.0.135.rwhois > 224.0.0.1.rwhois: UDP, length 17
    	0x0000:  4500 002d 68bf 4000 0111 6fd0 c0a8 0087
    	0x0010:  e000 0001 10e1 10e1 0019 5910 0081 0101
    	0x0020:  0203 a005 a007 3009 a00d 300f a0

Notice that after the last octet of the ipv4 header (`e000 0001`) comes the 
ASN.1 encoded BER message.


### Future

As it stands, the discovery mechanism here could be greatly improved.  It could,
for instance, use a unix domain socket with a qemu VM to expose discovery via
a non-IP network, using the same socket monitor.  This could be used to perform
things like VM orchestration, or monitoring.  Additional front-ends could be
devised, such as SOAP interfaces using the XML encoding rules.  This is meant
to be just a simple taste of what can be done.

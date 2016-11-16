Developer's Guide
=================

## Contents

- [Introduction](#introduction)
- [Quick Example](#tutorial)

## Introduction

### What is ASN.1?

ASN.1 is an International Telecommunications Union (ITU) standard, also known as 
the Abstract Syntax Notation version 1.  It is formed of two parts - a set of 
language standards used to describe some structured data, and a set of rules for
representing that data as sequences of bits and bytes.  Think of something like
a powerful IDL.  Unlike other systems which provide serialization and deserialization
libraries, ASN.1 is *standards based*, and has almost **40 years** of development
and design.

The beauty of ASN.1 lies in the amount of usage and testing.  ASN.1 is involved in
every secure web browser request.  It forms the basis of most secure messaging.  It
is used in most telco applications (cell phones, and landlines alike).  There are
hundreds of competing implementations, which means the interoperability is unmatched.

There are other serialization and interoperability libraries and mechanisms.  Flat
XML, and JSON data is one example.  Google's Protocol Buffers, and Apache Thrift are
two popular libraries.  We believe ESNACC's ASN.1 is a competitive alternate to these
ecosystems, as it has been in production use for decades, and is built on the powerful
ASN.1 standards.


### How does ESNACC's implementation of ASN.1 work?

eSNACC is composed of two parts.  The first part is a compiler which will take the
ASN.1 translation units and output code in the backend format.  This code will be
in source form, and should generally not be edited.

The second part is a back-end library.  Each back-end will have a langauge specific
set of calls to provide the various features to integrate and exchange ASN.1 data.


## Tutorial

The following aims to demonstrate a simple request-response network based 
communication system.  However, it merely aims to highlight one aspect of ASN.1
applications programming.

### Setting up data structures

First, the set of data to be exchanged is spelled out in the ASN.1 language and saved
in a `.asn1` file.  Each ASN.1 translation unit contains a `DEFINITIONS` stanza, which
defines all of the data structures to appear.  As an example, the following 
definitions illustrate a simple set of heart-beat and status messages:

```
Heart-beat DEFINITIONS AUTOMATIC TAGS ::=
BEGIN
    RunningProcess ::= SEQUENCE {
        processId INTEGER,
        processName UTF8String,
        processUser UTF8String OPTIONAL,
        processGroup UTF8String OPTIONAL,
        ...
    }

    ProcessList ::= SEQUENCE OF RunningProcess

    -- This is the request --
    HeartbeatRequest ::= SEQUENCE {
        transactionId INTEGER,
        requestProcesses BOOLEAN,
        ...
    }

    HeartbeatErrorMessage ::= SEQUENCE {
        errorId INTEGER,
        errorString UTF8String OPTIONAL
    }

    HeartbeatReplyNoProcesses ::= SEQUENCE {
        ...
    }

    HeartbeatReplyProcesses ::= SEQUENCE {
        processes ProcessList,
        ...
    }
    
    HeartbeatResponses ::= CHOICE {
        HeartbeatErrorMessage,
        HeartbeatReplyNoProcesses,
        HeartbeatReplyProcesses,
        ...
    }

    -- This is the response --
    HeartbeatResponse ::= SEQUENCE {
        transactionId INTEGER,
        response HeartbeatResponses,
        ...
    }
END
```

Notice in the example message definitions, there is a request, and it will always
yield a response, which will have the corresponding `transactionId` and either an
error message, a reply without processes, or a reply with the processes list.

The `...` notation above implies that new fields can be added without breaking
backwards compatibility.  Older code will simply ignore the new field.  The automatic
tagging implies that ESNACC will generate tags.

### Compiling to your back-end language

At this point, the definitions are ready to be compiled down to an implementation
language.  In this case, let's assume you're using C++.  You can then use the ESNACC
compiler with the *-C* flag, and it will generate a `.cpp` and `.h` file for the
heartbeat example code.  That will generate the classes in the following list:

```c++
class RunningProcess;
class HeartbeatErrorMessage;
class HeartbeatReplyNoProcesses;
class ProcessList;
class HeartbeatReplyProcesses;
class HeartbeatResponses;
class HeartbeatRequest;
class HeartbeatResponse;
```

### Interact with the library and generated code

These classes will represent the types defined in the ASN.1 file above.  A simple
request, for instance, can be generated as:

```c++
HeartbeatRequest req;
req.transactionId = globalTransaction++;
if (globalShouldGetProcesses) {
    req.requestProcesses = true;
}

socketStream << SNACC::EncodeBER << req;
```

And a response can be processed, for example, with the below code:

```c++
HeartbeatResponse rep;
socketStream >> SNACC::EncodeBER >> rep;
if (rep.transactionId != globalTransaction - 1) {
    std::cout << "No transaction lists" << std::endl;
}
switch (rep.choiceId) {
default:
case SNACC::HeartbeatResponses::heartbeatErrorMessageCid:
    std::cerr << "Oh No - an error!" << std::endl;
    break;
case SNACC::HeartbeatResponses::heartbeatReplyNoProcessesCid:
    std::cout << "Response" << std::endl;
    break;
case SNACC::HeartbeatResponses::heartbeatReplyProcessesCid:
    std::cout << "Response with processes" << std::endl;
    for (SNACC::ProcessList::iterator it = 
                rep.heartbeatReplyProcesses->processes.begin();
         it != rep.heartbeatReplyProcesses->processes.end();
         ++it) {
        std::cout << "Process #" << it->processId << " << std::endl;
    }
    break;
}
```

The whole application will get compiled and linked with `-lcxxasn1` and will then
be ready to execute.

A more complete C++ tutorial can be found [here](developer-guide-sample-app.md).

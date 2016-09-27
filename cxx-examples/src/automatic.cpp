#include "autotags.h"
#include <sstream>

static SNACC::Human *
getHuman(const char *name, int age, bool isBiblical,
         const char *firstWords = NULL)
{
    SNACC::Human *h = new SNACC::Human();
    h->name = name;
    if (firstWords)
        h->first_words = new SNACC::UTF8String(firstWords);
    h->age = new SNACC::HumanChoice();
    if (isBiblical) {
        h->age->biblical = new SNACC::HumanChoice::Biblical(age);
        h->age->choiceId = SNACC::HumanChoice::biblicalCid;
    } else {
        h->age->modern = new SNACC::HumanChoice::Modern(age);
        h->age->choiceId = SNACC::HumanChoice::modernCid;
    }
    return h;
}

int automaticTests()
{
    struct Tests {
        const unsigned char bytes[256];
        size_t byte_len;
        const char *enc_bytes;
        SNACC::Human *h;
    } t[] = {
        {
            {0x30,0x0B,0x80,0x04,0x41,0x64,0x61,0x6D,0xA2,0x03,0x81,0x01,0x5A,},
            13,
            NULL,
            getHuman("Adam", 90, false)
        },
        {
            {0x30,0x0C,0x80,0x04,0x41,0x64,0x61,0x6D,0xA2,0x04,0x80,0x02,0x03,
             0x84,},
            14,
            NULL,
            getHuman("Adam", 900, true)
        },
        {
            {0x30,0x1A,0x80,0x04,0x41,0x64,0x61,0x6D,0x81,0x0D,0x67,0x6F,0x6F,
             0x20,0x67,0x6F,0x6F,0x20,0x67,0x61,0x20,0x67,0x61,0xA2,0x03,0x81,
             0x01,0x5A,},
            28,
            NULL,
            getHuman("Adam", 90, false, "goo goo ga ga")
        },
        {
            {0x30,0x1B,0x80,0x04,0x41,0x64,0x61,0x6D,0x81,0x0D,0x67,0x6F,0x6F,
             0x20,0x67,0x6F,0x6F,0x20,0x67,0x61,0x20,0x67,0x61,0xA2,0x04,0x80,
             0x02,0x03,0x84,},
            29,
            NULL,
            getHuman("Adam", 900, true, "goo goo ga ga")
        },
    };

    size_t i;

    for(i = 0; i < (sizeof(t) / sizeof(t[0])); i++) {
        std::cout << "T[" << i << "]: Starting HUMAN test" << std::endl;
        bool fail_dec = false, fail_enc = false;
        bool fail_name = false;
        bool fail_age = false;
        bool fail_firstwords = false;
        std::stringstream s;
        SNACC::Human h;
        try {
            s.str(std::string((const char *)t[i].bytes, t[i].byte_len));
            s >> SNACC::EncodeBER >> h;
            if (h.name != t[i].h->name) {
                fail_name = true;
            }

            if (h.age->choiceId != t[i].h->age->choiceId) {
                fail_age = true;
            } else {
                if (h.age->choiceId == SNACC::HumanChoice::biblicalCid) {
                    if (*(h.age->biblical) != *(t[i].h->age->biblical)) {
                        fail_age = true;
                    }
                } else {
                    if (*(h.age->modern) != *(t[i].h->age->modern)) {
                        fail_age = true;
                    }
                }
            }

        } catch (SNACC::SnaccException &e) {
            fail_dec = true;
        }

        if (fail_dec || fail_name || fail_age || fail_firstwords) {
            std::cout << "D:F(" << i << "): " << fail_dec << ","
                      << fail_name << ","
                      << fail_age << ","
                      << fail_firstwords
                      << std::endl;
            return 1;
        }

        SNACC::AsnBuf expected((const char *)(t[i].bytes), t[i].byte_len);
        try {
            std::stringstream s;
            s << SNACC::EncodeBER;
            s << h;
            SNACC::AsnBuf benc(s.str().c_str(), s.str().length());

            if (!(benc == expected)) {
                fail_enc = true;
            }
        } catch (SNACC::SnaccException &e) {
            fail_enc = true;
        }
        if (fail_enc || fail_name || fail_age || fail_firstwords) {
            std::cout << "E:F(" << i << "): " << fail_enc << ","
                      << fail_name << ","
                      << fail_age << ","
                      << fail_firstwords
                      << std::endl;
            return 1;
        }
    }
    return 0;
}

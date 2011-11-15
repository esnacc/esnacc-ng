#include "asn-buf.h"
#include "asnutil.h"
#include "vda_threads.h"

void newBufTest()
{
   std::cout << "*** Testing AsnBuf ****\n";
   try
	{
		AsnBuf testBufExcept;
		testBufExcept.GetByte();
	}
	catch (SNACC::SnaccException &e)
	{
   	std::cout << "Caught buffer exception as expected: " << e.what() << "\n";
	}

   std::fstream fs("c:\\temp\\test.txt");

   unsigned char test[10000];
   int j;
   try
   {
      fillTestBuffer(&test[0], 10000);

      // file test
      std::filebuf fb;
      fb.open("test.txt", std::ios_base::out | std::ios_base::binary);
      fb.sputn((char *)&test[0], 10000);
      fb.close();

      AsnBuf fileBuf("test.txt");
      if (fileBuf.length() != 10000)
      {
         std::cout << "ERROR: AsnBuf() file length check failed!\n";
      }
      else
      {
         char fileData[10000];
         try
         {
            fileBuf.GetSeg(&fileData[0], 10000);
            if (memcmp(fileData, test, 10000) != 0)
            {
               std::cout << "ERROR: AsnBuf() file data check failed!\n";
            }

         }
         catch(...)
         {
            std::cout << "ERROR: AsnBuf() file GetSeg() failed!\n";
         }
        
      }
   
      AsnBuf buf2[1000];
      
      for (j = 0; j < 1; j++)
      {

         AsnBuf tmpBuf((char *)test,4200);
#ifdef _DEBUG
         tmpBuf.status(std::cout);
#endif
         tmpBuf.ResetMode();
         char ch = tmpBuf.GetByte();
         char ch2 = tmpBuf.GetByte();
         std::cout << "ch == " << (int)ch << " ch2 == " << (int)ch2 << "\n";

         
         buf2[j].PutSegRvs((char *)test, 10000);

#ifdef _DEBUG
         buf2[j].status(std::cout);
#endif
         AsnBuf buf;
         buf = buf2[j];
         buf.ResetMode();
         if (buf.length() != 10000)
            std::cout << "length() after PutSegRvs() test FAILED!\n";

         std::stringstream ss, ss2;
   
         ss << buf;
         buf.ResetMode();
         ss2 << buf;

#ifdef _DEBUG
         buf.status(std::cout);
#endif
         buf.ResetMode();
         const char *ps = ss.str().data(); // buf.GetSeg(10000)
         
         checkTestBuffer((unsigned char *) ps, 10000);

         if ((ss.str().length() == 10000) &&
            (memcmp(test, ps, 10000) == 0))
            std::cout << "ss is good!" << std::endl;
         else
            std::cout << "[" << j << "] ss is not good!" << std::endl;

         const char *ps2 = ss2.str().data();
         if (ss2.str().length() == 10000 &&
             memcmp(test, ps2, 10000) == 0)

            std::cout << "ss2 is good!" << std::endl;
         else
            std::cout << "[" << j << "] ss2 is not good" << std::endl;
      }
   }
   catch (std::exception &e)
   {
      std::cout << "Caught Standard Exception: " << e.what() << std::endl;
   }
   catch (...)
   {
      std::cout << "Caught un-handled exception!" << std::endl;
   }

	std::cout << "*** end of AsnBuf test ***\n";
}

// file: .../c++-lib/inc/tcl-if.h
//
// $Header: /baseline/SNACC/c++-lib/inc/tcl-if.h,v 1.1.1.1 2000/08/21 20:36:12 leonberp Exp $
// $Log: tcl-if.h,v $
// Revision 1.1.1.1  2000/08/21 20:36:12  leonberp
// First CVS Version of SNACC.
//
// Revision 1.5  1997/01/01 23:27:22  rj
// `typename' appears to be a reserved word in gcc 2.7, so prefix it with `_'
//
// Revision 1.4  1995/09/07  18:50:34  rj
// duplicate code merged into a new function SnaccTcl::gettypedesc().
//
// Revision 1.3  1995/08/17  15:06:43  rj
// snacced.[hC] renamed to tcl-if.[hC].
// class SNACCDLL_API SnaccEd renamed to SnaccTcl.
//
// Revision 1.2  1995/07/27  09:53:25  rj
// comment leader fixed
//
// Revision 1.1  1995/07/27  09:52:12  rj
// new file: tcl interface used by snacced.

#ifdef DEBUG
#include <assert.h>
#endif

class SNACCDLL_API SnaccTcl
{
  Tcl_Interp		*interp;
  Tcl_HashTable		modules,
			types,
			files;

  Tcl_HashEntry		*create();
  const AsnTypeDesc	*gettypedesc (const char *cmdname, const char *type_name);

public:
			SnaccTcl (Tcl_Interp *);
			~SnaccTcl();

  int			create (int argc, char **argv);
  int			openfile (int argc, char **argv);
  int			finfo (int argc, char **argv);
  int			read (int argc, char **argv);
  int			write (int argc, char **argv);
  int			closefile (int argc, char **argv);

  int			modulesinfo (int argc, char **argv);
  int			typesinfo (int argc, char **argv);
  int			typeinfo (int argc, char **argv);
  int			info (int argc, char **argv);

  int			getval (int argc, char **argv);
  int			setval (int argc, char **argv);
  int			unsetval (int argc, char **argv);

  int			test (int argc, char **argv);

#ifdef DEBUG
  void			ckip (Tcl_Interp *i)	{ assert (i == interp); }
#endif
};

class SNACCDLL_API ASN1File
{
  const AsnTypeDesc	*type;
  AsnType		*pdu;

  char			*fn;
  int			fd;
  off_t			filesize;

public:
			ASN1File (const AsnTypeDesc *);
			ASN1File (const AsnTypeDesc *, const char *fn, int fd);
  virtual		~ASN1File();

  bool			bad();

			operator AsnType * ()	{ return pdu; }

  int			finfo (Tcl_Interp *);

  int			read (Tcl_Interp *, const char *fn=NULL);
  int			write (Tcl_Interp *, const char *fn=NULL);
};

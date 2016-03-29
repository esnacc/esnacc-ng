// file: .../c++-lib/inc/asn-list.h - List template classes
//
//

#ifdef _MSC_VER
	#pragma warning(disable: 4189)		// Disable local var initialized warning
	#pragma warning(disable: 4786)		// Disable symbols truncated warning
#endif
#include <list>


//////////////////////////
// Base List template  //
////////////////////////

template <class T> class List 
{
protected:
  unsigned long count;

  struct ListElmt 
  {
    T  *elmt;
    struct ListElmt  *next, *prev;
  } *first, *last;

  mutable ListElmt *curr;

public:
  List()						{ count = 0; first = curr = last = NULL; }
  virtual ~List()				{ RemoveAll(); }
  List(const List<T> &that);

  List<T>& operator=(const List<T>& that);

  void  SetCurrElmt (unsigned long index) const;
  unsigned long GetCurrElmtIndex(void) const;
  void  SetCurrToFirst(void) const    { curr = first; }
  void  SetCurrToLast(void) const     { curr = last; }
  

  // reading member fcns
  T  *Curr()  { return curr ? curr->elmt : NULL; }
  T  *First() { return count > 0 ? first->elmt : NULL; }
  T  *Last()  { return count > 0 ? last->elmt : NULL; }
  T  *Next()  { return curr && curr->next ? curr->next->elmt : NULL; }
  T  *Prev()  { return curr && curr->prev ? curr->prev->elmt : NULL; }
  int Count() const  { return count; }
  const T  *Curr()  const  { return curr ? curr->elmt : NULL; }
  const T  *First() const  { return count > 0 ? first->elmt : NULL; }
  const T  *Last()  const  { return count > 0 ? last->elmt : NULL; }
  const T  *Next()  const  { return curr && curr->next ? curr->next->elmt : NULL; }
  const T  *Prev()  const  { return curr && curr->prev ? curr->prev->elmt : NULL; }
  const T  *GoNext() const  { if (curr) curr = curr->next; return Curr(); }
  const T  *GoPrev() const    { if (curr) curr = curr->prev; return Curr(); }

  // routines that move the curr elmt
  T  *GoNext() { if (curr) curr = curr->next; return Curr(); }
  T  *GoPrev() { if (curr) curr = curr->prev; return Curr(); }

  // write & alloc fcns - returns new elmt
  T  * Append();   // add elmt to end of list
  T  * Prepend();   // add elmt to begginning of list

  // write & alloc & copy - returns list after copying elmt
  List<T>& AppendCopy (const List<T> &list);
  List<T>& AppendCopy (const T& elmt);   // add elmt to end of list
  List<T>& PrependCopy (const T& elmt);   // add elmt to begginning of list

  // Special append -- does not copy element
  void AppendNoCopy(T* pElmt);

  void convertTo(std::list<T> &stdList);
  void convertFrom(const std::list<T> &stdList);

  // Remove current element from list
  void RemoveCurrFromList();
  // Remove all elements from list
  void RemoveAll();
};



//////////////////////////
// Base List template  //
//       CODE         //
///////////////////////
template <class T>
void List<T>::convertTo(std::list<T> &stdList)
{
   // PIERCE: Note should I clear stdList first?
   //
   SetCurrToFirst();
   for (; Curr() != NULL; GoNext())
   {
      stdList.push_back(*Curr());
   }
}

// Copy elements from stdList into this list.
//
template <class T>
void List<T>::convertFrom(const std::list<T> &stdList)
{
   RemoveAll();

   typename std::list<T>::const_iterator ci;
   ci = stdList.begin();
   for (; ci != stdList.end(); ci++)
      AppendCopy(*ci);
}

template <class T>
void List<T>::SetCurrElmt (unsigned long index) const
{
    unsigned long i;
    curr = first;
    for (i = 0; (i < (count-1)) && (i < index); i++)
        curr = curr->next;
}

template <class T>
unsigned long List<T>::GetCurrElmtIndex(void) const
{
   unsigned long int i;
   ListElmt *tmp;
   if (curr != NULL)
   {
      for (i = 0, tmp = first; tmp != NULL; i++)
      {
         if (tmp == curr)
            return i;
         else
            tmp = tmp->next;
      }
   }
   return count;
}

// alloc new list elmt, put at end of list
//  and return the component type
template <class T>
T * List<T>::Append()
{
    ListElmt *newElmt;
    newElmt  = new ListElmt;
    newElmt->elmt  = new T;
    newElmt->next = NULL;
    if (last == NULL)
    {
        newElmt->prev = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->prev = last;
        last->next    = newElmt;
        last          = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;

} /* List::Append */


// alloc new list elmt, put at end of list
//  and add the component
template <class T>
void List<T>::AppendNoCopy(T* pElmt)
{
    if (pElmt == NULL)
		return;

    ListElmt *newElmt;
    newElmt  = new ListElmt;
    newElmt->elmt  = pElmt;
    newElmt->next = NULL;
    if (last == NULL)
    {
        newElmt->prev = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->prev = last;
        last->next    = newElmt;
        last          = newElmt;
    }
    count++;
} /* List::AppendNoCopy */


// alloc new list elmt, put at beggining of list
//  and return the component type
template <class T>
T * List<T>::Prepend()
{
    ListElmt *newElmt;
    newElmt  = new ListElmt;
    newElmt->elmt = new T;
    newElmt->prev = NULL;
    if (first == NULL)
    {
        newElmt->next = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->next = first;
        first->prev   = newElmt;
        first         = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;
} /* AsnList::Prepend */


template <class T>
List<T>& List<T>::AppendCopy (const List<T>& list)
{
	ListElmt* pTemp = list.first;
	ListElmt* pLast = list.last;
	
	// Append all but the last element
	while (pTemp != pLast)
	{
		AppendCopy(*pTemp->elmt);
		pTemp = pTemp->next;
	}
	
	// Append the last element
	if (pTemp != NULL)
		AppendCopy(*pTemp->elmt);
	
	return *this;
}


template <class T>
List<T>& List<T>::AppendCopy (const T& elmt)
{
    ListElmt *newElmt;
    newElmt  = new ListElmt;
    newElmt->elmt = new T;
    *newElmt->elmt = elmt;
    newElmt->next = NULL;
    if (last == NULL)
    {
        newElmt->prev = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->prev = last;
        last->next    = newElmt;
        last          = newElmt;
    }
    count++;
    return *this;

} /* AppendAndCopy */

template <class T>
List<T>& List<T>::PrependCopy (const T& elmt)
{
    ListElmt *newElmt;
    newElmt  = new ListElmt;
    newElmt->elmt = new T;
    *newElmt->elmt = elmt;
    newElmt->prev = NULL;
    if (first == NULL)
    {
        newElmt->next = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->next = first;
        first->prev   = newElmt;
        first         = newElmt;
    }
    count++;
    return *this;

} /* PrependAndCopy */


template <class T>
void List<T>::RemoveCurrFromList()
{
    ListElmt *del_elmt;

    if (curr != NULL)
    {
        del_elmt = curr;
        count--;

        if (count == 0)
            first = last = curr = NULL;
        else if (curr == first)
        {
            curr = first= first->next;
            first->prev = NULL;
        }
        else if (curr == last)
        {
            curr = last = last->prev;
            last->next = NULL;
        }
        else
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            curr = curr->next;
        }

        delete del_elmt->elmt;
        delete del_elmt;
    }
}

template <class T>
void List<T>::RemoveAll()
{
	curr = first;
	while (curr != NULL)
	{
		ListElmt* next = curr->next;
		delete curr->elmt;
		delete curr;
		curr = next;
	}

	count = 0;
	first = NULL;
	last = NULL;
}

template <class T>
List<T>::List(const List<T> &that)
{
   count = 0; 
   first = curr = last = NULL;
   operator=(that);
}

template <class T>
List<T>& List<T>::operator=(const List<T>& that)
{
	if (this != &that)
		AppendCopy(that);
	return *this;
}

#if 0
#if META

const SNACC::AsnTypeDesc AsnList::_desc (NULL, NULL, false, SNACC::AsnTypeDesc::SET_or_SEQUENCE_OF, NULL);

const SNACC::AsnTypeDesc *AsnList::_getdesc() const
{
  return &_desc;
}

#if TCL

int AsnList::TclGetVal (Tcl_Interp *interp) const
{
	return TCL_ERROR;
}

int AsnList::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
	return TCL_ERROR;
}

#endif /* TCL */
#endif /* META */
#endif /* 0 */

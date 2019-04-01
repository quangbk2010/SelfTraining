	#include <stdio.h>
#include <stdlib.h>
		struct node 
	{
	  int value;
	  struct node *next;
	};
	void rearrange(struct node *list)
	{
  	  struct node *p, * q;
	  int temp;
	  if ((!list) || !list->next) 
	      return;
	  p = list;
	  q = list->next;
	  while(q) 
	  {
	     temp = p->value;
         p->value = q->value;
	     q->value = temp;
	     p = q->next;
	     q = p?p->next:0;
	  }
	}
	void print_list (struct node *list) {
		struct node *temp;

	}

	int main()
	{
		int i = 1, k;
		struct node *n;
		struct node *temp;
		n = (struct node*) malloc (20 * sizeof (struct node));
	    while (i < 7)  {
	    	n -> value = i;
	    	n = n -> next;l
	    	i ++;
	    	k = i;//n -> value;
	    	printf("%d\t", n -> value);
	    }
	   

	    return 0;
	}